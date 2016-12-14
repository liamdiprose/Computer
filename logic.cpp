//
// Created by liam on 11/04/16.
//

#include <string.h>
#include <iostream>
#include "logic.h"
#include "string"
#include "algorithm"

std::map<std::string, Gate*> Gate::gates;

bool Gate::areParentsUpdated() {
    for (Gate* g : inputs) {
        if (g->isGray()) {
            return false;
        }
    }
    return true;
}

void Gate::update() {
//    std::cout << name << " [";
//    for (Gate* g : inputs) {
//    std::cout << g->getState();
//    }
//    std::cout << "] New state: ";
    state = calculate();
//    std::cout << state << std::endl;
    gray = false;
}

bool Gate::isGray() {
    return gray;
}

bool Gate::getState() {
    return state;
}

bool AND::calculate() {
    for (Gate* g : inputs) {
        if (!g->getState()) {
            return false;
        }
    }
    return true;
}

Gate::Gate(std::string name) {
    this->name = name;
}

void Gate::setOutput(Gate *g) {
 //   std::cout << this->name <<  " adding output " << g->getName() << std::endl;
    outputs.push_back(g);
    g->setInput(this);
}

void Gate::setInput(Gate *g) {
    inputs.push_back(g);
}

Gate *Gate::factory(std::string name, std::string gateType) {
    if (gateType == "AND") {
        AND* an = new AND(name);
        Gate::gates[name]= an;
        return an;
    }
    else if (gateType == "OR"){
        OR* ore = new OR(name);
        Gate::gates[name] = ore;
        return ore;
    }
    else if (gateType == "XOR") {
        XOR* xore = new XOR(name);
        Gate::gates[name] = xore;
        return xore;
    }
    else if (gateType =="NAND") {
        NAND* nand = new NAND(name);
        Gate::gates[name] = nand;
        return nand;
    }
    else {
        throw 1;
    }
}

Gate *Gate::getGate(std::string name) {
    return Gate::gates.at(name);
}

void Gate::setGray() {
    gray = true;
}

void Input::set(bool newState) {
    state = newState;
}

bool Input::calculate() {
    return state;
}

bool Output::calculate() {
    return inputs.at(0)->getState();
}


bool OR::calculate() {
    for (Gate* g : inputs) {
        if (g->getState()) {
            return true;
        }
    }
    return false;
}

bool XOR::calculate() {
    return (inputs.at(0)->getState() != inputs.at(1)->getState());
}

bool NAND::calculate() {
    return (! AND::calculate());
}
bool NOR::calculate() {
    return (! OR::calculate());
}
bool XNOR::calculate() {
    return (inputs[0]->getState() == inputs[1]->getState());
}

std::string Gate::getName() {
    return name;
}

bool Gate::isGate(std::string gateType) {
    //std::cout << "Finding if " << gateType << " is a Gate" << std::endl;
    for (std::string gateName : {"AND", "OR", "XOR", "NAND", "NOR", "XNOR"}) {
        if (gateName == gateType) {
            //std::cout << gateType << " is in the list" << std::endl;
            return true;
        }
    }
    return false;
}

std::vector<Gate *> Output::getInputs() {
    return inputs;
}

std::vector<Gate *> Input::getOutputs() {
    return outputs;
}

void Gate::deleteInput(Gate* input) {

    this->inputs.erase(std::remove(inputs.begin(), inputs.end(), input), inputs.end());
}

void Gate::deleteOutput(Gate* val) {

    // TODO PROBLEM STARTS HERE

//    std::cout << "Trying to delete " << val->getName() << " from " << this->getName() << std::endl;
//    std::cout << this->getName() << "'s outputs: [";
//    for (Gate* out : outputs) {
//        std::cout << out->getName() << " ";
//    }
//    std::cout << "]" << std::endl;
    outputs.erase(std::remove(outputs.begin(), outputs.end(), val), outputs.end()); // TODO

    //
    //this->outputs.erase(outputName);
    //outputs.erase()
    //this->outputs.erase(std::remove(outputs.begin(), outputs.end(), val), outputs.end());
}
//
//bool Gate::getIsGate() {
//    return isGate;
//}
