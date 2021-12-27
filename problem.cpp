#include "problem.h"

#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

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

    total_distances.resize(answer.size());
    for (unsigned i = 0; i < total_distances.size(); ++i) {
        for (unsigned j = 0; j + 1 < answer[i].size(); ++j) {
            total_distances[i] += distance_matrix.get(answer[i][j], answer[i][j + 1]);
        }
    }

    for (unsigned i = 0; i < required_vertex_.size(); ++i) {
        required_vertex_set_.insert(required_vertex_[i]);
    }
    assert(required_vertex_.size() == required_vertex_set_.size());
}

void Problem::update_total_distance(int line_number, int delta_total_distance) {
    total_distances[line_number] += delta_total_distance;
}

bool Problem::is_required_vertex(int v) const {
    return required_vertex_set_.find(v) != required_vertex_set_.end();
}
