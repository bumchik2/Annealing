#pragma once

#include "mutation.h"
#include "penalty.h"

#include <vector>
#include <memory>

using std::shared_ptr;
using std::vector;

class SwapHeadTailMutation: public Mutation {
public:
    // transfers a vertex from one place to another

    SwapHeadTailMutation(const vector<shared_ptr<Penalty>>& penalties): Mutation(penalties) {
    }

    double get_delta_penalty(const Problem& problem, int random_seed) override;
    void mutate(Problem& problem, int random_seed) override;

private:
    int get_delta_distance_(const Problem& problem, int line_number, unsigned pos);

};
