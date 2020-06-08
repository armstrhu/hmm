#include "filter.h"

namespace hmm {

filter::filter(std::shared_ptr<state> init) : current_(init) {}

void filter::addTimestep(int64_t t) {
    timesteps_.push_back(t);
}

void filter::setModel(const std::shared_ptr<model>& m) {
    model_ = m;
}

void filter::push(std::shared_ptr<measurement> meas) {
    unprocessedMeasurements_.push_back(meas);
    std::push_heap(unprocessedMeasurements_.begin(), unprocessedMeasurements_.end(),
                   [](std::shared_ptr<measurement> lhs, std::shared_ptr<measurement> rhs){return lhs->time() > rhs->time();});
}

std::shared_ptr<measurement> filter::pop() {
    std::shared_ptr<measurement> meas = unprocessedMeasurements_.front();
    std::pop_heap(unprocessedMeasurements_.begin(), unprocessedMeasurements_.end(),
                  [](std::shared_ptr<measurement> lhs, std::shared_ptr<measurement> rhs){return lhs->time() > rhs->time();});
    unprocessedMeasurements_.pop_back();
    return meas;
}

std::vector<std::shared_ptr<state>> filter::run() {
    std::sort(timesteps_.begin(), timesteps_.end());
    timesteps_.erase(std::unique(timesteps_.begin(), timesteps_.end()), timesteps_.end());

    // should we flip the order of the loop so that measurements are on the outside?
    for (auto itr = timesteps_.begin(); itr != timesteps_.end(); ++itr) {
        predict(*itr);
        while(!unprocessedMeasurements_.empty()) {
           // right now, this will only process a measurement if it is exactly on a time step
           if(unprocessedMeasurements_.front()->time() != *itr) break;

           std::shared_ptr<measurement> meas = pop();
           update(meas);
           processedMeasurements_.push_back(meas);
        }
    };
    return estimates_;
}

} /* namespace filter */
