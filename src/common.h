/////////////////////////////////////////////////////
///                                               ///
///                                               ///
///  defines common class                         ///
///                                               ///
///                                               ///
/////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
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
    int ifparser(int i,vector<string> lines, vector<Common> module);
    string parse_variables(string to_convert, int datwidth);
    void getfromvar(vector<variables> var,char* str[100],int index);
    double calcpath(string op,int width);//calculate criticalpath
    string getopout();
    string getoperation();
    int getdatawidth();
    int gettimewidth();//get scheduled time width
    string getSigned();
    string getline();
    double getlatency();
    void setdelay(double delay);
    vector<int> getTimeFrame();
    void setTimeFrame(int edge);
    //void setoperation(string operation);
    void setoperation(string operation);
    vector<string> getopin();
    void setline(string line);
    void setopout(string op_out);
    void setsign(string issigned);
    void setdatawidth(int datawidth);
    void setopin(vector<string> op_in);//input in each operation
    void setforce(float forces);
    void updateAlap(int time);
    void updateAsap(int time);

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
    int datawidth;
    string line;
    double latency;
    vector<string> op_in;
    vector<int> timeFrame;
    //vector of 2 integers,first is scheduled time in asap,second is scheduled time in alap,third is forced schedule time
    int timewidth;
    vector<float> force;
};
