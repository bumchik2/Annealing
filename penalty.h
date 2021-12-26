#pragma once

#include "problem.h"

#include <stdexcept>
#include <string>
#include <vector>
#include <memory>

using std::runtime_error;
using std::string;
using std::vector;
using std::shared_ptr;

class Penalty {
public:
    Penalty(double penalty_size): penalty_size(penalty_size) {}

    virtual double get_penalty(const Problem& problem) {
        throw runtime_error("base get penalty method called");
    }

    virtual string get_name() {
        throw runtime_error("base get name method called");
    }

    const double penalty_size;
};

void print_penalties(const vector<shared_ptr<Penalty>>& penalties, const Problem& problem);
