#include "meas/radar.h"

namespace hmm {

radar::radar() : measurement(3) {}

Eigen::MatrixXd radar::jacobian(pv2D* sv, void* user) const {
    (void)user;

    Eigen::MatrixXd H(3,4);
    H <<  0,0,0,0,
          0,0,0,0,
          0,0,0,0;

    //recover state parameters
    double px = sv->mean(0);  double py = sv->mean(1);
    double vx = sv->mean(2);  double vy = sv->mean(3);

    //pre-compute a set of terms to avoid repeated calculation
    double c1 = px*px+py*py;
    double c2 = sqrt(c1);
    double c3 = (c1*c2);

/*
    //check division by zero
    if(fabs(c1) < 0.0001){
      std::cout << "Function CalculateJacobian() has Error: Division by Zero" << std::endl;
      return H;
    }
*/

    //compute the Jacobian matrix
    H  << (px/c2),                (py/c2),                0,      0,
          -(py/c1),               (px/c1),                0,      0,
          py*(vx*py - vy*px)/c3,  px*(px*vy - py*vx)/c3,  px/c2,  py/c2;

    return H;
}

data radar::simulate(pv2D* sv, void* user) {
    (void)user;

    data obs(this->dim());

    double px, py, vx, vy;
    px = sv->mean(0);
    py = sv->mean(1);
    vx = sv->mean(2);
    vy = sv->mean(3);

    double rho, phi, rho_dot;
    rho = sqrt(px*px + py*py);
    phi = atan2(py, px);  // returns values between -pi and pi

    // if rho is very small, set it to 0.0001 to avoid division by 0 in computing rho_dot
    if(rho < 0.000001) rho = 0.000001;

    rho_dot = (px * vx + py * vy) / rho;

    obs.mean() = Eigen::VectorXd(3);
    obs.mean() << rho, phi, rho_dot;

    return obs;
}

} /* namespace filter */
