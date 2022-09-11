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
            const vector<shared_ptr<Mutation>>& mutations, const vector<double>& mutation_probabilities, int steps_number):
            problem_(problem), temperature_(temperature), initial_temperature_(temperature_), penalties_(penalties),
            mutations_(mutations), mutation_probabilities_(mutation_probabilities), steps_number_(steps_number) {
        init_();
    }

    void make_step(int step_number);
    void work();

    vector<double> penalty_history;

private:
    void init_();
    void swap_tails_();
    void update_temperature_(int step_number);
    int choose_mutation_number_();
    double try_mutate_();

    Problem& problem_;
    double temperature_;
    const double initial_temperature_;

    vector<shared_ptr<Penalty>> penalties_;
    vector<shared_ptr<Mutation>> mutations_;
    vector<double> mutation_probabilities_;

    int steps_number_;

    double current_total_penalty_ = 0;
    const int history_period_ = 1000;
    const int swap_tails_period_ = -1;
};
