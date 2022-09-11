#pragma once

#include "mutation.h"
#include "penalty.h"

#include <stdexcept>
#include <vector>
#include <memory>

using std::shared_ptr;
using std::vector;
using std::runtime_error;

class SwapBetweenLinesMutation: public Mutation {
public:
    // swaps a pair of vertex from two different lines

    SwapBetweenLinesMutation(const vector<shared_ptr<Penalty>>& penalties): Mutation(penalties) {
    }

    double get_delta_penalty(const Problem& problem, int random_seed) override;
    void mutate(Problem& problem, int random_seed) override;

private:
    vector<int> choose_mutation_parameters_(const Problem& problem);
    vector<double> get_delta_distances_(const Problem& problem, int line_number1, int line_number2, unsigned pos1, unsigned pos2);

};
