#include "model/basicModel.h"
#include "state/pv2D.h"

namespace hmm {

Eigen::MatrixXd basicModel::Phi(double dt) const {
    Eigen::MatrixXd result = Eigen::MatrixXd::Zero(4, 4);
    result << 1, 0, 1, 0,
              0, 1, 0, 1,
              0, 0, 1, 0,
              0, 0, 0, 1;

    result(0, 2) = dt;
    result(1, 3) = dt;

    return result;
}

Eigen::MatrixXd basicModel::Q(double dt) const {
    double noise_ax = 9;  double noise_ay = 9;

    double dt_2 = dt * dt;
    double dt_3 = dt_2 * dt;
    double dt_4 = dt_3 * dt;

    Eigen::MatrixXd result = Eigen::MatrixXd::Zero(4, 4);
    result << dt_4/4*noise_ax,   0,                dt_3/2*noise_ax,  0,
              0,                 dt_4/4*noise_ay,  0,                dt_3/2*noise_ay,
              dt_3/2*noise_ax,   0,                dt_2*noise_ax,    0,
              0,                 dt_3/2*noise_ay,  0,                dt_2*noise_ay;

    return result;
}

std::shared_ptr<state> basicModel::predict(std::shared_ptr<state> sv, int64_t t) {
    // compute the time elapsed between the current and previous measurements
    double dt = (t - sv->time()) / 1000000.0;  //  in seconds

    std::shared_ptr<state> result = sv->create();
    result->time() = t;

    Eigen::MatrixXd F = Phi(dt);
    result->mean() = F * sv->mean();
    result->cov()  = F * sv->cov() * F.transpose() + Q(dt);

    return result;
}

} /* namespace hmm */
