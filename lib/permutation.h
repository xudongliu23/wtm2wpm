#ifndef PERMUTATION_H
#define PERMUTATION_H

#include "types.h"

/* compute permutations of a vector of strings */
/* NOTE: assume no issue has a domain of more than 16 values */
void computePerms(vvstring *perms, vstring input);

/* generate all n-digit binary sequences */
void generateBinaries(vstring *binaries, string *have, int number_of_bits_total);

/* generate permutations of a given string, even when it has repeated characters */
void generateLexPerms(vstring& permutations, string& input_string);
string getNextLexPerm(string this_perm);
void reverseString(string& str, size_t start, size_t end);


#endif // PERMUTATION_H
