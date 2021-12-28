#pragma once

#include <vector>
#include <string>
#include <utility>

using std::vector;
using std::string;
using std::pair;

class DistanceMatrix {
public:
    DistanceMatrix(const vector<vector<double>>& distances):
        distances_(distances) {}

    double get(int i, int j) const {
        return distances_[i][j];
    }

    unsigned size() const {
        return distances_.size();
    }

private:
    vector<vector<double>> distances_;
};

DistanceMatrix generate_matrix(int n, double max_distance=7, bool symmetric=false);
DistanceMatrix read_matrix(int n, const string& filename);

vector<pair<double, double>> make_rectangle(int n, int m, double noise = 0.5);
DistanceMatrix get_euclidean_distance_matrix(const vector<pair<double, double>>& coords);
