#include "penalty.h"

#include <memory>
#include <iostream>

using std::shared_ptr;
using std::cout;
using std::endl;

void print_penalties(const vector<shared_ptr<Penalty>>& penalties, const Problem& problem) {
    double total_penalty = 0;
    for (const auto& penalty: penalties) {
        double penalty_amount = penalty->get_penalty(problem);
        total_penalty += penalty_amount;
        cout << penalty->get_name() << ": " << penalty_amount << endl;
    }
    cout << "Total penalty: " << total_penalty;
}
