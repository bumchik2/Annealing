#pragma once

#include <vector>
#include "distance_matrix.h"

using std::vector;

class Problem {
public:
    Problem(const DistanceMatrix& distance_matrix,
            const vector<int>& required_vertex):
        distance_matrix(distance_matrix), required_vertex_(required_vertex) {
        init_();
    }

    const DistanceMatrix& distance_matrix;

    // 3 lines of vertex corresponding to permutations
    vector<vector<int>> answer;

private:
    // vertex numbers that have to present in each of the lines
    // the rest vertex have to present in at least one line
    const vector<int>& required_vertex_;

    void init_();
};
