#include "annealing.h"
#include "mutation.h"
#include "distance_matrix.h"
#include "utils.h"
#include "distance_penalty.h"
#include "swap_vertex_mutation.h"
#include "add_vertex_mutation.h"
#include "maximum_penalty.h"
#include "difference_penalty.h"
#include "swap_between_lines_mutation.h"
#include "transfer_mutation.h"

#include <iostream>
#include <vector>
#include <memory>
#include <time.h>
#include <fstream>

using std::vector;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::ofstream;


int main() {
    const int n = 5000; // number of vertex
    const DistanceMatrix distance_matrix = generate_matrix(n);
    const int required_vertex_number = n / 5;
    vector<int> required_vertex;
    for (int i = 0; i < required_vertex_number; ++i) {
        required_vertex.push_back(i);
    }

    Problem problem(distance_matrix, required_vertex);

    const double temperature = 100;
    const int steps_number = 1 * 1000000;

    cout << "total distances: " <<  problem.total_distances << endl;
    const vector<shared_ptr<Penalty>> penalties = {
        make_shared<DistancePenalty>(10),
        make_shared<MaximumPenalty>(30),
        make_shared<DifferencePenalty>(2)
    };
    const vector<shared_ptr<Mutation>> mutations = {
        make_shared<SwapVertexMutation>(penalties),
        make_shared<TransferMutation>(penalties),
        make_shared<SwapBetweenLinesMutation>(penalties)
        // make_shared<AddVertexMutation>(penalties)
    };

    cout << "Initial penalties:" << endl;
    print_penalties(penalties, problem);
    cout << endl;

    Annealing annealing(problem, temperature, penalties, mutations, steps_number);
    annealing.work();

    ofstream out("results/total_penalties.txt");
    out << annealing.penalty_history;
    out.close();

    cout << "Expected total penalty: " << annealing.penalty_history[annealing.penalty_history.size() - 1] << endl;
    cout << "Result penalties:" << endl;
    print_penalties(penalties, problem);
    cout << endl;
    return 0;
}
