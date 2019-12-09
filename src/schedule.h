//
//  schedule.hpp
//  hlsyn
//
//  
//

#ifndef schedule_h
#define schedule_h

#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "variables.h"
#include "common.h"
#include "CDFG_graph.h"
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
    CDFG_graph schedulerGraph;

    int navigateBranch(int path_length, Operation branch_at, 
                                vector<Operation>& to_end, vector<Operation> which_branch);
    
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
    vector<Operation> ifScheduler(Operation branch_at, int & total_path_length);
    int navigateBranch(int path_lenth, Operation branch_at, vector<Operation>& to_end);

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
