#pragma once

#include <memory>

#include <Eigen/Dense>

#include "state.h"
#include "measurement.h"

namespace hmm {

class observation;
class measurement;

class pv2D : public state {
protected:
    friend class stateFactory;
    pv2D();

public:
    virtual ~pv2D() = default;

    Eigen::MatrixXd jacobian(std::shared_ptr<measurement> obs,  void* user)  override;
    data simulate(std::shared_ptr<measurement> obs,  void* user)  override;
};

} /* namespace hmm */
