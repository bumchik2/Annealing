#pragma once

#include "mutation.h"
#include "penalty.h"

#include <vector>
#include <memory>

using std::shared_ptr;
using std::vector;

class TransferMutation: public Mutation {
public:
    // transfers a vertex from one place to another

    TransferMutation(const vector<shared_ptr<Penalty>>& penalties): Mutation(penalties) {
    }

    double get_delta_penalty(const Problem& problem, int random_seed) override;
    void mutate(Problem& problem, int random_seed) override;

private:
    vector<int> get_delta_distances_(const Problem& problem, int line_number1, int line_number2, unsigned pos1, unsigned pos2);

};
