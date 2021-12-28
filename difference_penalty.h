#pragma once

#include "penalty.h"
#include "problem.h"

#include <string>

using std::string;

class DifferencePenalty: public Penalty {
public:
    // penalty for difference between maximum and minimum lines

    DifferencePenalty(double penalty_size): Penalty(penalty_size) {}

    string get_name() override {
        return "Difference penalty";
    }

    double get_penalty(const Problem& problem) override;
};
