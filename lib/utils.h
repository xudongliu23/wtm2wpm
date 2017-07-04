#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* Get domain info for all issues from the description data file */
void getDomains(vi *bitMapIssues, vpIssDom *domains, const string domainDescriptionFile);

/* Display domains of all issues */
void displayDomains(vpIssDom *domains);

/* Get outcomes from the outcomes data file */
void getOutcomes(umIntVstring *outcomes, const string outcomesFile);

/* Display outcomes */
void displayOutcomes(umIntVstring *outcomes);

/* Get training and testing examples from the examples data file */
void getExamples(vi *bitMapTrainExamples,
                 vpii *trainExamples,
                 vpii *testExamples,
                 const string examplesFile,
                 const int numTrainExamples);

/**
 * @brief sampling with replacement
 *
 * @param bitMapTrainExamples
 * @param trainExamples
 * @param testExamples
 * @param examplesFile
 * @param numTrainExamples
 */
void getExamplesWithReplacement(
        vi *bitMapTrainExamples,
        vpii *trainExamples,
        vpii *testExamples,
        const string examplesFile,
        const int numTrainExamples);

/* Get all examples from the examples data file */
void getAllExamples(vpii *examples, const string examplesFile);

/* Display examples */
void displayExamples(vpii *examples);

/* Display examples in details */
void displayExamplesDetails(vpii *examples, umIntVstring &outcomes);

/* Display permuations of strings */
void displayPerms(vvstring *perms);

UInt32 getNumUnusedExamples(vi *bitMapTrainStrictExamples);

UInt32 getNumUnusedIssues(vi *bitMapIssues);

UInt32 getNumOnesInBitMap(vi *bitMap);

int getIssIndFromName(string name, vpIssDom *domains);

/**
 * @brief get the index of value val in an issue's domain
 *
 * @param val
 * @param domain
 *
 * @return index of val in domain
 */
int getValIndInIssDomain(string val, pIssDom domain);

bool agree(string value1, string value2, vstring *issPref);

int getValIndInPref(string val1, vstring issPref);

void printVstring(vstring * vstr);

void printVi(vi * vint);

int sumVi(vi * vint);

/* Get numTrainStrictExamplesPerTree examples from trainStrictExamples starting
   from position startingPos, if possible;
   Otherwise, get whatever that is left.
*/
void getTrainStrictExamplesPerTree(vpii* trainStrictExamplesPerTree,
                                   vpii *trainStrictExamples,
                                   int startingPos,
                                   const int numTrainStrictExamplesPerTree);

/* Get numTrainStrictExamplesPerTree examples randomly from trainStrictExamples */
void getTrainStrictExamplesPerTreeRandom(vpii* trainStrictExamplesPerTree,
                                         vpii trainStrictExamples,
                                         const int numTrainStrictExamplesPerTree);

void getTrainStrictExamplesPerTreeRandomWithReplacement(vpii* trainStrictExamplesPerTree,
                                         vpii trainStrictExamples,
                                         const int numTrainStrictExamplesPerTree);

int getNumLinesInFile(const string fileName);

void getLinesInFile(vstring *lines, const string fileName);

/* Get number strings randomly from strings */
void getRandStrings(vstring *randStrings, vstring strings, const int number);

/**
 * @brief compute the size of the combinatorial domain (i.e., total number of outcomes)
 *
 * @param domains
 *
 * @return size of the combinatorial domain
 */
int computeSizeOfCombDomain(vpIssDom *domains);

/**
 * @brief compute the Spearman's Rho between two total orders
 *
 * @param to1
 * @param to2
 *
 * @return Spearman's Rho in [-1,1]: the closer to 1 the more similar, and
 *         the closer to -1 the more dissimilar.
 */
double computeSpearmanRhoBetweenTotalOrders(
        vi *to1,
        vi *to2);

/**
 * @brief linearize a given total preorder to compute a total order
 *        tie breaking rule is applied in favor of smaller-indexed outcomes
 *
 * @param tp
 * @param to: the result
 */
void computeTotalOrderFromTotalPreorder(
        vector<sInt> *tp,
        vi* to);

/**
 * @brief compute the outcome at ind position in the combinatorial space
 *
 * @param ind: index of an outcome in the combinatorial domain, starting from 0 to size of CD-1.
 * @param domains
 *
 * @return the outcome as a vector of values for each issue.
 */
vstring computeOutcomeFromIndInCombDomain(
        int ind,
        vpIssDom *domains);

/**
 * @brief compute the index of a given outcome o in the combinatorial domain
 *
 * @param o
 * @param domains
 *
 * @return the index of o in CD, from 0 to size of CD-1
 */
int computeIndFromOutcomeInCombDomain(
        vstring o,
        vpIssDom *domains);

/**
 * @brief generate tabs of count many and return them as a string
 *
 * @param count
 * @return a string of tabs of count many
 */
string tabs(int count);


#endif // UTILS_H
