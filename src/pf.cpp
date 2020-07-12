#include <map>

#include <omp.h>

#include "filter/pf.h"
#include "utils/stats.h"

namespace hmm {

pf::pf(std::shared_ptr<state> init) : filter(init), rng_(time(0)),
       uniform_dist_(0.0, 1.0), normal_dist_(0.0, 1.0),
       GaussianGenerator(rng_, normal_dist_) {

    particle par;
    par.state_ = current_;

    int N = 1E3;
    particles_.reserve(N);
    for(int n = 0; n < N; ++n) {
        particles_.push_back(par.clone());
        particles_.back().regularize(GaussianGenerator);
    }
    renormalize();
}

void pf::predict(int64_t t) {
    double dt = (t - current_->time())*1.0E-6;

    Eigen::MatrixXd F = model_->Phi(dt);
#pragma omp parallel for schedule(dynamic)
    for(size_t i = 0; i < particles_.size(); ++i) {
        particles_[i].getState()->mean() = F * particles_[i].getState()->mean();
    }

    // update state estimate
    updateCurrentEstimate();
    current_->time() = t;
    estimates_.push_back(current_->copy());
}

void pf::update(std::shared_ptr<measurement> meas) {
  Eigen::MatrixXd H = current_->jacobian(meas);
  Eigen::MatrixXd noise = H * current_->cov() * H.transpose();

#pragma omp parallel for schedule(dynamic)
  for(size_t i = 0; i < particles_.size(); ++i) {
      data obs = particles_[i].getState()->simulate(meas); // simulate the expected observation given current state
      double m2 = mahalanobis2(obs.mean(), meas->mean(), meas->cov() + noise); // add uncertainty to account for undersampled particles
      particles_[i].weight() *= chi2pvalue(m2, meas->dim());
  }

  renormalize();
  resample();
  updateCurrentEstimate();
}

void pf::renormalize() {
    int np = omp_get_max_threads();

    std::vector<double> sums(np, 0.0);
#pragma omp parallel for schedule(dynamic)
    for(size_t i = 0; i < particles_.size(); ++i) {
        sums[omp_get_thread_num()] += particles_[i].weight();
    }
    double sum = 0.0;
    for(int i = 0; i < np; ++i) {
        sum += sums[i];
    }
    if(sum == 0) throw 999;

    sum = 1.0/sum; // inverse so we can multiply vs divide
#pragma omp parallel for schedule(dynamic)
    for(size_t i = 0; i < particles_.size(); ++i) {
        particles_[i].weight() *= sum;
    }
}

void pf::resample() {
    // TODO: make mo betta...this resampling algorithm is slow
    std::map<double, particle> cdf;
    double cum = 0.0;
    for(auto p = particles_.begin(); p != particles_.end(); ++p) {
        if(p->weight() == 0.0) continue;

        cum += p->weight();
        cdf[cum] = *p;
    }
    if(cum == 0) throw 999;

    Eigen::MatrixXd A = model_->Q(1.0).llt().matrixL(); // choleski decomposition, use process noise 1.0 second dt

    size_t N = particles_.size();
    std::vector<particle> particles;
    particles.reserve(N);
    for(size_t n = 0; n < N; ++n) {
        double x = uniform_dist_(rng_);
        auto itr = cdf.lower_bound(x);
        particle par = itr->second.clone();
        par.regularize(A, GaussianGenerator); // regularize with process noise from model
        particles.push_back(std::move(par));
    }
    std::swap(particles_, particles);

    renormalize();
}

void pf::updateCurrentEstimate() {
    size_t np = omp_get_max_threads();

    std::vector<Eigen::VectorXd> means(np, Eigen::VectorXd::Zero(current_->dim()));
#pragma omp parallel for schedule(dynamic)
    for(size_t i = 0; i < particles_.size(); ++i) {
        means[omp_get_thread_num()] += particles_[i].weight() * particles_[i].getState()->mean();
    }
    Eigen::VectorXd mean = means[0];
    for(size_t i = 1; i < np; ++i) {
        mean += means[i];
    }

    std::vector<Eigen::MatrixXd> covs(np, Eigen::MatrixXd::Zero(current_->dim(), current_->dim()));
#pragma omp parallel for schedule(dynamic)
    for(size_t i = 0; i < particles_.size(); ++i) {
        Eigen::MatrixXd C = particles_[i].getState()->mean() - mean;
        covs[omp_get_thread_num()] += particles_[i].weight() * C * C.transpose();
    }
    Eigen::MatrixXd cov = covs[0];
    for(size_t i = 1; i < np; ++i) {
        cov += covs[i];
    }

    std::swap(current_->mean(), mean);
    std::swap(current_->cov(), cov);
}

} /* namespace hmm */
