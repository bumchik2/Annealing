#pragma once

#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <string>
#include <vector>

using std::string;
using std::ostream;
using std::vector;

int randint(int l, int r, int seed = -1);
double random_double(int seed = -1);

void remove_file(const string& file_name);

template<typename T>
ostream& operator << (ostream& out, const vector<T>& v) {
    for (unsigned i = 0; i < v.size(); ++i) {
        if (i != 0) {
            out << " ";
        }
        out << v[i];
    }
    return out;
}

int string_to_int(const string& s);
string int_to_string(int a);
