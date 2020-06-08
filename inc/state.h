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
};

class stateFactory {
public:
    stateFactory() = default;
    ~stateFactory() = default;

    template<typename T>
    static std::unique_ptr<state> create();
};

template<typename T>
std::unique_ptr<state> stateFactory::create() {
    return std::unique_ptr<state>(new T);
}

} /* namespace hmm */
