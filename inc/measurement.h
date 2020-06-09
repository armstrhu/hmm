#pragma once

#include <memory>

#include <Eigen/Dense>

#include "data.h"

namespace hmm {

class state;
class pv2D;

class measurement : public data {
public:
    measurement(int d);
    virtual ~measurement() = default;

    Eigen::MatrixXd jacobian(state* sv, void* user = nullptr) const;
    data simulate(state* sv,  void* user = nullptr);

    virtual Eigen::MatrixXd jacobian(pv2D* sv, void* user = nullptr) const ;
    virtual data simulate(pv2D* sv,  void* user = nullptr);

    virtual std::unique_ptr<measurement> create() = 0;
    virtual std::unique_ptr<measurement> copy() = 0;
};

} /* namespace hmm */
