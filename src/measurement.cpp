#include "measurement.h"
#include "state.h"

namespace hmm {

measurement::measurement(int d) : data(d) {}

Eigen::MatrixXd measurement::jacobian(state* sv,  void* user) const {
    throw std::runtime_error("Jacobian not defined for state");
}

data measurement::simulate(state* sv,  void* user) {
    throw std::runtime_error("Visitor not defined for state");
}

Eigen::MatrixXd measurement::jacobian(pv2D* sv,  void* user) const {
    throw std::runtime_error("Jacobian not defined for pv2D");
}

data measurement::simulate(pv2D* sv,  void* user) {
    throw std::runtime_error("Visitor not defined for pv2D");
}

} /* namespace hmm */
