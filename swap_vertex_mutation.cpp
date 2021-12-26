#include "swap_vertex_mutation.h"
#include "utils.h"

#include <utility>
#include <stdexcept>
#include <iostream>

using std::swap;
using std::runtime_error;
using std::cout;
using std::endl;

double SwapVertexMutation::get_delta_penalty(
        const Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    unsigned pos1 = randint(0, problem.answer[line_number].size());
    unsigned pos2 = randint(0, problem.answer[line_number].size());

    if (pos1 > pos2) {
        swap(pos1, pos2);
    }

    if (pos1 == pos2) {
        return 0;
    }

    int v1 = problem.answer[line_number][pos1];
    int v2 = problem.answer[line_number][pos2];

    // ... - a - v1 - b - ... - c - v2 - d - ... transforms into
    // ... - a - v2 - b - ... - c - v1 - d - ...
    double result = 0;
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

    return result * penalty_sizes_["Distance penalty"];
}

void SwapVertexMutation::mutate(Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    int pos1 = randint(0, problem.answer[line_number].size());
    int pos2 = randint(0, problem.answer[line_number].size());

    swap(problem.answer[line_number][pos1], problem.answer[line_number][pos2]);
}
