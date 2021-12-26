#include "distance_matrix.h"
#include "utils.h"

#include <vector>

using std::vector;

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
