/////////////////////////////////////////////////////
///                                               ///
///                                               ///
///  defines common class                         ///
///                                               ///
///                                               ///
/////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
#include "CDFG_graph.h"
#include "variables.h"
#include <sstream>
using std::string;
using namespace std;
//class Module        //not used unless we find a need for something to contain
////more than one variable
//{
//public:
//    int label = 0;
//};
class variables;

class Common {
    //need to instantiate constructor, destructor, copy constructor, and operator= override
    //if we use dynamic memory
public:
    //Common();
    //~Common();
    //explicit Common(const Common &obj);
    //Common operator = (Common const &obj);
    Common();
    vector<Common> convert(vector<string> to_parse, vector<variables> var);
    Common(string to_parse, vector<variables> var);
    inline void operator= (const Common &to_assign) {
        op_out = to_assign.getopout();
        issigned = to_assign.getSigned();
        operation = to_assign.getoperation();
        branches = to_assign.getBranches();
        datawidth = to_assign.getdatawidth();
        line = to_assign.getline();
        latency = to_assign.getlatency();
        op_in = to_assign.getopin();
        timeFrame = to_assign.getTimeFrame();
        timewidth = to_assign.gettimewidth();
        force = to_assign.getForce();
    }
    vector<Common> ifparser(string lines, vector<Common>& module, vector<variables> var, 
                                int level = 0);
    string parse_variables(string to_convert, int datwidth);
    void getfromvar(vector<variables> var,char* str[100],int index);
    double calcpath(string op,int width);//calculate criticalpath
    string getopout() const;
    string getoperation() const;
    int getdatawidth() const;
    int gettimewidth() const;//get scheduled time width
    string getSigned() const;
    string getline() const;
    double getlatency() const;
    inline vector<float> getForce() const {
        return force;
    }
    void setdelay(double delay);
    vector<int> getTimeFrame() const;
    void setTimeFrame(int edge);
    //void setoperation(string operation);
    void setoperation(string operation);
    vector<string> getopin() const;
    void setline(string line);
    void setopout(string op_out);
    void setsign(string issigned);
    void setdatawidth(int datawidth);
    void setopin(vector<string> op_in);//input in each operation
    void setforce(float forces);
    void updateAlap(int time);
    void updateAsap(int time);
    inline vector<string> getBranches() const {
        return branches;
    }
    inline void setBranches(vector<string> to_set) {
        branches = to_set;
    }
    vector<Common> elseparser();
    void clear();               //clears all members
    void parse_operation(vector<Common>& parsed, string to_parse, 
                            vector<string> to_inputs);


private:
    static int REG_moduleNum;
    static int INC_moduleNum;
    static int ADD_moduleNum;
    static int SUB_moduleNum;
    static int MUL_moduleNum;
    static int COMP_GT_moduleNum;
    static int COMP_LT_moduleNum;
    static int COMP_EQ_moduleNum;
    static int MUX2X1_moduleNum;
    static int SHR_moduleNum;
    static int SHL_moduleNum;
    static int DIV_moduleNum;
    static int MOD_moduleNum;
    static int DEC_moduleNum;
    int uFlag;
    string op_out;
    string issigned;
    string operation;
    vector<string> branches;        //hold branches of if statement in module's path
    int datawidth;
    string line;
    double latency;
    vector<string> op_in;
    vector<int> timeFrame;
    //vector of 2 integers,first is scheduled time in asap,second is scheduled time in alap,third is forced schedule time
    int timewidth;
    vector<float> force;
};
