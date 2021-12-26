#pragma once

#include "mutation.h"
#include "penalty.h"

#include <vector>
#include <memory>
#include <stdexcept>

using std::shared_ptr;
using std::vector;
using std::runtime_error;

class RemoveVertexMutation: public Mutation {
public:
    RemoveVertexMutation(const vector<shared_ptr<Penalty>>& penalties): Mutation(penalties) {
        throw runtime_error("So far deprecated mutation");
    }

    // removes random vertex from random line

    double get_delta_penalty(const Problem& problem, int random_seed) override;
    void mutate(Problem& problem, int random_seed) override;
};
