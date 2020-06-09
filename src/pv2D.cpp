#include "state/pv2D.h"

namespace hmm {

pv2D::pv2D() : state(4) {}

Eigen::MatrixXd pv2D::jacobian(std::shared_ptr<measurement> obs,  void* user) {
    return obs->jacobian(this, user);
}

data pv2D::simulate(std::shared_ptr<measurement> obs,  void* user) {
    return obs->simulate(this, user);
}

std::unique_ptr<state> pv2D::create() {
    return std::unique_ptr<state>(new pv2D);
}

std::unique_ptr<state> pv2D::copy() {
    return std::unique_ptr<state>(new pv2D(*this));
}

} /* namespace hmm */
