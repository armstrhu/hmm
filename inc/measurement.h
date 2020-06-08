#pragma once

#include <memory>

#include <Eigen/Dense>

#include "data.h"

namespace hmm {

class state;
class pv2D;

class measurement : public data {
protected:
    friend class measurementFactory;
    measurement(int d);

public:
    virtual ~measurement() = default;

    Eigen::MatrixXd jacobian(state* sv, void* user = nullptr) const;
    data simulate(state* sv,  void* user = nullptr);

    virtual Eigen::MatrixXd jacobian(pv2D* sv, void* user = nullptr) const ;
    virtual data simulate(pv2D* sv,  void* user = nullptr);
};

class measurementFactory {
public:
    measurementFactory() = default;
    ~measurementFactory() = default;

    template<typename T>
    static std::unique_ptr<measurement> create();
};

template<typename T>
std::unique_ptr<measurement> measurementFactory::create() {
    return std::unique_ptr<measurement>(new T);
}

} /* namespace hmm */
