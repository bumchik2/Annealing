#pragma once

#include "penalty.h"
#include "problem.h"

#include <string>

using std::string;

class DistancePenalty: public Penalty {
public:
    DistancePenalty(double penalty_size): Penalty(penalty_size) {}

    string get_name() override {
        return "Distance penalty";
    }

    double get_penalty(const Problem& problem) override;
};
