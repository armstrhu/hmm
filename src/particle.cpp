#include "particle.h"

#include <boost/random.hpp>

namespace hmm {

particle::particle(): weight_(1.0) {
    // TODO Auto-generated constructor stub

}

particle::~particle() {
    // TODO Auto-generated destructor stub
}

std::shared_ptr<state>& particle::getState() {
    return state_;
}

double& particle::weight() {
    return weight_;
}

const double& particle::weight() const {
    return weight_;
}

particle particle::clone() {
    particle result(*this);
    result.state_ = result.state_->copy();
    return result;
}

void particle::regularize(boost::variate_generator<boost::random::mt19937,
                          boost::normal_distribution<double>>& gen) {
    Eigen::MatrixXd A = state_->cov().llt().matrixL(); // choleski decomposition
    return regularize(A, gen);
}

void particle::regularize(const Eigen::MatrixXd& A,
                          boost::variate_generator<boost::random::mt19937,
                          boost::normal_distribution<double>>& gen) {
    int dim = state_->dim();
    Eigen::VectorXd mean = Eigen::VectorXd::Zero(dim);
    for(int i = 0; i < dim; ++i) {
        mean(i) = gen();
    }

    state_->mean() += A * mean;
}

} /* namespace hmm */
