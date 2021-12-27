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
        srand(i);
        update_temperature_(i);
        current_total_penalty_ = try_mutate_();
        if ((i != 0 && i % history_period_ == 0) || i + 1 == steps_number_) {
            penalty_history.push_back(current_total_penalty_);
        }
        if ((swap_tails_period_ > 0) && (i % swap_tails_period_ == 0)) {
            swap_tails_();
        }
    }
}

void Annealing::swap_tails_() {
    // PROBLEM with this idea: required vertex go brr...

    int line_number1 = randint(0, problem_.answer.size());
    int line_number2 = (randint(1, problem_.answer.size()) + line_number1) % problem_.answer.size();
    int delta = 5;
    unsigned pos1 = unsigned(randint(-delta, delta) + 0.5 * problem_.answer[line_number1].size());
    unsigned pos2 = unsigned(randint(-delta, delta) + 0.5 * problem_.answer[line_number2].size());

    // swap tails of line1 and line2, starting with pos1 and pos2
    vector<int> tail1;
    vector<int> tail2;
    for (unsigned i = pos1; i < problem_.answer[line_number1].size(); ++i) {
        tail1.push_back(problem_.answer[line_number1][i]);
    }
    for (unsigned i = pos1; i < problem_.answer[line_number1].size(); ++i) {
        tail1.pop_back();
    }
    for (unsigned i = pos2; i < problem_.answer[line_number2].size(); ++i) {
        tail2.push_back(problem_.answer[line_number2][i]);
    }
    for (unsigned i = pos2; i < problem_.answer[line_number2].size(); ++i) {
        tail2.pop_back();
    }
    for (unsigned i = 0; i < tail1.size(); ++i) {
        problem_.answer[line_number2].push_back(tail1[i]);
    }
    for (unsigned i = 0; i < tail2.size(); ++i) {
        problem_.answer[line_number1].push_back(tail2[i]);
    }

    // update total_distances
    problem_.initialize_total_distances();
    // update penalties correctly
    current_total_penalty_ = 0;
    for (const auto& penalty: penalties_) {
        current_total_penalty_ += penalty->get_penalty(problem_);
    }
}

void Annealing::update_temperature_(int step_number) {
    temperature_ = initial_temperature_ * (1.0 - static_cast<double>(step_number) / steps_number_);
}

double Annealing::try_mutate_() {
    // returns new total penalty after the mutation did or did not happen

    int mutation_number = -1;
    double mutation_lucky = random_double();
    double total = 0;
    for (unsigned i = 0; i < mutations_.size(); ++i) {
        total += mutation_probabilities_[i];
        if (mutation_lucky < total) {
            mutation_number = i;
            break;
        }
    }

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
