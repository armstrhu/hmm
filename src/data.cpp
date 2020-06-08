#include "data.h"

namespace hmm {

data::data(int d) : id_(boost::uuids::random_generator()()), dim_(d) {
    reset(d);
}

bool data::operator==(const data& rhs) {
    return this->id_ == rhs.id_;
}

bool data::operator<(const data& rhs) {
    return this->time() < rhs.time();
}

void data::reset(int d) {
    const_cast<int&>(dim_) = d;
    time_ = 0;
    mean_ = Eigen::VectorXd(d);
    cov_ = Eigen::MatrixXd(d,d);
}

int data::dim() const {
    return dim_;
}

int64_t& data::time() {
    return time_;
}

const int64_t& data::time() const {
    return time_;
}

Eigen::VectorXd& data::mean() {
    return mean_;
}

double& data::mean(int i) {
    return mean_(i);
}

const Eigen::VectorXd& data::mean() const {
    return mean_;
}

const double& data::mean(int i) const {
    return mean_(i);
}

Eigen::MatrixXd& data::cov() {
    return cov_;
}

double& data::cov(int i, int j) {
    return cov_(i,j);
}

const Eigen::MatrixXd& data::cov() const {
    return cov_;
}

const double& data::cov(int i, int j) const {
    return cov_(i,j);
}

} /* namespace hmm */
