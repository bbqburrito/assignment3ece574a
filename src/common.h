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
    /*int get_REG_moduleNum() const;
     int get_INC_moduleNum() const;
     int get_DEC_moduleNum() const;
     int get_ADD_moduleNum() const;
     int get_SUB_moduleNum() const;
     int get_MUL_moduleNum() const;
     int get_COMP_GT_moduleNum() const;
     int get_COMP_LT_moduleNum() const;
     int get_COMP_EQ_moduleNum() const;
     int get_MUX2X1_moduleNum() const;
     int get_SHR_moduleNum() const;
     int get_SHL_moduleNum() const;
     int get_DIV_moduleNum() const;
     int get_MOD_moduleNum() const;
     void set_REG_moduleNum(int to_set);
     void set_INC_moduleNum(int to_set);
     void set_DEC_moduleNum(int to_set);
     void set_ADD_moduleNum(int to_set);
     void set_SUB_moduleNum(int to_set);
     void set_MUL_moduleNum(int to_set);
     void set_COMP_GT_moduleNum(int to_set);
     void set_COMP_LT_moduleNum(int to_set);
     void set_COMP_EQ_moduleNum(int to_set);
     void set_MUX2X1_moduleNum(int to_set);
     void set_SHR_moduleNum(int to_set);
     void set_SHL_moduleNum(int to_set);
     void set_DIV_moduleNum(int to_set);
     void set_MOD_moduleNum(int to_set);
     */
    
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
    int timewidth;
};
