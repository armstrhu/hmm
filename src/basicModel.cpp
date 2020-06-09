#include "model/basicModel.h"
#include "state/pv2D.h"

namespace hmm {

std::shared_ptr<state> basicModel::predict(std::shared_ptr<state> sv, int64_t t) {
    double noise_ax = 9;  double noise_ay = 9;

    // compute the time elapsed between the current and previous measurements
    double dt = (t - sv->time()) / 1000000.0;  //  in seconds

    double dt_2 = dt * dt;
    double dt_3 = dt_2 * dt;
    double dt_4 = dt_3 * dt;

    Eigen::MatrixXd F = Eigen::MatrixXd::Zero(4, 4);
    F << 1, 0, 1, 0,
         0, 1, 0, 1,
         0, 0, 1, 0,
         0, 0, 0, 1;

    F(0, 2) = dt;
    F(1, 3) = dt;

    //set the process covariance matrix Q
    Eigen::MatrixXd Q = Eigen::MatrixXd::Zero(4, 4);
    Q << dt_4/4*noise_ax,   0,                dt_3/2*noise_ax,  0,
         0,                 dt_4/4*noise_ay,  0,                dt_3/2*noise_ay,
         dt_3/2*noise_ax,   0,                dt_2*noise_ax,    0,
         0,                 dt_3/2*noise_ay,  0,                dt_2*noise_ay;

    std::shared_ptr<state> result = sv->create();
    result->time() = t;
    result->mean() = F * sv->mean();
    result->cov()  = F * sv->cov() * F.transpose() + Q;

    return result;
}

} /* namespace hmm */
