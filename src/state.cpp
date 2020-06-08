#include "measurement.h"
#include "state.h"

namespace hmm {

state::state(int d) : data(d) {}

Eigen::MatrixXd state::jacobian(std::shared_ptr<measurement> obs,  void* user) {
    return obs->jacobian(this, user);
}

data state::simulate(std::shared_ptr<measurement> obs,  void* user) {
    return obs->simulate(this, user);
}

} /* namespace hmm */
