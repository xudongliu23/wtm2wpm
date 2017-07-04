#include "helper.h"
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;


int numLinesInFile(const string file)
{
    ifstream ifile;
    size_t numLines = 0;

    ifile.open(file.c_str());
    if(!ifile.is_open()) {
        cout << "Error opening file: " << file << endl;
    } else {
        string line;
        while(getline(ifile, line)) {
            numLines++;
        }
    }
    ifile.close();
    return numLines;
}

vstring split(string input, const char splitter)
{
    vstring splits;
    stringstream input_stream (input);
    string cell_value;
    while ( getline (input_stream, cell_value, splitter) ){
        splits.push_back(cell_value);
    }

    return splits;
}

UInt32 LevenshteinDistance(const vi &s1, const vi &s2)
{
    const size_t len1 = s1.size(), len2 = s2.size();
    vui col(len2+1), prevCol(len2+1);

    for (UInt32 i = 0; i < prevCol.size(); i++)
        prevCol[i] = i;
    for (UInt32 i = 0; i < len1; i++) {
        col[0] = i+1;
        for (UInt32 j = 0; j < len2; j++)
            col[j+1] = min( min(prevCol[1 + j] + 1, col[j] + 1),
                    prevCol[j] + (s1[i]==s2[j] ? 0 : 1) );
        col.swap(prevCol);
    }
    return prevCol[len2];
}

UInt32 computeSizeOfIntersection(const vi &v1, const vi &v2)
{
    UInt32 number = 0;
    for(size_t i=0; i<v1.size(); i++){
        size_t j;
        for(j=0; j<v2.size(); j++){
            if(v2[j] == v1[i]){
                number += 1;
                break;
            }
        }
    }

    return number;
}

bool eqVectors(vstring v1, vstring v2)
{
    bool are_equal = true;
    REP (i, 0, v1.size()-1) {
      if (v1[i] != v2[i]) {
        are_equal = false;
        break;
      }
    }

    return are_equal;
}
