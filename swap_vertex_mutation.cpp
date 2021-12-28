#include "swap_vertex_mutation.h"
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

double SwapVertexMutation::get_delta_distance_(
        const Problem& problem, int line_number, unsigned pos1, unsigned pos2) {
    double result = 0;

    int v1 = problem.answer[line_number][pos1];
    int v2 = problem.answer[line_number][pos2];

    // ... - a - v1 - b - ... - c - v2 - d - ... transforms into
    // ... - a - v2 - b - ... - c - v1 - d - ...
    if (pos1 != 0) {
        int a = problem.answer[line_number][pos1 - 1];
        result += (problem.distance_matrix.get(a, v2) - problem.distance_matrix.get(a, v1));
    }
    if (pos2 + 1 != problem.answer[line_number].size()) {
        int d = problem.answer[line_number][pos2 + 1];
        result += (problem.distance_matrix.get(v1, d) - problem.distance_matrix.get(v2, d));
    }
    if (pos2 != pos1 + 1) {
        int b = problem.answer[line_number][pos1 + 1];
        int c = problem.answer[line_number][pos2 - 1];
        result += (problem.distance_matrix.get(v2, b) - problem.distance_matrix.get(v1, b));
        result += (problem.distance_matrix.get(c, v1) - problem.distance_matrix.get(c, v2));
    } else {
        result += (problem.distance_matrix.get(v2, v1) - problem.distance_matrix.get(v1, v2));
    }

    return result;
}

double SwapVertexMutation::get_delta_penalty(
        const Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    int pos1 = randint(0, problem.answer[line_number].size());
    int pos2 = randint(0, problem.answer[line_number].size());

    if (pos1 > pos2) {
        swap(pos1, pos2);
    }

    if (pos1 == pos2) {
        return 0;
    }

    double delta_distance = get_delta_distance_(problem, line_number, pos1, pos2);
    double delta_distance_penalty = delta_distance * penalty_sizes_["Distance penalty"];

    vector<double> total_distances = problem.total_distances;
    double min1 = 1000000000;
    double max1 = -min1, min2 = min1, max2 = -min1;
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

void SwapVertexMutation::mutate(Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    int pos1 = randint(0, problem.answer[line_number].size());
    int pos2 = randint(0, problem.answer[line_number].size());

    if (pos1 == pos2) {
        return;
    }

    if (pos1 > pos2) {
        swap(pos1, pos2);
    }

    double delta_distance = get_delta_distance_(problem, line_number, pos1, pos2);
    problem.update_total_distance(line_number, delta_distance);

    swap(problem.answer[line_number][pos1], problem.answer[line_number][pos2]);
}
