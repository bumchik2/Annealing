#pragma once

#include "mutation.h"
#include "penalty.h"

#include <vector>
#include <memory>

using std::shared_ptr;
using std::vector;

class AddVertexMutation: public Mutation {
public:
    AddVertexMutation(const vector<shared_ptr<Penalty>>& penalties): Mutation(penalties) {
    }

    // adds random vertex into random line

    double get_delta_penalty(const Problem& problem, int random_seed) override;
    void mutate(Problem& problem, int random_seed) override;

private:
    double get_delta_distance_(const Problem& problem, int line_number, unsigned pos, int v);

};
