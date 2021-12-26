#pragma once

#include "penalty.h"
#include "problem.h"

#include <string>

using std::string;

class MaximumPenalty: public Penalty {
public:
    // penalty for difference between maximum and minimum lines

    MaximumPenalty(double penalty_size): Penalty(penalty_size) {}

    string get_name() override {
        return "Maximum penalty";
    }

    double get_penalty(const Problem& problem) override;
};
