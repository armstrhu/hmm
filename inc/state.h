#pragma once

#include <memory>

#include <Eigen/Dense>

#include "data.h"

namespace hmm {

class observation;
class measurement;

class state : public data {
protected:
    friend class stateFactory;
    state(int d);

public:
    virtual ~state() = default;

    virtual Eigen::MatrixXd jacobian(std::shared_ptr<measurement> obs,  void* user = nullptr);
    virtual data simulate(std::shared_ptr<measurement> obs,  void* user = nullptr);

    virtual std::unique_ptr<state> create() = 0;
    virtual std::unique_ptr<state> copy() = 0;
};

} /* namespace hmm */
