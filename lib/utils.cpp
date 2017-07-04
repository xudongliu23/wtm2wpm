#include "utils.h"
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <time.h>
#include "helper.h"


using namespace std;

void getDomains(vi *bitMapIssues, vpIssDom *domains, const string domainDescriptionFile)
{
    ifstream ifile;

    ifile.open(domainDescriptionFile.c_str());
    if(!ifile.is_open()) {
        cout << "Error opening file: " << domainDescriptionFile << endl;
    } else {
        string line;
        while(getline(ifile, line)) {
            size_t found_colon = line.find(":");
            string issue = line.substr(0,found_colon);
            vstring dom;
            size_t found_comma = line.find(",");
            string val;
            size_t start = found_colon+1;
            while(found_comma != string::npos) {
                val = line.substr(start, found_comma-start);
                dom.push_back(val);
                start = found_comma+1;
                found_comma = line.find(",", start);
            }
            dom.push_back(line.substr(start));  // get the last value
            domains->push_back(make_pair(issue, dom));
            bitMapIssues->push_back(1);
        }
    }
    ifile.close();
}

void displayDomains(vpIssDom *domains)
{
    REP(i, 0, domains->size()-1) {
        cout << "Issue " << i << " (" << domains->at(i).first << "):" << endl;
        REP(j, 0, domains->at(i).second.size()-1) {
            cout << domains->at(i).second.at(j) << " ";
        }
        cout << endl;
    }
}

void getOutcomes(umIntVstring *outcomes, const string outcomesFile)
{
    ifstream ifile;
    size_t lineInd = 0;

    ifile.open(outcomesFile.c_str());
    if(!ifile.is_open()) {
        cout << "Error opening file: " << outcomesFile << endl;
    } else {
        string line;
        while(getline(ifile, line)) {
            lineInd++;
            if(lineInd == 1) {continue;}

            int outcomeID;
            vstring outcome;
            size_t found_comma = line.find(",");
            outcomeID = stoi(line.substr(0, found_comma), NULL, 10);
            string val;
            size_t start = found_comma + 1;
            found_comma = line.find(",", start);
            while(found_comma != string::npos) {
                val = line.substr(start, found_comma-start);
                outcome.push_back(val);
                start = found_comma+1;
                found_comma = line.find(",", start);
            }
            outcome.push_back(line.substr(start));  // get the last value
            outcomes->insert({outcomeID, outcome});
        }
    }
    ifile.close();
}

void displayOutcomes(umIntVstring *outcomes)
{
    for (umIntVstring::iterator it=outcomes->begin(); it!=outcomes->end(); it++) {
        cout << "OutcomeID " << it->first << " :" << endl;
        REP(i, 0, it->second.size()-1) {
            cout << it->second[i] << " ";
        }
        cout << endl;
    }
}

void getExamples(vi *bitMapTrainExamples,
                 vpii *trainExamples,
                 vpii *testExamples,
                 const string examplesFile,
                 int numTrainExamples)
{
    string comm = "";
    if (numTrainExamples == -1) {  // -1 means to take all examples for training
        numTrainExamples = numLinesInFile(examplesFile)-1;
        comm = "exec bash -c 'tail -n +2 " + examplesFile + " | shuf'";
    } else if (numTrainExamples <= 0){
        cout << "Illegal number of examples (<=0)." << endl;
        exit(1);
    } else if (numTrainExamples > numLinesInFile(examplesFile)-1) {
        //cout << "Illegal number of examples (too many)." << endl;
        //exit(1);
        numTrainExamples = numLinesInFile(examplesFile)-1-100;
        comm = "exec bash -c 'tail -n +2 " + examplesFile + " | shuf'";
    } else {
        comm = "exec bash -c 'tail -n +2 " + examplesFile + " | shuf'";
    }

    const int MAX_BUFFER = 128;
    int counter = 0;
    FILE *fp;
    char buffer[MAX_BUFFER];
    fp = popen(comm.c_str(), "r");
    if (fp == NULL)
        perror("Error opening pipe");

    while (fgets(buffer, MAX_BUFFER, fp) != NULL) {
        string line(buffer);
        size_t found_comma = line.find(",");
        size_t start = found_comma + 1;
        found_comma = line.find(",", start);
        int firstOutcomeInd = atoi(line.substr(start, found_comma-start).c_str());
        int secondOutcomeInd = atoi(line.substr(found_comma + 1).c_str());

        if (counter < numTrainExamples) {
            trainExamples->push_back(make_pair(firstOutcomeInd, secondOutcomeInd));
            bitMapTrainExamples->push_back(1);
        } else {
            testExamples->push_back(make_pair(firstOutcomeInd, secondOutcomeInd));
        }

        counter++;
    }
    pclose(fp);
}

void getExamplesWithReplacement(
        vi *bitMapTrainExamples,
        vpii *trainExamples,
        vpii *testExamples,
        const string examplesFile,
        const int numTrainExamples)
{
    vstring examples;
    getLinesInFile(&examples, examplesFile);
    vb taken(examples.size(), false);

    for (int i=0; i<numTrainExamples; ++i) {
        int randInd = rand() % (examples.size()-1) +1;
        size_t found_comma = examples[randInd].find(",");
        size_t start = found_comma + 1;
        found_comma = examples[randInd].find(",", start);
        int firstOutcomeInd = atoi(examples[randInd].substr(start, found_comma-start).c_str());
        int secondOutcomeInd = atoi(examples[randInd].substr(found_comma + 1).c_str());

        trainExamples->push_back({firstOutcomeInd, secondOutcomeInd});
        bitMapTrainExamples->push_back(1);
        taken[randInd] = true;
    }

    for (int i=1; i<examples.size(); ++i) {
        if (!taken[i]) {
            size_t found_comma = examples[i].find(",");
            size_t start = found_comma + 1;
            found_comma = examples[i].find(",", start);
            int firstOutcomeInd = atoi(examples[i].substr(start, found_comma-start).c_str());
            int secondOutcomeInd = atoi(examples[i].substr(found_comma + 1).c_str());

            testExamples->push_back({firstOutcomeInd, secondOutcomeInd});
        }
    }
}

void getAllExamples(vpii *examples, const string examplesFile)
{
    string comm = "";
    comm = "exec bash -c 'tail -n +2 " + examplesFile + " | shuf'";

    const int MAX_BUFFER = 128;
    FILE *fp;
    char buffer[MAX_BUFFER];
    fp = popen(comm.c_str(), "r");
    if (fp == NULL)
        perror("Error opening pipe");

    while (fgets(buffer, MAX_BUFFER, fp) != NULL) {
        string line(buffer);
        size_t found_comma = line.find(",");
        size_t start = found_comma + 1;
        found_comma = line.find(",", start);
        int firstOutcomeInd = atoi(line.substr(start, found_comma-start).c_str())-1;
        int secondOutcomeInd = atoi(line.substr(found_comma + 1).c_str())-1;

        examples->push_back(make_pair(firstOutcomeInd, secondOutcomeInd));
    }
    pclose(fp);
}

void displayExamples(vpii *examples)
{
    REP(i, 0, examples->size()-1) {
        cout << "Example " << i << " :" << endl;
        cout << examples->at(i).first << ",";
        cout << examples->at(i).second << endl;
    }
}

void displayExamplesDetails(vpii *examples, umIntVstring& outcomes)
{
    REP(i, 0, examples->size()-1) {
        cout << "Example " << i << " :" << endl;
        REP (j, 0, outcomes[examples->at(i).first].size()-1) {
            cout << outcomes[examples->at(i).first][j] << ",";
        }
        cout << ";";
        REP (j, 0, outcomes[examples->at(i).second].size()-1) {
            cout << outcomes[examples->at(i).second][j] << ",";
        }
        cout << endl;
    }
}

void displayPerms(vvstring *perms)
{
    REP(i, 0, perms->size()-1) {
        cout << "Permutation " << i << " :" << endl;
        REP(j, 0, perms->at(i).size()-1) {
            cout << perms->at(i).at(j) << " ";
        }
        cout << endl;
    }
}

UInt32 getNumOnesInBitMap(vi *bitMap)
{
    UInt32 sum = 0;
    REP (i, 0, bitMap->size()-1) {
        if (bitMap->at(i) == 1) {
            sum++;
        }
    }
    return sum;
}

UInt32 getNumUnusedExamples(vi *bitMapTrainStrictExamples)
{
    return getNumOnesInBitMap(bitMapTrainStrictExamples);
}

UInt32 getNumUnusedIssues(vi *bitMapIssues)
{
    return getNumOnesInBitMap(bitMapIssues);
}

int getIssIndFromName(string name, vpIssDom *domains)
{
    int ind = -1;

    REP (i, 0, domains->size()-1) {
        if (domains->at(i).first == name) {
            ind = i;
            break;
        }
    }

    return ind;
}

int getValIndInIssDomain(string val, pIssDom domain)
{
    int ind = -1;
    for (int i=0; i<domain.second.size(); i++) {
        if (domain.second[i] == val) {
            ind = i;
            break;
        }
    }
    return ind;
}

bool agree(string value1, string value2, vstring *issPref)
{
    int ind1 = -1;
    int ind2 = -1;
    int counter = 0;
    REP (i, 0, issPref->size()-1) {
        // have two if's because value1 and value2 can be same.
        if (issPref->at(i) == value1) {
            ind1 = i;
            counter++;
        }
        if (issPref->at(i) == value2) {
            ind2 = i;
            counter++;
        }

        if (counter == 2)
            break;
    }

    return ind1 != -1 && ind2 != -1 && ind1 < ind2;
}

int getValIndInPref(string val1, vstring issPref)
{
    REP (i, 0, issPref.size()-1) {
        if (val1 == issPref[i])
            return i;
    }
    return -1;    // val1 is not found in the domain of issue at ind in domains
}

void printVstring(vstring * vstr)
{
    REP (i, 0, vstr->size()-1)
        cout << vstr->at(i) << " ";
    cout << endl;
}

void printVi(vi * vint)
{
    REP (i, 0, vint->size()-1)
        cout << vint->at(i) << " ";
    cout << endl;
}

int sumVi(vi * vint) {
    int sum=0;
    for (int i=0; i<vint->size(); i++) sum+=vint->at(i);

    return sum;
}

void getTrainStrictExamplesPerTree(vpii* trainStrictExamplesPerTree,
                                   vpii *trainStrictExamples,
                                   int startingPos,
                                   const int numTrainStrictExamplesPerTree)
{
    if (startingPos >= trainStrictExamples->size()) {
        cout << "Cannot pick training examples for tree!" << endl;
        exit(1);
    }

    int counter = 0;
    while (1) {
        if (counter == numTrainStrictExamplesPerTree)
            break;
        if (startingPos+counter == trainStrictExamples->size())
            break;

        trainStrictExamplesPerTree->push_back(trainStrictExamples->at(startingPos+counter));
        counter++;
    }
}

void getTrainStrictExamplesPerTreeRandom(vpii* trainStrictExamplesPerTree,
                                         vpii trainStrictExamples,
                                         const int numTrainStrictExamplesPerTree)
{
    if (numTrainStrictExamplesPerTree > trainStrictExamples.size()) {
        cout << "ERROR: numTrainStrictExamplesPerTree is more than there are!" << endl;
        exit(1);
    }

    pii tmp;
    int size = trainStrictExamples.size();
    int rind = size-1;

    for (int i=size-1; i>1; i--) {  // shuffle it
        rind = rand()%(i+1);
        tmp = trainStrictExamples[rind];
        trainStrictExamples[rind] = trainStrictExamples[i];
        trainStrictExamples[i] = tmp;
    }

    // pick the top numTrainStrictExamplesPerTree
    REP (i, 0, numTrainStrictExamplesPerTree-1) {
        trainStrictExamplesPerTree->push_back(trainStrictExamples[i]);
    }
}

void getTrainStrictExamplesPerTreeRandomWithReplacement(
        vpii* trainStrictExamplesPerTree,
        vpii trainStrictExamples,
        const int numTrainStrictExamplesPerTree)
{
    for (int i=0; i<numTrainStrictExamplesPerTree; ++i)
        trainStrictExamplesPerTree->push_back(trainStrictExamples[rand() % trainStrictExamples.size()]);
}

int getNumLinesInFile(const string fileName)
{
    ifstream ifile;
    int numLines = 0;
    string line = "";

    ifile.open(fileName.c_str());
    if(!ifile.is_open()) {
        cout << "Error opening file: " << fileName << endl;
    } else {
        while(getline(ifile, line)) {
            numLines++;
        }
    }
    ifile.close();

    return numLines;
}

void getLinesInFile(vstring *lines, const string fileName)
{
    ifstream ifile;
    string line = "";

    ifile.open(fileName.c_str());
    if(!ifile.is_open()) {
        cout << "Error opening file: " << fileName << endl;
    } else {
        while(getline(ifile, line)) {
            lines->push_back(line);
        }
    }
    ifile.close();
}

void getRandStrings(vstring *randStrings, vstring strings, const int number)
{
    if (number > strings.size()) {
        cout << "ERROR: number is more than there are!" << endl;
        exit(1);
    }

    //srand ( time(NULL) ); //initialize the random seed

    string tmp;
    int size = strings.size();
    int rind = size-1;

    for (int i=size-1; i>1; i--) {  // shuffle it
        rind = rand()%(i+1);
        tmp = strings[rind];
        strings[rind] = strings[i];
        strings[i] = tmp;
    }

    // pick the top numTrainStrictExamplesPerTree
    REP (i, 0, number-1) {
        randStrings->push_back(strings[i]);
    }
}

int computeSizeOfCombDomain(vpIssDom *domains)
{
    int res = 1;
    for (auto& domain : *domains) res*=domain.second.size();

    return res;
}

double computeSpearmanRhoBetweenTotalOrders(
        vi *to1,
        vi *to2)
{
    umIntInt objectToRank;  // for to2
    int n = to1->size();
    for (int i=0; i<n; i++) {
        objectToRank[(*to2)[i]] = i;
    }

    ull sumOfDSquares = 0;  // sum of d^2
    for (int i=0; i<n; i++) {
        ull d = i - objectToRank[(*to1)[i]];;
        sumOfDSquares += d*d;
    }

    return (1-(6*(double)sumOfDSquares/(n*(n*n-1))));
}

void computeTotalOrderFromTotalPreorder(
        vector<sInt> *tp,
        vi* to)
{
    for (int i=0; i<tp->size(); i++) {
        for (auto it=(*tp)[i].begin(); it!=(*tp)[i].end(); it++)
            to->push_back(*it);
    }
}

vstring computeOutcomeFromIndInCombDomain(
        int ind,
        vpIssDom *domains)
{
    vstring res;
    int issInd = domains->size()-1;
    int issDomainSize;

    while (issInd >= 0) {
        issDomainSize = (*domains)[issInd].second.size();

        res.insert(res.begin(), (*domains)[issInd].second.at(ind%issDomainSize));
        ind /= issDomainSize;
        issInd--;
    }

    return res;
}

int computeIndFromOutcomeInCombDomain(
        vstring o,
        vpIssDom *domains)
{
    int res = 0;
    int multiple = 1;

    for (int i=o.size()-1; i>=0; i--) {
        res += getValIndInIssDomain(o[i], (*domains)[i]) * multiple;

        multiple *= (*domains)[i].second.size();
    }

    return res;
}

string tabs(int count) {
    string res (count, '\t');
    return res;
}


