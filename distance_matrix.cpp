#include "distance_matrix.h"
#include "utils.h"
#include "csv_utils.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <cmath>

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;

DistanceMatrix generate_matrix(int n, double max_distance, bool symmetric) {
    vector<vector<double>> distances;

    distances.resize(n);
    for (int i = 0; i < n; ++i) {
        distances[i].resize(n);
        for (int j = 0; j < n; ++j) {
            distances[i][j] = randint(1, max_distance + 1);
        }
    }

    if (symmetric) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i > j) {
                    distances[i][j] = distances[j][i];
                }
            }
        }
    }

    return DistanceMatrix(distances);
}

DistanceMatrix read_matrix(int n, const string& filename) {
    vector<vector<double>> distances(n);

    int rows_read = 0;
    ifstream file(filename);
    for (auto& row: CSVRange(file)) {
        rows_read += 1;
        if (rows_read == 1) {
            continue;
        }
        for (int i = 1; i <= n; ++i) {
            distances[rows_read - 2].push_back(string_to_int(row[i]));
        }
    }
    assert(rows_read == n + 1);

    return DistanceMatrix(distances);
}

pair<double, double> get_rectangle_coords(int x, int y, double noise) {
    return make_pair<double, double>(x + noise * (random_double() * 2 - 1),
                                     y + noise * (random_double() * 2 - 1));
}

vector<pair<double, double>> make_rectangle(int n, int m, double noise) {
    vector<pair<double, double>> coords;
    for (int i = 0; i < n * m; ++i) {
            pair<double, double> coords_i = get_rectangle_coords(i / m, i % m, noise);
            coords.push_back(coords_i);
    }
    return coords;
}

DistanceMatrix get_euclidean_distance_matrix(const vector<pair<double, double>>& coords) {
    vector<vector<double>> distances(coords.size(), vector<double>(coords.size()));
    for (unsigned i = 0; i < coords.size(); ++i) {
        for (unsigned j = 0; j < coords.size(); ++j) {
            const pair<double, double>& coords_i = coords[i];
            const pair<double, double>& coords_j = coords[j];
            distances[i][j] = sqrt(
                   (coords_i.first - coords_j.first) * (coords_i.first - coords_j.first) +
                   (coords_i.second - coords_j.second) * (coords_i.second - coords_j.second));
        }
    }
    return DistanceMatrix(distances);
}
