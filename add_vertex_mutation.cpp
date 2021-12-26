#include "add_vertex_mutation.h"
#include "utils.h"

#include <utility>
#include <stdexcept>
#include <iostream>

using std::swap;
using std::runtime_error;
using std::cout;
using std::endl;

double AddVertexMutation::get_delta_penalty(
        const Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(0, problem.answer[line_number].size() + 1);
    int v = randint(0, problem.distance_matrix.size());

    double result = 0;
    if (pos != 0) {
        result += problem.distance_matrix.get(problem.answer[line_number][pos - 1], v);
    }
    if (pos != problem.answer[line_number].size()) {
        result += problem.distance_matrix.get(v, problem.answer[line_number][pos]);
    }
    if (pos != 0 && pos != problem.answer[line_number].size()) {
        result -= problem.distance_matrix.get(problem.answer[line_number][pos - 1],
                                              problem.answer[line_number][pos]);
    }
    return result * penalty_sizes_["Distance penalty"];
}

void AddVertexMutation::mutate(
        Problem& problem, int random_seed) {
    srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(0, problem.answer[line_number].size() + 1);
    int v = randint(0, problem.distance_matrix.size());

    // cout << "inserting " << v << " to " << line_number << endl;

    const auto iterator_pos = problem.answer[line_number].begin();
    problem.answer[line_number].insert(iterator_pos, v);
}
