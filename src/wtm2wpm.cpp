#include "include/wtm2wpm.h"
#include "lib/types.h"


void dump(const string wtmfile, const string wpmfile) {
    string line;
    int nvars = 0;
    int nclauses = 0;
    ull top = 0;

    ifstream input (wtmfile);
    if (!input.is_open()) {
        cout << "Unable to open file " << wtmfile << " for input!" << endl;
        exit(1);
    }
    ofstream output (wpmfile);
    if (!output.is_open()) {
        cout << "Unable to open file " << wpmfile << " for output!" << endl;
        exit(1);
    }

    /* Read wtmfile to compute nvars, nclauses and top*/
    while (getline(input, line)) {
        if (line[0]=='c' || line[0]=='\0') {
            /* skip comment lines and empty lines */
        } else if (line[0] == 'p') {
            int found = line.find(" ", 6);
            nvars = stoi(line.substr(6, found-6));
        } else {
            int pos = 0;
            int found = 0;
            while ((found=line.find(" ", pos)) != string::npos) {
                if (!pos) {
                    nvars++;
                    top += stoi(line.substr(pos, found-pos));
                }
                nclauses++;
                pos = found+1;
            }
            nclauses++;
        }
    }
    top++;  // top needs to be strictly bigger than sum of weights

    // clear and rewind
    input.clear();
    input.seekg(0);

    output << "c" << endl;
    output << "c wpm file for " << wtmfile << endl;
    output << "c" << endl << endl;

    while (getline(input, line)) {
        if (line[0]=='c' || line[0]=='\0') {
            /* skip comment lines and empty lines */
        } else if (line[0] == 'p') {
            output << "p wcnf " << nvars << " " << nclauses
                   << " " << top << endl;
            int found = line.find(" ", 6);
            nvars = stoi(line.substr(6, found-6));  // reset
        } else {
            int pos = 0;
            int found = 0;
            string negliterals = "";
            while ((found=line.find(" ", pos)) != string::npos) {
                if (!pos) {
                    output << line.substr(pos, found-pos) << " " << ++nvars
                           << " 0" << endl;
                } else {
                    output << top << " " << nvars*(-1) << " "
                           << line.substr(pos, found-pos) << " 0" << endl;
                    negliterals += to_string(stoi(line.substr(pos, found-pos))*(-1))
                                   + " ";
                }
                pos = found+1;
            }
            output << top << " " << nvars << " " << negliterals << "0" << endl;
        }
    }

    input.close();
    output.close();
}
