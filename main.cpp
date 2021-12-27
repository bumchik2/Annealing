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
#include "swap_head_tail_mutation.h"
#include "csv_utils.h"

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
using std::ifstream;

int main() {
    // TODO: transfer within the same line
    // TODO: smarter problem initializing
    // TODO: swap halves of 2 random lines once in several iterations in order to avoid stagnation

    const int n = 5040; // number of vertex
    const DistanceMatrix distance_matrix = read_matrix(n, "distance_matrix.csv");
    const int required_vertex_number = n / 42;
    vector<int> required_vertex;
    for (int i = 0; i < required_vertex_number; ++i) {
        required_vertex.push_back(i);
    }

    Problem problem(distance_matrix, required_vertex);

    const double temperature = 25;
    const int steps_number = 1000 * 1000000;

    cout << "total distances: " <<  problem.total_distances << endl;
    const vector<shared_ptr<Penalty>> penalties = {
        make_shared<DistancePenalty>(10),
        make_shared<MaximumPenalty>(30),
        make_shared<DifferencePenalty>(2)
    };
    const vector<shared_ptr<Mutation>> mutations = {
        make_shared<SwapVertexMutation>(penalties),
        make_shared<TransferMutation>(penalties),
        make_shared<SwapBetweenLinesMutation>(penalties),
        make_shared<SwapHeadTailMutation>(penalties)
        // make_shared<AddVertexMutation>(penalties)
    };
    const vector<double> mutation_probabilities = {
        0.33,
        0.33,
        0.33,
        0.01
    };
    assert(mutations.size() == mutation_probabilities.size());

    cout << "Initial penalties:" << endl;
    print_penalties(penalties, problem);
    cout << endl;

    Annealing annealing(problem, temperature, penalties, mutations, mutation_probabilities, steps_number);
    annealing.work();

    ofstream out_penalties("results/total_penalties.txt");
    out_penalties << annealing.penalty_history;
    out_penalties.close();

    ofstream out_answer("results/answer.txt");
    for (unsigned i = 0; i < problem.answer.size(); ++i) {
        out_answer << problem.answer[i] << endl;
    }
    out_answer.close();

    cout << "Expected total penalty: " << annealing.penalty_history[annealing.penalty_history.size() - 1] << endl;
    cout << "Result penalties:" << endl;
    print_penalties(penalties, problem);
    cout << endl;

    cout << "Vertex distribution in the answer:" << endl;
    for (unsigned i = 0; i < problem.answer.size(); ++i) {
        cout << problem.answer[i].size() << endl;
    }
    return 0;
}
