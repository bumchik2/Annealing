#include "swap_head_tail_mutation.h"
#include "utils.h"

#include <utility>
#include <stdexcept>
#include <iostream>
#include <cassert>

using std::swap;
using std::runtime_error;
using std::cout;
using std::endl;
using std::min;
using std::max;

double SwapHeadTailMutation::get_delta_penalty(const Problem& problem, int random_seed) {
    srand(random_seed);
    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(1, problem.answer.size());

    int delta_distance = get_delta_distance_(problem, line_number, pos);

    vector<int> total_distances = problem.total_distances;
    int min1 = 1000000000;
    int max1 = -min1, min2 = min1, max2 = -min1;
    for (unsigned i = 0; i < total_distances.size(); ++i) {
        min1 = min(min1, total_distances[i]);
        max1 = max(max1, total_distances[i]);
    }
    total_distances[line_number] += delta_distance;
    for (unsigned i = 0; i < total_distances.size(); ++i) {
        min2 = min(min2, total_distances[i]);
        max2 = max(max2, total_distances[i]);
    }

    double delta_distance_penalty = delta_distance * penalty_sizes_["Distance penalty"];
    double delta_difference_penalty = ((max2 - min2) - (max1 - min1)) * penalty_sizes_["Difference penalty"];
    double delta_maximum_penalty = (max2 - max1) * penalty_sizes_["Maximum penalty"];
    return delta_distance_penalty + delta_difference_penalty + delta_maximum_penalty;
}

void SwapHeadTailMutation::mutate(Problem& problem, int random_seed) {
    srand(random_seed);
    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(1, problem.answer.size());

    int delta_distance = get_delta_distance_(problem, line_number, pos);
    problem.update_total_distance(line_number, delta_distance);

    vector<int> new_line;
    for (unsigned i = pos; i < problem.answer[line_number].size(); ++i) {
        new_line.push_back(problem.answer[line_number][i]);
    }
    for (unsigned i = 0; i < pos; ++i) {
        new_line.push_back(problem.answer[line_number][i]);
    }
    problem.answer[line_number] = std::move(new_line);
}

int SwapHeadTailMutation::get_delta_distance_(const Problem& problem, int line_number, unsigned pos) {
    // ... - a - b - ...
    // index of b is pos
    int a = problem.answer[line_number][pos - 1];
    int b = problem.answer[line_number][pos];
    int first = problem.answer[line_number][0];
    int last = problem.answer[line_number][problem.answer[line_number].size() - 1];

    int delta_distance = 0;
    delta_distance += (problem.distance_matrix.get(last, first) - problem.distance_matrix.get(a, b));
    return delta_distance;
}
