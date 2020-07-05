#pragma once

#include <Eigen/Dense>

#include "state.h"

namespace hmm {

class model {
public:
    model() = default;

    virtual ~model() = default;

    virtual Eigen::MatrixXd Phi(double dt) const = 0;
    virtual Eigen::MatrixXd Q(double dt) const = 0;

    virtual std::shared_ptr<state>
    predict(std::shared_ptr<state>, int64_t t) = 0;
};

} /* namespace hmm */
