#include "permutation.h"
#include "helper.h"
#include <iostream>
using namespace std;


extern "C" const char HDTable[] = {'0', '1', '2', '3',
                                   '4', '5', '6', '7',
                                   '8', '9', ':', ';',
                                   '<', '=', '>', '?'};


void generateBinaries(vstring *binaries, string *have, int number_of_bits_total)
{
    // base case
    if( (int) have->size() == number_of_bits_total )
    {
        binaries->push_back(*have);
//        cout << *have << endl;
        return;
    }

    // two branches at any inner node
    for (int i=0; i<2; i++)
    {
        string cp_have = *have;

        generateBinaries(binaries, &cp_have.append(numberToString(i)), number_of_bits_total);

    }
}

void generateLexPerms(vector<string>& permutations, string& input_string)
{
    string permutation = "";

    while(true)
    {
        permutation = getNextLexPerm(input_string);

        if(permutation == "")
        {
            break;
        }
        else
        {
            permutations.push_back(permutation);
            input_string = permutation;
        }
    }
}

string getNextLexPerm(string this_perm)
{
    /* 1. Find the right most character that can be changed in order to bigger the whole string */
    size_t length = this_perm.size();
    size_t pos = length+1;

    for(size_t i=length-1; i>0; i--)
    {
        if(this_perm[i] > this_perm[i-1])  // i-1 is what we want to find
        {
            pos = i-1;
            break;
        }
    }

    if(pos == length+1)  // if cannot find that character, return empty string
    {
        return "";
    }
    else
    {
        /* 2. Increase this_perm[pos] */
        for(size_t l=length-1; l>pos; l--)
        {
            if(this_perm[pos] < this_perm[l])
            {
                // swith the two values
                char tmp = this_perm[pos];
                this_perm[pos] = this_perm[l];
                this_perm[l] = tmp;
                break;
            }
        }

        /* 3. Reverse this_perm[pos+1..length-1] */
        reverseString(this_perm, pos+1, length-1);

        return this_perm;
    }
}

void reverseString(string& str, size_t start, size_t end)
{
    for(size_t i=start; i<=(start+end)/2; i++)
    {
        // switch
        char tmp = str[i];
        str[i] = str[end-i+start];
        str[end-i+start] = tmp;
    }
}

void computePerms(vvstring *perms, vstring input)
{
    string inputInds = "";
    REP (i, 0, input.size()-1) {
        inputInds += numberToString(HDTable[i]);
    }

    vstring permsInputInds;
    permsInputInds.push_back(inputInds);
    generateLexPerms(permsInputInds, inputInds);

    REP (i, 0, permsInputInds.size()-1) {
        vstring perm;
        REP (j, 0, permsInputInds[i].length()-1) {
            perm.push_back(input.at(permsInputInds[i][j]-48));
        }
        perms->push_back(perm);
    }
}











