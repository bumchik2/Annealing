#pragma once

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <codecvt>

using std::string;
using std::size_t;
using std::istream;
using std::getline;
using std::vector;

std::wstring read_file(const char* filename);

class CSVRow {
public:
    string operator[] (size_t index) const {
        return string(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
    }
    size_t size() const {
        return m_data.size() - 1;
    }
    void readNextRow(istream& str) {
        getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        string::size_type pos = 0;
        while((pos = m_line.find(',', pos)) != string::npos)
        {
            m_data.emplace_back(pos);
            ++pos;
        }
        // This checks for a trailing comma with no data after it.
        pos   = m_line.size();
        m_data.emplace_back(pos);
    }

private:
    string m_line;
    vector<int> m_data;

};

istream& operator >>(istream& str, CSVRow& data);

class CSVIterator {
public:
    typedef std::input_iterator_tag     iterator_category;
    typedef CSVRow                      value_type;
    typedef size_t                 difference_type;
    typedef CSVRow*                     pointer;
    typedef CSVRow&                     reference;

    CSVIterator(std::istream& str)  :m_str(str.good()?&str:NULL) { ++(*this); }
    CSVIterator()                   :m_str(NULL) {}

    // Pre Increment
    CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = NULL;}}return *this;}
    // Post increment
    CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
    CSVRow const& operator*()   const       {return m_row;}
    CSVRow const* operator->()  const       {return &m_row;}

    bool operator ==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));}
    bool operator !=(CSVIterator const& rhs) {return !((*this) == rhs);}

private:
    istream* m_str;
    CSVRow m_row;

};


class CSVRange {
private:
    istream&   stream;

public:
    CSVRange(istream& str)
        : stream(str)
    {}
    CSVIterator begin() const {return CSVIterator{stream};}
    CSVIterator end()   const {return CSVIterator{};}

};
