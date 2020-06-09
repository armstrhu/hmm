#pragma once

#include "measurement.h"
#include "state/pv2D.h"

namespace hmm {

class radar : public measurement {
public:
    radar();
    virtual ~radar() = default;

    Eigen::MatrixXd jacobian(pv2D* sv, void* user = nullptr) const override;
    data simulate(pv2D* sv, void* user = nullptr) override;

    virtual std::unique_ptr<measurement> create() override;
    virtual std::unique_ptr<measurement> copy() override;
};

} /* namespace filter */
