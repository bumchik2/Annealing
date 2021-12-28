#pragma once

#include "problem.h"
#include "penalty.h"

#include <stdexcept>
#include <unordered_map>
#include <memory>

using std::shared_ptr;
using std::runtime_error;
using std::unordered_map;

class Mutation {
public:
    // each Mutation has to execute one and the same mutation
    // for fixed problem and random_seed

    Mutation(vector<shared_ptr<Penalty>> penalties) {
        for (unsigned i = 0; i < penalties.size(); ++i) {
            penalty_sizes_[penalties[i]->get_name()] = penalties[i]->penalty_size;
        }
    }

    virtual double get_delta_penalty(const Problem& problem,
            int random_seed) {
        throw runtime_error("base get_fine method called");
    }

    virtual void mutate(Problem& problem, int random_seed) {
        throw runtime_error("base mutate method called");
    }

protected:
    unordered_map<string, double> penalty_sizes_;

};
