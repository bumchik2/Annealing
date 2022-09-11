#include "remove_vertex_mutation.h"
#include "utils.h"

#include <utility>
#include <stdexcept>
#include <iostream>

using std::swap;
using std::runtime_error;
using std::cout;
using std::endl;

double RemoveVertexMutation::get_delta_penalty(const Problem& problem, int random_seed) {
    return 0;
    /*srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(0, problem.answer[line_number].size());

    return 0; // TODO*/
}

void RemoveVertexMutation::mutate(Problem& problem, int random_seed) {
    /*srand(random_seed);

    int line_number = randint(0, problem.answer.size());
    unsigned pos = randint(0, problem.answer[line_number].size());

    const auto iterator_pos = problem.answer[line_number].begin() + pos;
    problem.answer[line_number].erase(iterator_pos);*/
}
