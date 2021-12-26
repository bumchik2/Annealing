#pragma once

#include "penalty.h"
#include "mutation.h"
#include "problem.h"

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

class Annealing {
public:
    Annealing(Problem& problem, double temperature, const vector<shared_ptr<Penalty>>& penalties,
              const vector<shared_ptr<Mutation>>& mutations, int steps_number):
            problem_(problem), temperature_(temperature), initial_temperature_(temperature_),
            penalties_(penalties), mutations_(mutations), steps_number_(steps_number) {
        init_();
    }

    void work();

    vector<double> penalty_history;

private:
    void init_();
    void update_temperature_(int step_number);
    double try_mutate_();

    Problem& problem_;
    double temperature_;
    const double initial_temperature_;

    vector<shared_ptr<Penalty>> penalties_;
    vector<shared_ptr<Mutation>> mutations_;

    int steps_number_;

    double current_total_penalty_ = 0;
    const int history_period_ = 1000;
};
