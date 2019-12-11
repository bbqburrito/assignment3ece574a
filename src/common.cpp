#include "common.h"
#include "variables.h"
//using  cout;
//using  endl;
//using  string;
using namespace std;

int Common::INC_moduleNum;
int Common::REG_moduleNum;
int Common::ADD_moduleNum;
int Common::SUB_moduleNum;
int Common::MUL_moduleNum;
int Common::COMP_GT_moduleNum;
int Common::COMP_LT_moduleNum;
int Common::COMP_EQ_moduleNum;
int Common::MUX2X1_moduleNum;
int Common::SHR_moduleNum;
int Common::SHL_moduleNum;
int Common::DIV_moduleNum;
int Common::MOD_moduleNum;
int Common::DEC_moduleNum;
Common::Common()
{
    this->op_out = "";
    this->operation = "";
    this->datawidth = 0;
    this->issigned="";
    this->line="";
    this->op_in={};
    this->latency=0;
    this->timeFrame = {};
    this->timewidth=0;
    this->force={};
}
//to do: throw exceptions when empty and when any line cannot be converted to verilog
//evaluate whether to divide parsing routines into functions
vector<Common> Common::convert(vector<string> lines, vector<variables> var)
{unsigned int i;
    vector<Common> module;
    Common eachmod;
    
    string no_input = "//nothing";            //error message
    string converted;                       //contains return value
    
    //    if(lines.empty())                    //check for empty
    //    {
    //        return no_input;                    //to do: set error codes
    //    }
    for(i=0;i<lines.size();i++){
        // if(to_parse.find("=") != string::npos){
        if (lines.at(i).find("if") != string::npos)
        {
            i = ifparser(i, lines, module);//
        }
        else{
            eachmod=Common(lines.at(i), var);
            converted = eachmod.getline();//get converted line
            module.push_back(eachmod);//add each module into a vector
        }}
    return module;
    
}
int Common::ifparser(int i,vector<string> lines,  vector<Common> module)
{
    stringstream content(lines.at(i));
    string if_dummy, parenthesis, if_var;//variable in condition
    unsigned int j;
    string elsestate = "";
    
    content >> if_dummy >> parenthesis >> if_var;
    for (j = i + 1; j < lines.size(); j++) {
        stringstream string2(lines.at(j));
        string next;
        string2 >> next;
        // module.at(i).setline(next);//read each line in if statement
        if (next.compare("}") == 0) {//find the end of this if statement
            i = j + 1;//move to the next
            if (lines.size() > i) {
                stringstream string3(lines.at(i));
                string3 >> elsestate;//if there's no more lines
            }
            else {
                stringstream string3(" ");//if there's a else statement
                string3 >> elsestate;
            }
            break;
        }
        else if (next.compare("\0") == 0 || next.substr(0, 2).compare("//") == 0) {
            // If it is a empty line or a comment
            continue;
        }
        else if (next.compare("if") == 0) {//another if statement
            j = ifparser(j, lines,module);
        }
        else
        {
            //add the contents inside if statement to module vector
        }
    }
    if (elsestate.compare("else") == 0) {
        for (j = i + 1; j < lines.size(); j++) {
            stringstream string2(lines.at(j));
            string next;
            string2 >> next;//read the contects in else statement
            if (next.compare("}") == 0) {
                break;
            }
            else if (next.compare("\0") == 0 || next.substr(0, 2).compare("//") == 0) {
                // If it is a empty line or a comment
                continue;
            }
            else if (next.compare("if") == 0) {//another if statement inside
                j = ifparser(j, lines, module);
            }
            else
            {
                //need to figure out a way to
                //add the contents inside if statement to module vector
                
            }
        }
    }
    return j;
    
}
//takes a string and and int as parameters, returns string
//parses string into verilog module statement, then returns that statement
 Common::Common(string to_parse,  vector<variables> var)
{ if(to_parse.find("=") != string::npos){
   // eachmod=Common(to_parse, var);
  //  converted = eachmod.getline();
   // module.push_back(eachmod);}
  //  int datawidth=0;
      unsigned  int width1=0;
     unsigned   int width2=0;
   unsigned int x=0,y=0,z=0,w=0;
    unsigned int i;
   // int test[100];
    string sign="";
    string temp;
    string converted;
    int mod_varsNum = 0;                    //to give unique array members if more than one
    //conditional is taken in the function. not necessary
    //and can be removed.
    char * mod_vars[150];                   //to store variable names from netlist functions
   // string s(mod_vars[100]);
    //test[0] = to_parse.find("+");
    //test[1] = to_parse.find("1");
    if ((to_parse.find("+") != string::npos))         //parse inc
    {
//        mod_vars[mod_varsNum] = new char[to_parse.find("=")];
//        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
//        mod_vars[mod_varsNum +1] = new char[to_parse.find("+") - to_parse.find("=")];
//        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("+") - to_parse.find("=") - 1),
//                      (to_parse.find("=") + 1));
	
        mod_vars[mod_varsNum] = new char[to_parse.find("=") + 1];                             //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
	    mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("+") - to_parse.find("=")];        //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("+") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("+") - to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("+")];        //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("+") - 1),
                      (to_parse.find("+") + 1));
	    mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("+") - 1] = '\0';
        
        
        string one=string(mod_vars[mod_varsNum + 2]);
        one.erase(remove(one.begin(), one.end(), ' '),one.end());
        if (one=="1")         //parse inc
        {
        /*this->op_input=mod_varsNum+1;
         this->op_output=mod_varsNum;
         if(not found in var,error code101)
         */
            temp=string(mod_vars[mod_varsNum]);
            temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
            setopout(temp);
            temp=string(mod_vars[mod_varsNum+1]);
            temp.erase(remove(temp.begin(), temp.end(), ' '),
                   temp.end());
            this->op_in.push_back(temp);
            for(i=0;i<var.size();i++){
                if(op_out==var.at(i).getName()){
                    if(var.at(i).getdummy()=="output"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                            setdatawidth(var.at(i).getBitWidth());
                        x=i;
                    }
                }
                if(op_in.at(0)==(var.at(i).getName())){
                    if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                        y=i;
                    }
                }
            }
            
            //make string
     
            this->Common::INC_moduleNum++;
            mod_varsNum += 2;
            setoperation("INC");
        }
        else {
            mod_vars[mod_varsNum] = new char[to_parse.find("=")];                             //output variable
            to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
            mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
            mod_vars[mod_varsNum +1] = new char[to_parse.find("+") - to_parse.find("=")];        //first input variable            
            to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("+") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
            mod_vars[mod_varsNum + 1][to_parse.find("+") - to_parse.find("=") - 1] = '\0';
            mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("+")];        //second input variable            
            to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("+") - 1),
                      (to_parse.find("+") + 1));
            mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("+") - 1] = '\0';
            
            getfromvar(var,mod_vars,mod_varsNum);
            sign=(this->getSigned()=="signed")? "S":"";
            //make string
            
            this->Common::ADD_moduleNum++;
            mod_varsNum += 3;            
            setoperation("ADD");
        }
    }
    else if ((to_parse.find("-") != string::npos))                //parse DEC
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("-") - to_parse.find("=") - 1];    //input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("-") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("-") - to_parse.find("=") - 1] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("-")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("+") - 1),
                      (to_parse.find("+") + 1));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("+") - 1] = '\0';

        string one=string(mod_vars[mod_varsNum + 2]);
        one.erase(remove(one.begin(), one.end(), ' '),one.end());
        if (one=="1")         //parse dec
        {
            temp=string(mod_vars[mod_varsNum]);
            temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
            setopout(temp);
            temp=string(mod_vars[mod_varsNum+1]);
            temp.erase(remove(temp.begin(), temp.end(), ' '),
                   temp.end());
            this->op_in.push_back(temp);
            for(i=0;i<var.size();i++){
                if(op_out==var.at(i).getName()){
                    if(var.at(i).getdummy()=="output"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                            setdatawidth(var.at(i).getBitWidth());
                        x=i;
                    }
                }
                if(op_in.at(0)==(var.at(i).getName())){
                    if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                            y=i;
                    }
                }
            }
           
            mod_varsNum += 2;
            setoperation("DEC");
        }
        else {
            mod_vars[mod_varsNum] = new char[to_parse.find("=")];                             //output variable
            to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
            mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
            mod_vars[mod_varsNum +1] = new char[to_parse.find("-") - to_parse.find("=")];        //first input variable            
            to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("-") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
            mod_vars[mod_varsNum + 1][to_parse.find("-") - to_parse.find("=") - 1] = '\0';
            mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("-")];        //second input variable            
            to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("-") - 1),
                      (to_parse.find("-") + 1));
            mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("-") - 1] = '\0';
            
            getfromvar(var,mod_vars,mod_varsNum);
            
            this->Common::ADD_moduleNum++;
            mod_varsNum += 3;            
            setoperation("SUB");
        }
    }
    else if ((to_parse.find("*") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                             //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("*") - to_parse.find("=")];        //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("*") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("*") - to_parse.find("=") - 1] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("*")];        //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("*") - 1),
                      (to_parse.find("*") + 1));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("*") - 1] = '\0';
        
        getfromvar(var,mod_vars,mod_varsNum);
        
        this->Common::MUL_moduleNum++;
        mod_varsNum += 3;
        setoperation("MUL");
    }

    else if ((to_parse.find(">>") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find(">>") - to_parse.find("=") - 1];       //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find(">>") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find(">>") - to_parse.find("=") - 2] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find(">>") - 1];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find(">>") - 1),
                      (to_parse.find(">>") + 2));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find(">>") - 2] = '\0';

        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
        temp=string(mod_vars[mod_varsNum + 1]);
        temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
        
        
        this->Common::SHR_moduleNum++;
        mod_varsNum += 3;
        setoperation("SHR");
    }
    else if ((to_parse.find("<<") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                                 //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("<<") - to_parse.find("=") - 1];           //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("<<") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("<<") - to_parse.find("=") - 2] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("<<") - 1];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("<<") - 1),
                      (to_parse.find("<<") + 2));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("<<") - 2] = '\0';

        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
        
        this->Common::SHL_moduleNum++;
        mod_varsNum += 3;
        setoperation("SHL");
    }
    else if ((to_parse.find(">") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find(">") - to_parse.find("=") - 1];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find(">") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find(">") - to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find(">")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find(">") - 1),
                      (to_parse.find(">") + 1));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find(">") - 1] = '\0';

        temp=string(mod_vars[mod_varsNum]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
        setopout(temp);
        temp=string(mod_vars[mod_varsNum+1]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        temp=string(mod_vars[mod_varsNum+2]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        for(i=0;i<var.size();i++){
            
            if(op_in.at(0)==(var.at(i).getName())){
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                       width1=var.at(i).getBitWidth();
                    x=i;
                }}
            if(op_in.at(1)==(var.at(i).getName())){
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                    y=i;
                     width2=var.at(i).getBitWidth();
                }}
        }  this->datawidth=max(width1,width2);
        
        this->Common::COMP_GT_moduleNum++;
        mod_varsNum += 3;
        setoperation("COMP");
    }
    else if ((to_parse.find("<") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("<") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("<") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("<") - to_parse.find("=") - 1] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find(">")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("<") - 1),
                      (to_parse.find("<") + 1));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("<") - 1] = '\0';
        
        temp=string(mod_vars[mod_varsNum]);
        temp.erase(remove(temp.begin(), temp.end(),' '),temp.end());
        setopout(temp);
        temp=string(mod_vars[mod_varsNum+1]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        temp=string(mod_vars[mod_varsNum+2]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        
       
        this->Common::COMP_LT_moduleNum++;
        mod_varsNum += 3;
        setoperation("COMP");
    }
    else if ((to_parse.find("==") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("==") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("==") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("==") - to_parse.find("=") - 1] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("==")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("==") - 2),
                      (to_parse.find("==") + 2));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("==") - 2] = '\0';
        
        temp=string(mod_vars[mod_varsNum]);
        temp.erase(remove(temp.begin(), temp.end(),' '),temp.end());
        setopout(temp);
        temp=string(mod_vars[mod_varsNum+1]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        temp=string(mod_vars[mod_varsNum+2]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),
                   temp.end());
        this->op_in.push_back(temp);
        
        this->Common::COMP_EQ_moduleNum++;
        mod_varsNum += 3;
        setoperation("COMP");
    }
    else if ((to_parse.find("?") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("?") - to_parse.find("=")];       //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("?") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("?") - to_parse.find("=") - 1] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.find(":") - to_parse.find("?")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.find(":") - to_parse.find("?") - 1),
                      (to_parse.find("?") + 1));
        mod_vars[mod_varsNum + 2][to_parse.find(":") - to_parse.find("?") - 1] = '\0';
        mod_vars[mod_varsNum + 3] = new char[to_parse.length() - to_parse.find(":")];       //third input variable
        to_parse.copy(mod_vars[mod_varsNum + 3], (to_parse.length() - to_parse.find(":") - 1), to_parse.find(":") + 1);
        mod_vars[mod_varsNum + 3][to_parse.length() - to_parse.find(":") - 1] = '\0';
        
        temp=string(mod_vars[mod_varsNum]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
        setopout(temp);
        temp=string(mod_vars[mod_varsNum+1]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        temp=string(mod_vars[mod_varsNum+2]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        temp=string(mod_vars[mod_varsNum+3]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);
        
        this->Common::MUX2X1_moduleNum++;
        mod_varsNum += 3;
        setoperation("MUX");
    }
    else if ((to_parse.find("/") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("/") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("/") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("/") - to_parse.find("=") - 1] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("/")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("/") - 1),
                      (to_parse.find("/") + 1));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("/") - 1] = '\0';
        
        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
       
        this->Common::DIV_moduleNum++;
        mod_varsNum += 3;
        setoperation("DIV");
    }
    else if (to_parse.find("%") != string::npos)
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.find("%") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("%") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.find("%") - to_parse.find("=") - 1] = '\0';
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("%")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("%") - 1),
                      (to_parse.find("%") + 1));
        mod_vars[mod_varsNum + 2][to_parse.length() - to_parse.find("%") - 1] = '\0';
        
        
        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
       
        //make string
       
        this->Common::MOD_moduleNum++;
        mod_varsNum += 3;
        setoperation("MOD");
    }
    else
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum][to_parse.find("=")] = '\0';
        mod_vars[mod_varsNum +1] = new char[to_parse.length() - to_parse.find("=")];    //input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.length() - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 1][to_parse.length() - to_parse.find("=") - 1] = '\0';
        
        temp=string(mod_vars[mod_varsNum]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
        setopout(temp);
        temp=string(mod_vars[mod_varsNum+1]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
        this->op_in.push_back(temp);
        
        
        //make string
       
        mod_varsNum += 2;
        setoperation("REG");
    }
    setline(to_parse);
   // setdelay(calcpath(this->operation,this->datawidth));
   // op_in.clear();
}
//    return converted;
}
void Common::getfromvar(vector<variables> var,char* str[150],int index){
   string temp;
  int y=0,z=0;
    unsigned int i=0;
    temp=string(str[index]);
    temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
    setopout(temp);//store ouput variable in this operation
    temp=string(str[index+1]);
    temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
    this->op_in.push_back(temp);//store input variables in this operation
    temp=string(str[index+2]);
    temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
    this->op_in.push_back(temp);
    
    for(i=0;i<var.size();i++){
        if(op_out==var.at(i).getName()){
            if(var.at(i).getdummy()=="output"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                setdatawidth(var.at(i).getBitWidth());
           //x=int(i);
            }
        }
        if(op_in.at(0)==(var.at(i).getName())){
            if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                y=int(i);
            }}
        if(op_in.at(1)==(var.at(i).getName())){
            if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                z=int(i);
            }}
    }
    if(var.at(z).getSigned()=="signed"||var.at(y).getSigned()=="signed"){
        setsign("signed");
        //sign="S";
    }
}

void Common::setline(string line){
    this->line=line;
}
void Common::setopout(string op_out){
    this->op_out=op_out;
}
void Common::setoperation(string operation){
    this->operation=operation;
}
void Common::setsign(string issigned){
    this->issigned=issigned;
}
void Common::setdatawidth(int datawidth){
    this->datawidth=datawidth;
}
void Common::setdelay(double delay){
    this->latency=delay;
}

string Common::getopout(){
    return this->op_out;
}
string Common::getoperation(){
    return this->operation;
}
int Common::getdatawidth(){
    return this->datawidth;
    
}
int Common::gettimewidth(){
    this->timewidth=this->timeFrame.at(1)-this->timeFrame.at(0)+1;
    return this->timewidth;
    
}

string Common::getSigned(){
    return this->issigned;
}
string Common::getline(){
    return this->line;
}
vector<string> Common::getopin(){
    return this->op_in;
}
double Common::getlatency(){
    return this->latency;
}
vector<int> Common::getTimeFrame()
//vector of 2 integers,first is scheduled time in asap,second is scheduled time in alap,third is forced schedule time
{
    return this->timeFrame;
}
void Common::setTimeFrame(int edge){//
    timeFrame.push_back(edge);
}
void Common::setforce(float forces){//
   force.push_back(forces);
}
void Common::updateAlap(int time){
    this->timeFrame.at(1)=time;
}
void Common::updateAsap(int time){
    this->timeFrame.at(0)=time;

}
