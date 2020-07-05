#pragma once

#include <Eigen/Dense>

namespace hmm {

double mahalanobis2(const Eigen::VectorXd& p, const Eigen::MatrixXd& cov);

double mahalanobis2(const Eigen::VectorXd& p,
                    const Eigen::VectorXd& x, const Eigen::MatrixXd& cov);

double chi2pvalue(double chi2, int N);

} /* namespace hmm */
