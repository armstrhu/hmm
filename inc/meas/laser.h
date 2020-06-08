#pragma once

#include "measurement.h"
#include "state/pv2D.h"

namespace hmm {

class laser : public measurement {
protected:
    friend class measurementFactory;
    laser();

public:
    virtual ~laser() = default;

    Eigen::MatrixXd jacobian(pv2D* sv, void* user = nullptr) const override;
    data simulate(pv2D* sv, void* user = nullptr) override;
};

} /* namespace filter */
