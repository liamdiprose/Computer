//
// Created by liam on 11/04/16.
//

#ifndef COMPUTER_ICLOADER_H
#define COMPUTER_ICLOADER_H

#include "logic.h"
#include <deque>

class Component {
private:

    std::map<std::string, Gate*> gates;
    std::string name;
public:
    std::map<std::string, Input*> inputs;       //TODO Make inputs and outputs private again
    std::map<std::string, Output*> outputs;
    Component(std::string name, std::string filename);
    void set(std::string inputName, bool state);
    Output* getOutput(std::string outputName);
    static void update(Gate* startNode);
    static void BFSUpdate(Gate* node, std::deque<Gate*> *queue);
    friend std::ostream& operator<<  (std::ostream& os, const Component& component);
    void printGraph();
    std::map<std::string, bool> getOutputs();
};



#endif //COMPUTER_ICLOADER_H
