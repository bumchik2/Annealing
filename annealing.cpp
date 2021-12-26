#include "annealing.h"
#include "utils.h"

#include <math.h>
#include <cassert>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::shared_ptr;

void Annealing::init_() {
    double total_penalty = 0;
    for (const auto& penalty: penalties_) {
        total_penalty += penalty->get_penalty(problem_);
    }
    penalty_history.push_back(total_penalty);
    current_total_penalty_ = total_penalty;
}

void Annealing::work() {
    for (int i = 0; i < steps_number_; ++i) {
        update_temperature_(i);
        current_total_penalty_ = try_mutate_();
        if ((i != 0 && i % history_period_ == 0) || i + 1 == steps_number_) {
            penalty_history.push_back(current_total_penalty_);
        }
    }
}

void Annealing::update_temperature_(int step_number) {
    temperature_ = initial_temperature_ * (1.0 - static_cast<double>(step_number) / steps_number_);
}

double Annealing::try_mutate_() {
    // returns new total penalty after the mutation did or did not happen

    // TODO: specific probability for each mutation
    int mutation_number = randint(0, mutations_.size());
    shared_ptr<Mutation>& mutation = mutations_[mutation_number];
    int mutation_seed = rand();
    double delta_penalty = mutation->get_delta_penalty(problem_, mutation_seed);

    double mutation_probability = pow(2.71, -delta_penalty / temperature_);
    double lucky = random_double();
    double new_total_penalty = current_total_penalty_;
    if (lucky < mutation_probability) {
        mutation->mutate(problem_, mutation_seed);
        new_total_penalty += delta_penalty;
    }

    return new_total_penalty;
}
