#include "problem.h"

#include <iostream>
#include <cassert>
#include <unordered_set>
#include <vector>

using std::vector;
using std::unordered_set;
using std::cout;
using std::endl;

vector<int> make_answer_from_part(const DistanceMatrix& distance_matrix, unordered_set<int>& part) {
    int v = *part.begin();
    vector<int> answer = {v};
    part.erase(*part.begin());

    while(!part.empty()) {
        int min_distance = 1000000000;
        int imin_distance = -1;
        for (const auto& item: part) {
            int distance = distance_matrix.get(answer[answer.size() - 1], item);
            if (distance < min_distance) {
                min_distance = distance;
                imin_distance = item;
            }
        }
        answer.push_back(imin_distance);
        part.erase(imin_distance);
    }
    return answer;
}

void Problem::initialize_total_distances() {
    total_distances = vector<int>();
    total_distances.resize(answer.size());
    for (unsigned i = 0; i < total_distances.size(); ++i) {
        for (unsigned j = 0; j + 1 < answer[i].size(); ++j) {
            total_distances[i] += distance_matrix.get(answer[i][j], answer[i][j + 1]);
        }
    }
}

void Problem::init_() {
    // initialize some default answer

    answer.resize(3);

    // initialize required_vertex_set_
    for (unsigned i = 0; i < required_vertex_.size(); ++i) {
        required_vertex_set_.insert(required_vertex_[i]);
    }
    assert(required_vertex_.size() == required_vertex_set_.size());

    // split vertex into 3 ~equal parts, arrange them greedily
    vector<unordered_set<int>> parts(answer.size());
    for (unsigned v = 0; v < distance_matrix.size(); ++v) {
        if (is_required_vertex(v)) {
            for (unsigned j = 0; j < parts.size(); ++j) {
                parts[j].insert(v);
            }
        } else {
            // parts[0].insert(v);
            parts[v % parts.size()].insert(v);
        }
    }

    int cnt = 0;
    for (auto& part: parts) {
        answer[cnt++] = make_answer_from_part(distance_matrix, part);
    }

    // initialize total distances
    initialize_total_distances();
}

void Problem::update_total_distance(int line_number, int delta_total_distance) {
    total_distances[line_number] += delta_total_distance;
}

bool Problem::is_required_vertex(int v) const {
    return required_vertex_set_.find(v) != required_vertex_set_.end();
}
