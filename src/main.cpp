#include <iostream>
#include "stdlib.h"
#include <time.h>
#include <string>
#include "include/wtm2wpm.h"
using namespace std;


int main(int argc, char** argv)
{
    srand ( time(NULL) ); //initialize the random seed

    if (argc != 3) {
        cout << "No enough arguments!" << endl;
        exit(1);
    }

    const string wtmfile (argv[1]);
    const string wpmfile (argv[2]);

    dump(wtmfile, wpmfile);

    return 0;
}




