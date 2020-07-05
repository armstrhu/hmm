#include "utils/stats.h"

#include <boost/math/distributions/chi_squared.hpp>

namespace hmm {

double mahalanobis2(const Eigen::VectorXd& p, const Eigen::MatrixXd& cov) {
    // p^T * cov^-1 * p
    Eigen::LDLT<Eigen::MatrixXd> dec(cov);
    return (p.transpose() * dec.solve(p)).norm();
}

double mahalanobis2(const Eigen::VectorXd& p,
                    const Eigen::VectorXd& x, const Eigen::MatrixXd& cov) {
    return mahalanobis2(x-p, cov);
}

double chi2pvalue(double chi2, int N) {
    boost::math::chi_squared_distribution<double> dist(N);
    return 1.0 - boost::math::cdf(dist, chi2);
}

} /* namespace hmm */
