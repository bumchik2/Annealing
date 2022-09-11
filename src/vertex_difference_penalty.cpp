#include "vertex_difference_penalty.h"
#include "problem.h"

#include <algorithm>
#include <iostream>

using std::max;
using std::min;
using std::cout;
using std::endl;

double VertexDifferencePenalty::get_penalty(const Problem& problem) {
    double maximum = -1000000000;
    double minimum = -maximum;

    for (unsigned i = 0; i < problem.answer.size(); ++i) {
    	minimum = min(minimum, static_cast<double>(problem.answer[i].size()));
    	maximum = max(maximum, static_cast<double>(problem.answer[i].size()));
    }
    return (maximum - minimum) * penalty_size;
}
