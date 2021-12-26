#include "maximum_penalty.h"
#include "problem.h"

#include <algorithm>

using std::max;

double MaximumPenalty::get_penalty(const Problem& problem) {
    int maximum = -1000000000;

    for (unsigned i = 0; i < problem.answer.size(); ++i) {
        int total_distance = 0;
        for (unsigned j = 0; j < problem.answer[i].size() - 1; ++j) {
            int v1 = problem.answer[i][j];
            int v2 = problem.answer[i][j + 1];
            total_distance += problem.distance_matrix.get(v1, v2);
        }
        maximum = max(maximum, total_distance);
    }
    return maximum * penalty_size;
}
