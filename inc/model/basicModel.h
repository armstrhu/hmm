#pragma once

#include "state.h"
#include "model.h"

namespace hmm {

class basicModel : public model {
public:
    basicModel() = default;

    virtual ~basicModel() = default;

    std::shared_ptr<state>
    predict(std::shared_ptr<state> sv, int64_t t) override;
};

} /* namespace filter */
