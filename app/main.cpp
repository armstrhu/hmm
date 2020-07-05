#include <string>
#include <fstream>
#include <cmath>
#include <iostream>

#include "state/pv2D.h"
#include "meas/laser.h"
#include "meas/radar.h"
#include "model/basicModel.h"
#include "filter/ekf.h"
#include "filter/pf.h"

void parseData(const std::string& in,
               std::vector<std::shared_ptr<hmm::measurement>>& allMeas,
               std::vector<Eigen::VectorXd>& truth) {

      std::ifstream in_file(in.c_str(), std::ifstream::in);

      std::string line;
      while (std::getline(in_file, line)) {
        std::string sensor_type;

        std::istringstream iss(line);

        // reads first element from the current line
        iss >> sensor_type;
        if (sensor_type.compare("L") == 0) {
          // LASER MEASUREMENT
          std::unique_ptr<hmm::measurement> meas(new hmm::laser);

          // read measurements at this timestamp
          double x;
          double y;
          int64_t timestamp;
          iss >> x;
          iss >> y;
          iss >> timestamp;

          meas->reset(2);
          meas->time() = timestamp;
          meas->mean() << x, y;
          meas->cov()  << 0.0225, 0,
                          0, 0.0225;

          allMeas.push_back(std::move(meas));
        } else if (sensor_type.compare("R") == 0) {
          // RADAR MEASUREMENT
          std::unique_ptr<hmm::measurement> meas(new hmm::radar);

          // read measurements at this timestamp
          double ro;
          double phi;
          double ro_dot;
          int64_t timestamp;
          iss >> ro;
          iss >> phi;
          iss >> ro_dot;
          iss >> timestamp;

          meas->reset(3);
          meas->time() = timestamp;
          meas->mean() << ro, phi, ro_dot;
          meas->cov()  << 0.09, 0, 0,
                          0, 0.0009, 0,
                          0, 0, 0.09;

          allMeas.push_back(std::move(meas));
        }

        // read ground truth data to compare later
        double x_gt;
        double y_gt;
        double vx_gt;
        double vy_gt;
        iss >> x_gt;
        iss >> y_gt;
        iss >> vx_gt;
        iss >> vy_gt;

        Eigen::VectorXd gt = Eigen::VectorXd(4);
        gt << x_gt, y_gt, vx_gt, vy_gt;
        truth.push_back(gt);
      }
}

int main(int argc, char* argv[]) {
  std::vector<std::shared_ptr<hmm::measurement>> allMeas;

  std::vector<Eigen::VectorXd> truth;
  parseData(argv[2], allMeas, truth);

  std::shared_ptr<hmm::state> s0(new hmm::pv2D);
  s0->time() = allMeas[0]->time();
  s0->mean() = 5.0 * Eigen::VectorXd::Ones(s0->dim());
  s0->cov() << 10, 0, 0, 0,
               0, 10, 0, 0,
               0, 0, 1000, 0,
               0, 0, 0, 1000;

  std::shared_ptr<hmm::filter> filter;
  if(strcmp(argv[1], "-kf") == 0) {
      filter.reset(new hmm::ekf(s0));
  } else if (strcmp(argv[1], "-pf") == 0) {
      filter.reset(new hmm::pf(s0));
  } else {
      throw 888;
  }

  for(auto itr = allMeas.begin(); itr != allMeas.end(); ++itr) {
      filter->addTimestep((*itr)->time());
      filter->push(*itr);
  }

  std::shared_ptr<hmm::model> m(new hmm::basicModel);
  filter->setModel(m);
  std::vector<std::shared_ptr<hmm::state>> estimates = filter->run();

  double rms = 0.0;
  for(size_t i = 0; i < truth.size(); ++i) {
      std::cout << estimates[i]->mean().transpose() << "\t" << truth[i].transpose() << std::endl;
      double diff = (estimates[i]->mean() - truth[i]).norm();
      rms += diff * diff;
  }
  std::cout << std::sqrt(rms/truth.size()) << std::endl;
}
