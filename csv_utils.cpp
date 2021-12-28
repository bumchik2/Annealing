#include "csv_utils.h"

#include <vector>

using std::istream;
using std::vector;
using std::wstring;

istream& operator >>(istream& str, CSVRow& data) {
    data.readNextRow(str);
    return str;
}

wstring read_file(const char* filename) {
    setlocale( LC_ALL, "" );
    std::wifstream wif(filename);
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}
