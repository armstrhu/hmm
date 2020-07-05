#pragma once

#include <vector>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

#include "filter.h"
#include "particle.h"

namespace hmm {

class pf : public filter {
public:
    pf() = delete;
    pf(std::shared_ptr<state> init);

    virtual ~pf() = default;

    virtual void predict(int64_t t);
    virtual void update(std::shared_ptr<measurement> meas);
protected:

    void renormalize();
    void resample();
    void updateCurrentEstimate();

    boost::random::mt19937 rng_;
    boost::uniform_real<double> uniform_dist_;
    boost::normal_distribution<double> normal_dist_;
    boost::variate_generator<boost::random::mt19937,
                             boost::normal_distribution<double>>
                               GaussianGenerator;

    std::vector<particle> particles_;
};

} /* namespace hmm */
