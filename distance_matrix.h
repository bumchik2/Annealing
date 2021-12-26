#pragma once

#include <vector>

using std::vector;

class DistanceMatrix {
public:
    DistanceMatrix(const vector<vector<int>>& distances):
        distances_(distances) {}

    int get(int i, int j) const {
        return distances_[i][j];
    }

    unsigned size() const {
        return distances_.size();
    }

private:
    vector<vector<int>> distances_;
};

DistanceMatrix generate_matrix(int n, int max_distance=7, bool symmetric=false);
