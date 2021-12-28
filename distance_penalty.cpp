#include "distance_penalty.h"
#include "problem.h"

double DistancePenalty::get_penalty(const Problem& problem) {
    double result = 0;
    for (unsigned i = 0; i < problem.answer.size(); ++i) {
        for (unsigned j = 0; j < problem.answer[i].size() - 1; ++j) {
            int v1 = problem.answer[i][j];
            int v2 = problem.answer[i][j + 1];
            result += problem.distance_matrix.get(v1, v2);
        }
    }
    return result * penalty_size;
}
