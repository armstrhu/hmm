#pragma once

#include <memory>

#include <Eigen/Dense>

#include "state.h"
#include "measurement.h"

namespace hmm {

class observation;
class measurement;

class pv2D : public state {
public:
    pv2D();
    virtual ~pv2D() = default;

    Eigen::MatrixXd jacobian(std::shared_ptr<measurement> obs,  void* user)  override;
    data simulate(std::shared_ptr<measurement> obs,  void* user)  override;

    virtual std::unique_ptr<state> create() override;
    virtual std::unique_ptr<state> copy() override;
};

} /* namespace hmm */
