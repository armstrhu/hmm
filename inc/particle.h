#pragma once

#include <memory>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random.hpp>

#include "state.h"

namespace hmm {

class particle {
public:
    particle();
    virtual ~particle();

    std::shared_ptr<state>& getState();

    double& weight();
    const double& weight() const;

    particle clone();

    void regularize(const Eigen::MatrixXd& A,
                    boost::variate_generator<boost::random::mt19937,
                    boost::normal_distribution<double>>& gen);

    void regularize(boost::variate_generator<boost::random::mt19937,
                    boost::normal_distribution<double>>& gen);

    std::shared_ptr<state> state_;
protected:

    double weight_;
};

} /* namespace hmm */
