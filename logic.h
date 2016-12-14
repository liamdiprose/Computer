//
// Created by liam on 11/04/16.
//

#ifndef COMPUTER_LOGIC_H
#define COMPUTER_LOGIC_H


// DEFINES logic gate components

#include <vector>
#include <string>
#include <map>



class Gate {
private:
    bool gray = false;
    static std::map<std::string, Gate*> gates;
    std::string name;
protected:
    bool state = false;
    virtual bool calculate() = 0;
    std::vector<Gate*> inputs;

public:
    static Gate* factory(std::string name, std::string gateType);
    static Gate* getGate(std::string name);
    Gate(std::string name);
    void setOutput(Gate* g);
    virtual void setInput(Gate* g);
    bool areParentsUpdated();
    void update();
    bool isGray();
    void setGray();
    bool getState();

    //bool isGate = true;
    std::vector<Gate*> outputs;
    std::string getName();
    void deleteInput(Gate* inputName);
    void deleteOutput(Gate* outputName);
    static bool isGate(std::string gateType);
   // bool getIsGate();
};

class AND : public Gate {
protected:
    virtual bool calculate();

public:
    AND(std::string name) : Gate(name) {};
};

class OR : public Gate {
protected:
    virtual bool calculate();
public:
    OR (std::string name) : Gate(name) {};
};

class XOR : public Gate {
protected:
    virtual bool calculate();

public:
    XOR (std::string name) : Gate(name) {};
};

class NAND : public AND {
private:
    bool calculate();
public:
    NAND (std::string name) : AND(name) {};
};
class NOR : public OR {
private:
    bool calculate();
public:
    NOR (std::string name) : OR(name) {};
};
class XNOR : public XOR {
private:
    bool calculate();
public:
    XNOR (std::string name) : XOR(name) {};
};

class Input : public Gate {
private:
    bool calculate();
public:
    Input(std::string name) : Gate(name){};
    void set(bool newState);
    std::vector<Gate*> getOutputs();
};

class Output : public Gate {
private:
    bool calculate();
public:
    Output(std::string name) : Gate(name) {};
    std::vector<Gate*> getInputs();

};

#endif //COMPUTER_LOGIC_H
