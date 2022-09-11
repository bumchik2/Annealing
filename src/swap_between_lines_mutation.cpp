#include "swap_between_lines_mutation.h"
#include "utils.h"

#include <utility>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <vector>

using std::vector;
using std::swap;
using std::runtime_error;
using std::cout;
using std::endl;
using std::min;
using std::max;

vector<int> SwapBetweenLinesMutation::choose_mutation_parameters_(const Problem& problem) {
    int line_number1 = -1, line_number2 = -1, pos1 = -1, pos2 = -1;
    while (line_number1 == -1 || problem.answer[line_number1].empty() || problem.answer[line_number2].empty()) {
        line_number1 = randint(0, problem.answer.size());
        line_number2 = (line_number1 + randint(1, problem.answer.size())) % problem.answer.size();
        if (problem.answer[line_number1].empty() || problem.answer[line_number2].empty()) {
            continue;
        }
        pos1 = randint(0, problem.answer[line_number1].size());
        pos2 = randint(0, problem.answer[line_number2].size());
    }
    
    return std::vector<int>{line_number1, line_number2, pos1, pos2};
}

double SwapBetweenLinesMutation::get_delta_penalty(
        const Problem& problem, int random_seed) {
    srand(random_seed);

    vector<int> parameters = choose_mutation_parameters_(problem);
    int line_number1 = parameters[0];
    int line_number2 = parameters[1];
    unsigned pos1 = parameters[2];
    unsigned pos2 = parameters[3];
    
    int v1 = problem.answer[line_number1][pos1];
    int v2 = problem.answer[line_number2][pos2];
    if ((problem.is_required_vertex(v1)) || (problem.is_required_vertex(v2))) {
        // we need two different lines to mutate and do not want to swap required vertex
        return 0;
    }

    vector<double> delta_distances = get_delta_distances_(problem, line_number1, line_number2, pos1, pos2);
    double delta_distance = 0;

    for (unsigned i = 0; i < delta_distances.size(); ++i) {
        delta_distance += delta_distances[i];
    }

    vector<double> total_distances = problem.total_distances;
    double min1 = 1000000000;
    double max1 = -min1, min2 = min1, max2 = -min1;
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

    double delta_distance_penalty = delta_distance * penalty_sizes_["Distance penalty"];
    double delta_difference_penalty = ((max2 - min2) - (max1 - min1)) * penalty_sizes_["Difference penalty"];
    double delta_maximum_penalty = (max2 - max1) * penalty_sizes_["Maximum penalty"];
    // swapping two vertex between routes does not change max_locations - min_locations
    double delta_difference_locations_penalty = 0;
    return delta_distance_penalty + delta_difference_penalty + delta_maximum_penalty + delta_difference_locations_penalty;
}

void SwapBetweenLinesMutation::mutate(
        Problem& problem, int random_seed) {
    srand(random_seed);

    vector<int> parameters = choose_mutation_parameters_(problem);
    int line_number1 = parameters[0];
    int line_number2 = parameters[1];
    unsigned pos1 = parameters[2];
    unsigned pos2 = parameters[3];

    int v1 = problem.answer[line_number1][pos1];
    int v2 = problem.answer[line_number2][pos2];
    if ((problem.is_required_vertex(v1)) || (problem.is_required_vertex(v2))) {
        // we need two different lines to mutate and do not want to swap required vertex
        return;
    }

    vector<double> delta_distances = get_delta_distances_(problem, line_number1, line_number2, pos1, pos2);
    problem.update_total_distance(line_number1, delta_distances[0]);
    problem.update_total_distance(line_number2, delta_distances[1]);
    swap(problem.answer[line_number1][pos1], problem.answer[line_number2][pos2]);
}

vector<double> SwapBetweenLinesMutation::get_delta_distances_(const Problem& problem,
        int line_number1, int line_number2, unsigned pos1, unsigned pos2) {
    assert(line_number1 != line_number2);

    // line_number1: - a - v1 - b -
    // line_number2: - c - v2 - d -
    int v1 = problem.answer[line_number1][pos1];
    int v2 = problem.answer[line_number2][pos2];

    double delta_distance1 = 0;
    if (pos1 != 0) {
        int a = problem.answer[line_number1][pos1 - 1];
        delta_distance1 += (problem.distance_matrix.get(a, v2) - problem.distance_matrix.get(a, v1));
    }
    if (pos1 + 1 != problem.answer[line_number1].size()) {
        int b = problem.answer[line_number1][pos1 + 1];
        delta_distance1 += (problem.distance_matrix.get(v2, b) - problem.distance_matrix.get(v1, b));
    }

    double delta_distance2 = 0;
    if (pos2 != 0) {
        int c = problem.answer[line_number2][pos2 - 1];
        delta_distance2 += (problem.distance_matrix.get(c, v1) - problem.distance_matrix.get(c, v2));
    }
    if (pos2 + 1 != problem.answer[line_number2].size()) {
        int d = problem.answer[line_number2][pos2 + 1];
        delta_distance2 += (problem.distance_matrix.get(v1, d) - problem.distance_matrix.get(v2, d));
    }

    return {delta_distance1, delta_distance2};
}
