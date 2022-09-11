#include "utils.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::ostream;
using std::vector;
using std::cout;
using std::endl;
using std::stoi;
using std::stringstream;

// rand generates random number between 0 and RAND_MAX inclusive

int randint(int l, int r, int seed) {
    assert(r > l);
    if (seed != -1) {
        srand(seed);
    }
    // random int between l inclusive and r exclusive
    int big_rand = rand() * (RAND_MAX - 1) + rand();
    return l + big_rand % (r - l);
}

double random_double(int seed) {
    if (seed != -1) {
        srand(seed);
    }
    double big_rand = rand() * (RAND_MAX - 1) + rand();
    double result = big_rand / (static_cast<double>(RAND_MAX) * RAND_MAX);
    if (result > 1) {
        cout << seed << ' ' << result << endl;
    }
    assert(0 <= result && result <= 1);
    return result;
}

void remove_file(const std::string& file_name) {
  const char* cstr = file_name.c_str();
  std::remove(cstr);
}

int string_to_int(const string& s) {
    return stoi(s);
}

string int_to_string(int a) {
    stringstream ss;
    ss << a;
    return ss.str();
}
