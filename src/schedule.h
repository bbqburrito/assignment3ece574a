//
//  schedule.hpp
//  hlsyn
//
//  
//

#ifndef schedule_h
#define schedule_h

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "variables.h"
#include "common.h"
using namespace std;

class schedule {
private:
    string name;
    string type;
    int bitWidth;
    string issigned;
    string line;
    string dummy;
    vector<float>aluprob;//probabilities for ALU
    vector<float>mulprob;//probabilities for MUL
    vector<float>divmodprob;//probabilities for DIV/MOD
public:
    
    
    schedule();
    schedule(string name, string type);
    void alap(vector<Common> &mod,int latency);
    void asap(vector<Common> &mod);
    int haspredecessor(vector<Common>mod,string current);
    vector<int> hassuccessor(vector<Common>mod,string current);
    void probability(vector<Common>&mod,int latency);
    vector<float> selfForce(Common &tempmod, int start, int end);
    float successorForce(vector<Common>&mod,Common prenode, int scheduleTime);
    float predecessorForce(vector<Common>&mod,Common nextnode, int scheduleTime);
    void forceschedule(vector<Common>&mod,int latency);

    string getName();
    string getType();
    int getBitWidth();
    string getSigned();
    string getline();
    string getdummy();
    void setline(string line);
    void setname(string name);
    void setdummy(string dummy);
    void setsign(string issigned);
};



#endif /* schedule_hpp */
