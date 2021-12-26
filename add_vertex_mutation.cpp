#include "add_vertex_mutation.h"
#include "utils.h"

#include <utility>
#include <stdexcept>
#include <iostream>

using std::swap;
using std::runtime_error;
using std::cout;
using std::endl;
using std::min;
using std::max;

double AddVertexMutation::get_delta_distance_(const Problem& problem, int line_number, unsigned pos, int v) {
    int delta_distance = 0;
    if (pos != 0) {
        delta_distance += problem.distance_matrix.get(problem.answer[line_number][pos - 1], v);
    }
    if (pos != problem.answer[line_number].size()) {
        delta_distance += problem.distance_matrix.get(v, problem.answer[line_number][pos]);
    }
    if (pos != 0 && pos != problem.answer[line_number].size()) {
        delta_distance -= problem.distance_matrix.get(problem.answer[line_number][pos - 1],
                                              problem.answer[line_number][pos]);
    }
    return delta_distance;
}

double AddVertexMutation::get_delta_penalty(
        const Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(0, problem.answer[line_number].size() + 1);
    int v = randint(0, problem.distance_matrix.size());

    double delta_distance = get_delta_distance_(problem, line_number, pos, v);
    double delta_distance_penalty = delta_distance * penalty_sizes_["Distance penalty"];

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

    double delta_difference_penalty = ((max2 - min2) - (max1 - min1)) * penalty_sizes_["Difference penalty"];
    double delta_maximum_penalty = (max2 - max1) * penalty_sizes_["Maximum penalty"];
    return delta_distance_penalty + delta_difference_penalty + delta_maximum_penalty;
}

void AddVertexMutation::mutate(
        Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(0, problem.answer[line_number].size() + 1);
    int v = randint(0, problem.distance_matrix.size());

    double delta_distance = get_delta_distance_(problem, line_number, pos, v);
    problem.update_total_distance(line_number, delta_distance);

    const auto iterator_pos = problem.answer[line_number].begin() + pos;
    problem.answer[line_number].insert(iterator_pos, v);
}
