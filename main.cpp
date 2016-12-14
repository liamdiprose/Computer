#include <iostream>
#include "logic.h"
#include "ICLoader.h"

using namespace std;

int main(int argc, char *argv[]) {
    std::string filename = argv[1];
    Component c ("M", filename);

    string inpu = "not nothing";
    string prefix;
    while (!inpu.empty()) {
        getline(cin, inpu);
        if (inpu.empty()){
            break;
        }
        //std::cout << "Getting another input..." << std::endl;
        if (inpu.front() == '+') {
            //std::cout << "Setting " << inpu.substr(1) << " high" << endl;
            c.set(inpu.substr(1), true);
        } else {
            c.set(inpu.substr(1), false);
        }
    }

//    for (pair<string, bool> output : c.getOutputs()) {
//        cout << output.second;
//    }
//    cout << endl;
    cout << c;



}
