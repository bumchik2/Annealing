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
#include <utility>

using std::vector;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::ofstream;
using std::ifstream;
using std::pair;

void solve_problem(Problem& problem, const double temperature, const int steps_number,
            const string& penalties_filename = "", const string& answer_filename = "");

void solve_problem() {
    const int n = 5040; // number of vertex
    const DistanceMatrix distance_matrix = read_matrix(n, "distance_matrix.csv");
    const int required_vertex_number = n / 42;
    vector<int> required_vertex;
    for (int i = 0; i < required_vertex_number; ++i) {
        required_vertex.push_back(i);
    }

    Problem problem(distance_matrix, required_vertex);

    const double temperature = 25;
    const int steps_number = 1 * 1000000;

    solve_problem(problem, temperature, steps_number,
                  "results/total_penalties.txt", "results/answer.txt");
}

void save_answer(const string& filename, const Problem& problem) {
    ofstream out_answer(filename);
    for (unsigned i = 0; i < problem.answer.size(); ++i) {
        out_answer << problem.answer[i] << endl;
    }
    out_answer.close();
}

vector<shared_ptr<Penalty>> get_penalties() {
    return {
        make_shared<DistancePenalty>(10),
        make_shared<MaximumPenalty>(30),
        make_shared<DifferencePenalty>(2)
    };
}

vector<shared_ptr<Mutation>> get_mutations(const vector<shared_ptr<Penalty>>& penalties) {
    return {
        make_shared<SwapVertexMutation>(penalties),
        make_shared<TransferMutation>(penalties),
        make_shared<SwapBetweenLinesMutation>(penalties),
        make_shared<SwapHeadTailMutation>(penalties)
    };
}

vector<double> get_mutation_probabilities() {
    return {
        0.33,
        0.33,
        0.33,
        0.01
    };
}

void print_initial_penalties_info(const vector<shared_ptr<Penalty>>& penalties, const Problem& problem) {
    cout << "Initial penalties:" << endl;
    print_penalties(penalties, problem);
    cout << endl;
}

void print_final_penalty_info(const Annealing& annealing, const Problem& problem,
            const vector<shared_ptr<Penalty>>& penalties) {
    cout << "Expected total penalty: " << annealing.penalty_history[annealing.penalty_history.size() - 1] << endl;
    cout << "Result penalties:" << endl;
    print_penalties(penalties, problem);
    cout << endl;

    cout << "Vertex distribution in the answer:" << endl;
    for (unsigned i = 0; i < problem.answer.size(); ++i) {
        cout << problem.answer[i].size() << endl;
    }
}

void solve_problem(Problem& problem, const double temperature, const int steps_number,
            const string& penalties_filename, const string& answer_filename) {
    cout << "total distances: " <<  problem.total_distances << endl;
    const vector<shared_ptr<Penalty>> penalties = get_penalties();
    const vector<shared_ptr<Mutation>> mutations = get_mutations(penalties);
    const vector<double> mutation_probabilities = get_mutation_probabilities();
    assert(mutations.size() == mutation_probabilities.size());

    print_initial_penalties_info(penalties, problem);

    Annealing annealing(problem, temperature, penalties, mutations, mutation_probabilities, steps_number);
    annealing.work();

    if (penalties_filename != "") {
        ofstream out_penalties(penalties_filename);
        out_penalties << annealing.penalty_history;
        out_penalties.close();
    }

    if (answer_filename != "") {
        save_answer(answer_filename, problem);
    }

    print_final_penalty_info(annealing, problem, penalties);
}

void make_rectangle_test(int n, int m, int routes_number) {
    const vector<pair<double, double>> coords = make_rectangle(n, m);

    ofstream out_coords("test_results/coords.txt");
    for (unsigned i = 0; i < coords.size(); ++i) {
        out_coords << coords[i].first << " " << coords[i].second << endl;
    }
    out_coords.close();

    DistanceMatrix distance_matrix = get_euclidean_distance_matrix(coords);
    Problem problem(distance_matrix, {}, routes_number);
    save_answer("test_results/initial_answer.txt", problem);

    const vector<shared_ptr<Penalty>> penalties = get_penalties();
    const vector<shared_ptr<Mutation>> mutations = get_mutations(penalties);
    const vector<double> mutation_probabilities = get_mutation_probabilities();
    assert(mutations.size() == mutation_probabilities.size());

    print_initial_penalties_info(penalties, problem);

    const double temperature = 25;
    const int steps_number = 1000 * 1000000;
    Annealing annealing(problem, temperature, penalties, mutations, mutation_probabilities, steps_number);

    const int checkpoint_period = steps_number / 100;
    int checkpoint_number = 0;
    for (int i = 0; i < steps_number; ++i) {
        annealing.make_step(i);
        if (i % checkpoint_period == 0 || i == 0 || i + 1 == steps_number) {
            save_answer("test_results/checkpoints/checkpoint_" +
                        int_to_string(checkpoint_number++) + ".txt", problem);
        }
    }

    print_final_penalty_info(annealing, problem, penalties);

    save_answer("test_results/final_answer.txt", problem);
}

int main() {
    // TODO: transfer within the same line
    // TODO: smarter problem initializing
    // TODO: swap halves of 2 random lines once in several iterations in order to avoid stagnation

    make_rectangle_test(17, 29, 10);
    return 0;
}
