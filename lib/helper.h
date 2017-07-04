#ifndef HELPER_H
#define HELPER_H

#include "types.h"
#include <sstream>

//string intToString(const int number);
template <typename T>
string numberToString(const T number)  /* the implementation needs to be here */
{
    ostringstream oss;
    oss << number;
    return oss.str();
}

int numLinesInFile(const string file);

vstring split(string input, const char splitter);
UInt32 LevenshteinDistance(const vi &s1, const vi &s2);
UInt32 computeSizeOfIntersection(const vi &v1, const vi &v2);
bool eqVectors(vstring v1, vstring v2);


#endif // HELPER_H
