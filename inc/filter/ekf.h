#pragma once

#include "filter.h"

namespace hmm {

class ekf : public filter {

public:
    ekf() = delete;
    ekf(std::shared_ptr<state> init);

    virtual ~ekf() = default;

    virtual void predict(int64_t t) override;
    virtual void update(std::shared_ptr<measurement> meas) override;
};

} /* namespace filter */
