#include "problem.h"

void Problem::init_() {
    // initialize some default answer

    answer.resize(3);
    for (unsigned j = 0; j < distance_matrix.size(); ++j) {
        answer[0].push_back(j);
    }

    for (unsigned i = 1; i < answer.size(); ++i) {
        for (unsigned j = 0; j < required_vertex_.size(); ++j) {
            answer[i].push_back(required_vertex_[j]);
        }
    }
}
