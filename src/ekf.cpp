#include "filter/ekf.h"

namespace hmm {

ekf::ekf(std::shared_ptr<state> init) : filter(init) {}

void ekf::predict(int64_t t) {
    current_ = model_->predict(current_, t);
    estimates_.push_back(current_);
}

void ekf::update(std::shared_ptr<measurement> meas) {
    Eigen::MatrixXd H = current_->jacobian(meas);
    Eigen::MatrixXd S = meas->cov() + H * current_->cov() * H.transpose();
    Eigen::MatrixXd K = current_->cov() * H.transpose() * S.inverse(); // see if we can avoid this inverse

    // update mean
    data obs = current_->simulate(meas); // simulate the expected observation given current state
    Eigen::VectorXd y = meas->mean() - obs.mean();
    current_->mean() = current_->mean() + K * y;

    // update covariance (Joseph form)
    Eigen::MatrixXd A = Eigen::MatrixXd::Identity(current_->dim(), current_->dim()) - K * H;
    current_->cov() = A * current_->cov() * A.transpose() + K * meas->cov() * K.transpose();
}

} /* namespace filter */
