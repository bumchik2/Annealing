#include "utils.h"

#include <iostream>
#include <vector>

using std::ostream;
using std::vector;

int randint(int l, int r, int seed) {
    assert(r > l);
    if (seed != -1) {
        srand(seed);
    }
    // random int between l inclusive and r exclusive
    int big_rand = rand() * RAND_MAX + rand();
    return l + big_rand % (r - l);
}

double random_double(int seed) {
    if (seed != -1) {
        srand(seed);
    }
    double big_rand = rand() * RAND_MAX + rand();
    double result = big_rand / (RAND_MAX * RAND_MAX);
    assert(0 <= result && result <= 1);
    return result;
}

void remove_file(const std::string& file_name) {
  const char* cstr = file_name.c_str();
  std::remove(cstr);
}
