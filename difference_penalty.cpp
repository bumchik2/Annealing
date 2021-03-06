#include "difference_penalty.h"
#include "problem.h"

#include <algorithm>
#include <iostream>

using std::max;
using std::min;
using std::cout;
using std::endl;

double DifferencePenalty::get_penalty(const Problem& problem) {
    double maximum = -1000000000;
    double minimum = -maximum;

    for (unsigned i = 0; i < problem.answer.size(); ++i) {
        double total_distance = 0;
        for (unsigned j = 0; j + 1 < problem.answer[i].size(); ++j) {
            int v1 = problem.answer[i][j];
            int v2 = problem.answer[i][j + 1];
            total_distance += problem.distance_matrix.get(v1, v2);
        }
        maximum = max(maximum, total_distance);
        minimum = min(minimum, total_distance);
    }
    return (maximum - minimum) * penalty_size;
}
