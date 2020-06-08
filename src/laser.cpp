#include "meas/laser.h"

namespace hmm {

laser::laser() : measurement(2) {}

Eigen::MatrixXd laser::jacobian(pv2D* sv, void* user) const {
    (void)user;

    Eigen::MatrixXd H = Eigen::MatrixXd(2,4);
    H << 1, 0, 0, 0,
         0, 1, 0, 0;
    return H;
}

data laser::simulate(pv2D* sv, void* user) {
    (void)user;

    Eigen::MatrixXd H = jacobian(sv);

    data obs(this->dim());
    obs.mean() = H * sv->mean();
    obs.cov()  = H * sv->cov() * H.transpose();
    return obs;
}

} /* namespace filter */
