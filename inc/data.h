#pragma once

#include <cstdint>

#include <Eigen/Dense>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace hmm {

class data {
public:
    data() = delete;
    data(int d);

    bool operator<(const data& rhs);

    void reset(int d);

    int dim() const;

    int64_t& time();
    const int64_t& time() const;

    Eigen::VectorXd& mean();
    double& mean(int i);
    const Eigen::VectorXd& mean() const;
    const double& mean(int i) const;

    Eigen::MatrixXd& cov();
    double& cov(int i, int j);
    const Eigen::MatrixXd& cov() const;
    const double& cov(int i, int j) const;

private:
    const int dim_;

    int64_t time_;
    Eigen::VectorXd mean_;
    Eigen::MatrixXd cov_;
};

} /* namespace hmm */


