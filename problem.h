#pragma once

#include <vector>
#include <unordered_set>

#include "distance_matrix.h"

using std::vector;
using std::unordered_set;

class Problem {
public:
    Problem(const DistanceMatrix& distance_matrix,
            const vector<int>& required_vertex, int problem_size = 3):
        distance_matrix(distance_matrix), required_vertex_(required_vertex), problem_size_(problem_size) {
        init_();
    }

    bool is_required_vertex(int v) const;

    void initialize_total_distances();
    void update_total_distance(int line_number, double delta_total_distance);

    const DistanceMatrix& distance_matrix;

    // 3 lines of vertex corresponding to permutations
    vector<vector<int>> answer;

    // total distances for each line
    vector<double> total_distances;

private:
    // vertex numbers that have to present in each of the lines
    // the rest vertex have to present in at least one line
    const vector<int>& required_vertex_;
    unordered_set<int> required_vertex_set_;
    int problem_size_ = 3;

    void init_();
};
