#include "distance_matrix.h"
#include "utils.h"
#include "csv_utils.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

using std::runtime_error;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;

DistanceMatrix generate_matrix(int n, int max_distance, bool symmetric) {
    vector<vector<int>> distances;

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
    vector<vector<int>> distances(n);

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
