//
// Created by liam on 11/04/16.
//

#include <iostream>
#include "ICLoader.h"
#include "logic.h"
#include "yaml-cpp/yaml.h"
#include <algorithm>



void Component::set(std::string inputName, bool state) {
    Input* input = inputs.at(inputName);
    input->set(state);
    update(input);
}


Output* Component::getOutput(std::string outputName) {
    return outputs.at(outputName);
}

void Component::update(Gate *startNode) {
  //  std::cout << "------------UPDATE BEGIN---------------" << std::endl;
    std::deque<Gate*> queue;
    std::deque<Gate*> list;

    startNode->setGray();
    queue.push_back(startNode);

    Gate* current;

    while (!queue.empty()) {
        current = queue.front();

       // std::cout << "Update Loop: Loop for " << current << std::endl;
        list.push_back(current);
        for (Gate* g: current->outputs) {
            if (!g->isGray()) {
                //std::cout << "Update: Adding Output" << std::endl;
                queue.push_back(g);
                g->setGray();
            }
        }
        queue.pop_front();
       // std::cout << "New queue size: " << queue.size() << " Empty? " << queue.empty() << std::endl;

    }

    //std::cout << "BFS queue has finshed with " << list.size() << " items." << std::endl;

//    std::cout << "Update Queue: [";
//    for (Gate* g : list) {
//        std::cout << g->getName() << " ";
//    }
//    std::cout << "]" <<std::endl;

    while (!list.empty()) {
        current = list.front();
        list.pop_front();
        if (current->isGray()) {
           // std::cout << "Loop with Gray node" <<std::endl;
        }
        Component::BFSUpdate(current, &list);
    }
}

void Component::BFSUpdate(Gate *node, std::deque<Gate *> *queue) {

    if (node->isGray()) {
       //std::cout << "BFS Update: " << node->getName() << std::endl;
        if (node->areParentsUpdated()) {
            node->update();
            //std::cout << "node updated, Gray? " << node->isGray() << std::endl;
            for (Gate *output : node->outputs) {
                //std::cout << "   ";
                Component::BFSUpdate(output, queue);
            }
        }
        else {
          //  std::cout << "[skipped, parents not updated]" << std::endl;
        }
        return;
    }
    else {
        return;
    }
}

Component::Component(std::string name, std::string filename) {
    //std::cout << "---------GRAPH GEN START-----------" << std::endl;
    //std::cout << "Loading filename: " << filename << std::endl;
    YAML::Node config = YAML::LoadFile(filename);
    this->name = name;
    for (YAML::const_iterator it=config["inputs"].begin(); it!=config["inputs"].end(); ++it) {
        std::string inputName = it->as<std::string>();
//        std::cout << "Adding " << name << " Input: " << inputName << std::endl;
        std::pair<std::string, Input*> inp = std::pair<std::string, Input*>(it->as<std::string>(), new Input(name + ":" + inputName));
        std::pair<std::string, Gate*> gate = std::pair<std::string, Input*>(it->as<std::string>(), new Input(name + ":" + inputName));

        inputs.insert(inp);
        gates.insert(inp);
    }
    for (YAML::const_iterator it=config["outputs"].begin(); it!=config["outputs"].end(); ++it) {
        std::string outputName = it->as<std::string>();
 //       std::cout << "Adding Output: " << outputName << std::endl;
        std::pair<std::string,Output*> out = std::pair<std::string, Output*>(it->as<std::string>(), new Output(name + ":" + outputName));
        outputs.insert(out);
    }
 //   std::cout << "Finding components" << std::endl;
    std::map<std::string, Component> components;

    for (YAML::const_iterator it=config["components"].begin(); it!=config["components"].end(); ++it) {

        std::string gateName = it->first.as<std::string>();
        std::string type = it->second.as<std::string>();



        if (Gate::isGate(type)) {
 //           std::cout << "Adding Gate: " << gateName << " (Type: " << type << ")" << std::endl;
            gates.insert(std::pair<std::string, Gate *>(gateName, Gate::factory(name + ":" + gateName, type)));
        }
        else {
 //           std::cout << "Adding component: " << gateName << " (Type: " << type << ")" << std::endl;
            Component subcomponent (gateName, type + ".component"); //TODO File exists check
            //subcomponent.printGraph();
            components.insert(std::pair<std::string, Component>(gateName, subcomponent));
        }
    }

//    std::cout << "Disolving components into gates" << std::endl;

//    std::cout << name << " inputs: [";
//    for (std::pair<std::string, Input*> currentComponent : inputs) {
//        std::cout << currentComponent.first << " ";
//    }
//    std::cout << "]" <<std::endl;

    for (std::pair<std::string, Component> currentComponent : components) {
        std::string componentName = currentComponent.first;
        Component currentComponentObj = currentComponent.second;
        //std::cout << "Looping " << componentName << "'s Outputs" << std::endl;
        for (auto it=config[componentName].begin(); it!=config[componentName].end(); ++it) {
            std::string outputName = it->first.as<std::string>();
            YAML::Node recevierList = it->second;

            for (YAML::const_iterator it2=recevierList.begin(); it2 != recevierList.end(); ++it2) {
                std::string receiverName = it2->first.as<std::string>();
                std::string receiverPlug = it2->second.as<std::string>();

               // bool isGate = gates.find(receiverName) != gates.end();
                bool isGate = gates.find(receiverPlug) != gates.end();

               // std::cout << receiverName << ":" << receiverPlug << " is a gate? " << isGate << std::endl;

                if (isGate) {
                    // ALGO for connecting component to gate (Output Removal)
                    //std::cout << "Adding a Gate to the end of a component" << std::endl;
                    Output* outputObj = currentComponentObj.outputs.at(outputName);
                    Gate* receivingGate = gates.at(receiverName);

                    Gate* sendingGate = outputObj->getInputs().at(0);

                    sendingGate->setOutput(receivingGate);
                    sendingGate->deleteInput(outputObj);

                }
                else if (receiverName == "OUT") {
                    //ALGO fo connecting component to output
                    //std::cout << "Appending new output obj" << std::endl;
                    Output* outputObj = currentComponentObj.outputs.at(outputName);
                    //std::cout << "Attempting to find output at " << receiverPlug << " in component " << name << std::endl;
                    Output* receivingOuput = outputs.at(receiverPlug);

                    Gate* sendingGate = outputObj->getInputs().at(0);
                    sendingGate->setOutput(receivingOuput);
                    //std::cout << "Is outputobj in sendingGate's outputs? " << (std::find(sendingGate->outputs.begin(), sendingGate->outputs.end(), outputObj) != sendingGate->outputs.end()) << std::endl;
                    sendingGate->deleteOutput(outputObj);
                }
                else {
                    //ALGO for connecting component to another component
                    //std::cout << "attempting to find output name: " << outputName << std::endl;
                    Output* outputObj = currentComponentObj.outputs.at(outputName);
                    //std::cout << "attemping to find input obj  " << receiverName << ":" << receiverPlug << std::endl;
                    Input* inputObj = components.at(receiverName).inputs.at(receiverPlug);

                    Gate* sendingGate = outputObj->getInputs().at(0);

                    for (Gate* inputsOutput : inputObj->getOutputs()) {
                        sendingGate->setOutput(inputsOutput);
                        inputsOutput->deleteInput(inputObj);
                    }
                    sendingGate->deleteOutput(outputObj);

                }
            }
        }
    }

    //std::cout << "Looping over Gates" << std::endl;
    for (std::pair<std::string, Gate*> g : gates) {
        std::string gateName = g.first;
        Gate* gateObj = g.second;

        for (YAML::const_iterator it=config[gateName].begin(); it!=config[gateName].end(); ++it) {
            std::string receiverName = it->first.as<std::string>();
            std::string receiverPlug = it->second.as<std::string>();

            //std::cout << "Finding if  " << receiverName << " is a gate. Searching : \n[";

//            for (std::pair<std::string, Gate*> g2 : gates) {
//                std::cout << g2.first << " ";
//            }
            //std::cout << "]" << std::endl;

            bool isGate = gates.find(receiverName) != gates.end();

            Gate* sendingGate = gateObj;


            if (isGate) {
                // Add Gate to another gate
                //std::cout << receiverName << " is a Gate" << std::endl;
                Gate* receivingGate = gates.at(receiverName);
                sendingGate->setOutput(receivingGate);
            }
            else if (receiverName == "OUT") {
                // Add output obj to Gates outputs
               //std::cout << receiverName << " is a Out" << std::endl;
                Output* receivingOutput = outputs.at(receiverPlug);
                sendingGate->setOutput(receivingOutput);
            }
            else {
                // Add gate into component (remove Input obj)
               // std::cout << receiverName << " is a Component" << std::endl;
                Input* receivingInput = components.at(receiverName).inputs.at(receiverPlug);
                for (Gate* inputsOutput : receivingInput->getOutputs()) {
                    sendingGate->setOutput(inputsOutput);
                    inputsOutput->deleteInput(receivingInput);
                }

            }
        }

    }
}

std::ostream &operator<<(std::ostream &os, const Component &component) {
//    for (std::pair<std::string, Gate*> gate : component.inputs) { // change to gates for all gate
//        os << gate.first << ": " << gate.second->getState() << std::endl;
//    }
//    os << std::endl;
    for (std::pair<std::string, Gate*> gate : component.outputs) {
        os << gate.first << ": " << gate.second->getState() << std::endl;
    }
    return os;

}

void Component::printGraph() {
    for (std::pair<std::string, Gate*> g : gates) {
        //std::cout << this->name << ":" << g.first << " [";
        for (Gate* h : g.second->outputs) {
            std::cout << h->getName() << " ";
        }
        std::cout << "]" << std::endl;
    }
}

std::map<std::string, bool> Component::getOutputs() {
    std::map<std::string, bool> ret;
    for (std::pair<std::string, Output*> o : outputs) {
        ret[o.first] = o.second->getState();
    }
    return ret;
}
