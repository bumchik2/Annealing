#pragma once

#include "penalty.h"
#include "problem.h"

#include <string>

using std::string;

class VertexDifferencePenalty: public Penalty {
public:
    // penalty for difference between maximum and minimum locations number

    VertexDifferencePenalty(double penalty_size): Penalty(penalty_size) {}

    string get_name() override {
        return "Vertex difference penalty";
    }

    double get_penalty(const Problem& problem) override;
};
