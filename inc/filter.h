#pragma once

#include <vector>

#include "model.h"
#include "state.h"
#include "measurement.h"

namespace hmm {

class filter {
public:
    filter(std::shared_ptr<state> init);

    virtual ~filter() = default;

    virtual void addTimestep(int64_t t);

    virtual void setModel(const std::shared_ptr<model>& m);

    virtual void push(std::shared_ptr<measurement> meas);
    virtual std::shared_ptr<measurement> pop();

    virtual void predict(int64_t t) = 0;
    virtual void update(std::shared_ptr<measurement> meas) = 0;

    virtual std::vector<std::shared_ptr<state>> run();

protected:
    std::vector<int64_t> timesteps_;
    std::vector<std::shared_ptr<state>> estimates_;

    std::shared_ptr<model> model_;
    std::shared_ptr<state> current_;

    std::vector<std::shared_ptr<measurement>> unprocessedMeasurements_; // min heap
    std::vector<std::shared_ptr<measurement>>   processedMeasurements_; // normal vector
};

} /* namespace hmm */
