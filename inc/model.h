#pragma once

#include <Eigen/Dense>

#include "state.h"

namespace hmm {

class model {
public:
    model() = default;

    virtual ~model() = default;

    virtual std::shared_ptr<state>
    predict(std::shared_ptr<state>, int64_t t) = 0;
};

} /* namespace hmm */
