#include "transfer_mutation.h"
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

double TransferMutation::get_delta_penalty(
        const Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number1 = randint(0, problem.answer.size());
    int line_number2 = (line_number1 + randint(1, problem.answer.size())) % problem.answer.size();
    unsigned pos1 = randint(0, problem.answer[line_number1].size());
    unsigned pos2 = randint(0, problem.answer[line_number2].size() + 1);

    int v = problem.answer[line_number1][pos1];
    if (problem.is_required_vertex(v)) {
        // we do not want to transfer required vertex
        return 0;
    }

    vector<int> delta_distances = get_delta_distances_(problem, line_number1, line_number2, pos1, pos2);
    assert(delta_distances.size() == 2);
    vector<int> total_distances = problem.total_distances;
    int min1 = 1000000000;
    int max1 = -min1, min2 = min1, max2 = -min1;
    for (unsigned i = 0; i < total_distances.size(); ++i) {
        min1 = min(min1, total_distances[i]);
        max1 = max(max1, total_distances[i]);
    }
    total_distances[line_number1] += delta_distances[0];
    total_distances[line_number2] += delta_distances[1];
    for (unsigned i = 0; i < total_distances.size(); ++i) {
        min2 = min(min2, total_distances[i]);
        max2 = max(max2, total_distances[i]);
    }
    double delta_distance = delta_distances[0] + delta_distances[1];

    double delta_distance_penalty = delta_distance * penalty_sizes_["Distance penalty"];
    double delta_difference_penalty = ((max2 - min2) - (max1 - min1)) * penalty_sizes_["Difference penalty"];
    double delta_maximum_penalty = (max2 - max1) * penalty_sizes_["Maximum penalty"];
    return delta_distance_penalty + delta_difference_penalty + delta_maximum_penalty;
}

void TransferMutation::mutate(
        Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number1 = randint(0, problem.answer.size());
    int line_number2 = (line_number1 + randint(1, problem.answer.size())) % problem.answer.size();
    unsigned pos1 = randint(0, problem.answer[line_number1].size());
    unsigned pos2 = randint(0, problem.answer[line_number2].size() + 1);

    int v = problem.answer[line_number1][pos1];
    if (problem.is_required_vertex(v)) {
        // we do not want to transfer required vertex
        return;
    }

    vector<int> delta_distances = get_delta_distances_(problem, line_number1, line_number2, pos1, pos2);
    problem.update_total_distance(line_number1, delta_distances[0]);
    problem.update_total_distance(line_number2, delta_distances[1]);

    problem.answer[line_number1].erase(problem.answer[line_number1].begin() + pos1);
    const auto iterator_pos = problem.answer[line_number2].begin() + pos2;
    problem.answer[line_number2].insert(iterator_pos, v);
}

vector<int> TransferMutation::get_delta_distances_(const Problem& problem,
        int line_number1, int line_number2, unsigned pos1, unsigned pos2) {
    assert(line_number1 != line_number2);
    // line_number1: ... - a - v - b - ...
    // line_number2: ... - c - < insert position > - d - ...
    int v = problem.answer[line_number1][pos1];

    int delta_distance1 = 0;
    if (pos1 != 0) {
        int a = problem.answer[line_number1][pos1 - 1];
        delta_distance1 -= problem.distance_matrix.get(a, v);
    }
    if (pos1 + 1 != problem.answer[line_number1].size()) {
        int b = problem.answer[line_number1][pos1 + 1];
        delta_distance1 -= problem.distance_matrix.get(v, b);
    }
    if ((pos1 != 0) && (pos1 + 1 != problem.answer[line_number1].size())) {
        int a = problem.answer[line_number1][pos1 - 1];
        int b = problem.answer[line_number1][pos1 + 1];
        delta_distance1 += problem.distance_matrix.get(a, b);
    }

    int delta_distance2 = 0;
    if (pos2 != 0) {
        int c = problem.answer[line_number2][pos2 - 1];
        delta_distance2 += problem.distance_matrix.get(c, v);
    }
    if (pos2 != problem.answer[line_number2].size()) {
        int d = problem.answer[line_number2][pos2];
        delta_distance2 += problem.distance_matrix.get(v, d);
    }
    if ((pos2 != 0) && (pos2 != problem.answer[line_number2].size())) {
        int c = problem.answer[line_number2][pos2 - 1];
        int d = problem.answer[line_number2][pos2];
        delta_distance2 -= problem.distance_matrix.get(c, d);
    }

    return {delta_distance1, delta_distance2};
}
