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
    this->op_out = "";//ouput of a operation
    this->operation = "";//operation type
    this->datawidth = 0;
    this->issigned="";
    this->line="";//converted line
    this->op_in={};//inputs of a operation,could be more than 1 input,so it's a vector
    this->latency=0;
    this->timeFrame = {};
    this->timewidth=0;
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

        mod_vars[mod_varsNum] = new char[to_parse.find("=")];              //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("+") - to_parse.find("=")];        //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("+") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("+")];        //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("+") - 1),
                      (to_parse.find("+") + 1));
        
        
        string one=string(mod_vars[mod_varsNum + 2]);
        one.erase(remove(one.begin(), one.end(), ' '),one.end());//get the second input,if it's 1, it's INC operation
        if (one=="1")         //parse inc
        {
     
        temp=string(mod_vars[mod_varsNum]);//output in this operation
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
        setopout(temp);//set ouput for this module
        temp=string(mod_vars[mod_varsNum+1]);//first input in this operation
        temp.erase(remove(temp.begin(), temp.end(), ' '),
                   temp.end());
        this->op_in.push_back(temp);//set input for this module
        for(i=0;i<var.size();i++){
            if(op_out==var.at(i).getName()){//find the output variable
                if(var.at(i).getdummy()=="output"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                    setdatawidth(var.at(i).getBitWidth());
                    x=i;
                }
            }
            if(op_in.at(0)==(var.at(i).getName())){//find the first input variable
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                    y=i;
                }}}
        if(var.at(x).getSigned()=="signed"||var.at(y).getSigned()=="signed"){//module is signed if either the input or output is signed
            setsign("signed");//set sign for this module
            sign="S";
        }
        //make string
        
//        converted = sign+string("INC #(.DATAWIDTH(") +  to_string(this->datawidth) +
//        string(")) INC_") +  to_string(this->Common::INC_moduleNum) +
//        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
//        string(mod_vars[mod_varsNum]) + string(");");
        this->Common::INC_moduleNum++;
        mod_varsNum += 2;
        setoperation("INC");
        }
        else{//if the second input is not 1, it's an ADD
            mod_vars[mod_varsNum] = new char[to_parse.find("=")];                             //output variable
            to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
            mod_vars[mod_varsNum +1] = new char[to_parse.find("+") - to_parse.find("=")];        //first input variable
            to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("+") - to_parse.find("=") - 1),
                          (to_parse.find("=") + 1));
            mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("+")];        //second input variable
            to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("+") - 1),
                          (to_parse.find("+") + 1));
            
            getfromvar(var,mod_vars,mod_varsNum);//determine sign and bitwidth
            sign=(this->getSigned()=="signed")? "S":"";
            //make string
            converted = sign+string("ADD #(.DATAWIDTH(") + std::to_string(this->datawidth) +
            string(")) ADD_") + std::to_string(this->Common::ADD_moduleNum) +
            string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
            string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
            string(");");
            this->Common::ADD_moduleNum++;
            mod_varsNum += 3;
            setoperation("ADD");
        }
    }
    else if ((to_parse.find("-") != string::npos) && (to_parse.find("1") != string::npos))                //parse DEC,same procedure as INC
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];    //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("-") - to_parse.find("=")];    //input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("-") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
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
                }}}
        if(var.at(x).getSigned()=="signed"||var.at(y).getSigned()=="signed"){
            setsign("signed");
            sign="S";
        }
        //make string
        converted = sign+string("DEC #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) DEC_") + std::to_string(this->Common::DEC_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum]) + string(");");
        this->Common::DEC_moduleNum++;
        mod_varsNum += 2;
        setoperation("DEC");
    }
    else if ((to_parse.find("*") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                             //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("*") - to_parse.find("=")];        //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("*") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("*")];        //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("*") - 1),
                      (to_parse.find("*") + 1));
        
        getfromvar(var,mod_vars,mod_varsNum);//determine sign and bitwidth
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
        converted = sign+string("MUL #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) MUL_") + std::to_string(this->Common::MUL_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::MUL_moduleNum++;
        mod_varsNum += 3;
        setoperation("MUL");
    }
//    else if ((to_parse.find("+") != string::npos))
//    {
//        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                             //output variable
//        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
//        mod_vars[mod_varsNum +1] = new char[to_parse.find("+") - to_parse.find("=")];        //first input variable
//        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("+") - to_parse.find("=") - 1),
//                      (to_parse.find("=") + 1));
//        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("+")];        //second input variable
//        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("+") - 1),
//                      (to_parse.find("+") + 1));
//
//        getfromvar(var,mod_vars,mod_varsNum);
//        sign=(this->getSigned()=="signed")? "S":"";
//        //make string
//        converted = sign+string("ADD #(.DATAWIDTH(") + std::to_string(this->datawidth) +
//        string(")) ADD_") + std::to_string(this->Common::ADD_moduleNum) +
//        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
//        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
//        string(");");
//        this->Common::ADD_moduleNum++;
//        mod_varsNum += 3;
//        setoperation("ADD");
//    }
    else if ((to_parse.find("-") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                             //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("-") - to_parse.find("=")];        //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("-") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("-")];        //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("-") - 1),
                      (to_parse.find("-") + 1));
        
        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
        converted = sign+string("SUB #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) SUB_") + std::to_string(this->Common::SUB_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::SUB_moduleNum++;
        mod_varsNum += 3;
        setoperation("SUB");
    }
    else if ((to_parse.find(">>") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find(">>") - to_parse.find("=")];       //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find(">>") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find(">>")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find(">>") - 2),
                      (to_parse.find(">>") + 2));
        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
        temp=string(mod_vars[mod_varsNum + 1]);
        temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());//remove white space
        
        converted = sign+string("SHR #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) SHR_") + std::to_string(this->Common::SHR_moduleNum) +
        string("(") + temp + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::SHR_moduleNum++;
        mod_varsNum += 3;
        setoperation("SHR");
    }
    else if ((to_parse.find("<<") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                                 //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("<<") - to_parse.find("=")];           //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("<<") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("<<")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("<<") - 2),
                      (to_parse.find("<<") + 2));
        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
        converted = sign+string("SHL #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) SHL_") + std::to_string(this->Common::SHL_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::SHL_moduleNum++;
        mod_varsNum += 3;
        setoperation("SHL");
    }
    else if ((to_parse.find(">") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find(">") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find(">") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find(">")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find(">") - 1),
                      (to_parse.find(">") + 1));

        temp=string(mod_vars[mod_varsNum]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
        setopout(temp);//output varibale
        temp=string(mod_vars[mod_varsNum+1]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);//input 1
        temp=string(mod_vars[mod_varsNum+2]);
        temp.erase(remove(temp.begin(), temp.end(),' '),
                   temp.end());
        this->op_in.push_back(temp);//input 2
        for(i=0;i<var.size();i++){
            
            if(op_in.at(0)==(var.at(i).getName())){//input 1
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                       width1=var.at(i).getBitWidth();
                    x=i;
                }}
            if(op_in.at(1)==(var.at(i).getName())){//input 2
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                    y=i;
                     width2=var.at(i).getBitWidth();
                }}
        }  this->datawidth=max(width1,width2);//datawidth of COMP depends on the largest input width
        if(var.at(x).getSigned()=="signed"||var.at(y).getSigned()=="signed"){
            setsign("signed");
            sign="S";
        }
        
        //make string
        converted = sign+string("COMP #(.DATAWIDTH(") +  to_string(this->datawidth) +
        string(")) COMP_GT_") + std::to_string(this->Common::COMP_GT_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::COMP_GT_moduleNum++;
        mod_varsNum += 3;
        setoperation("COMP");
    }
    else if ((to_parse.find("<") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("<") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("<") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find(">")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("<") - 1),
                      (to_parse.find("<") + 1));
        
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
        if(var.at(x).getSigned()=="signed"||var.at(y).getSigned()=="signed"){
            setsign("signed");
            sign="S";
        }
        //make string
        converted = sign+string("COMP #(.DATAWIDTH(") +  to_string(this->datawidth) +
        string(")) COMP_LT_") + std::to_string(this->Common::COMP_LT_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::COMP_LT_moduleNum++;
        mod_varsNum += 3;
        setoperation("COMP");
    }
    else if ((to_parse.find("==") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("==") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("==") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("==")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("==") -2),
                      (to_parse.find("==") + 2));
        
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
        if(var.at(x).getSigned()=="signed"||var.at(y).getSigned()=="signed"){
            setsign("signed");
            sign="S";
        }
        //make string
        converted = sign+string("COMP #(.DATAWIDTH(") +  to_string(this->datawidth) +
        string(")) COMP_EQ_") + std::to_string(this->Common::COMP_EQ_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::COMP_EQ_moduleNum++;
        mod_varsNum += 3;
        setoperation("COMP");
    }
    else if ((to_parse.find("?") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("?") - to_parse.find("=")];       //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("?") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.find(":") - to_parse.find("?")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.find(":") - to_parse.find("?") - 1),
                      (to_parse.find("?") + 1));
        mod_vars[mod_varsNum + 3] = new char[to_parse.length() - to_parse.find(":")];       //third input variable
        to_parse.copy(mod_vars[mod_varsNum + 3], (to_parse.length() - to_parse.find(":")), to_parse.find(":") + 1);
        
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
        for(i=0;i<var.size();i++){
            if(op_out==var.at(i).getName()){
                if(var.at(i).getdummy()=="output"||var.at(i).getdummy()=="wire"){
                    setdatawidth(var.at(i).getBitWidth());
                    x=i;
                }}
            if(op_in.at(0)==(var.at(i).getName())){
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                    z=i;
                }}
            if(op_in.at(1)==(var.at(i).getName())){
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                    y=i;
                }}
            if(op_in.at(2)==(var.at(i).getName())){
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                    w=i;
                    
                }}
      }  //this->datawidth=max(width1,width2);
        if(var.at(x).getSigned()=="signed"||var.at(y).getSigned()=="signed"||var.at(z).getSigned()=="signed"||var.at(w).getSigned()=="signed"){
            setsign("signed");
            sign="S";
        }
        //make string
        converted = sign+string("MUX2X1 #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) MUX2X1_") + std::to_string(this->Common::MUX2X1_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum + 3]) +
        string(", ") + string(mod_vars[mod_varsNum]) + string(");");
        this->Common::MUX2X1_moduleNum++;
        mod_varsNum += 3;
        setoperation("MUX");
    }
    else if ((to_parse.find("/") != string::npos))
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("/") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("/") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("/")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("/") - 1),
                      (to_parse.find("/") + 1));
        
        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
        //make string
        converted = sign+string("DIV #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) DIV_") + std::to_string(this->Common::DIV_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::DIV_moduleNum++;
        mod_varsNum += 3;
        setoperation("DIV");
    }
    else if (to_parse.find("%") != string::npos)
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.find("%") - to_parse.find("=")];    //first input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.find("%") - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        mod_vars[mod_varsNum + 2] = new char[to_parse.length() - to_parse.find("%")];       //second input variable
        to_parse.copy(mod_vars[mod_varsNum + 2], (to_parse.length() - to_parse.find("%") - 1),
                      (to_parse.find("%") + 1));
        
        
        getfromvar(var,mod_vars,mod_varsNum);
        sign=(this->getSigned()=="signed")? "S":"";
       
        //make string
        converted = sign+string("MOD #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) MOD_") + std::to_string(this->Common::MOD_moduleNum) +
        string("(") + string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum + 2]) + string(", ") + string(mod_vars[mod_varsNum]) +
        string(");");
        this->Common::MOD_moduleNum++;
        mod_varsNum += 3;
        setoperation("MOD");
    }
    else
    {
        mod_vars[mod_varsNum] = new char[to_parse.find("=")];                         //output variable
        to_parse.copy(mod_vars[mod_varsNum], to_parse.find("="));
        mod_vars[mod_varsNum +1] = new char[to_parse.length() - to_parse.find("=")];    //input variable
        to_parse.copy(mod_vars[mod_varsNum +1], (to_parse.length() - to_parse.find("=") - 1),
                      (to_parse.find("=") + 1));
        
        temp=string(mod_vars[mod_varsNum]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
        setopout(temp);
        temp=string(mod_vars[mod_varsNum+1]);
        temp.erase(remove(temp.begin(), temp.end(), ' '),temp.end());
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
            }}}
        if(var.at(x).getSigned()=="signed"||var.at(y).getSigned()=="signed"){
            setsign("signed");
            sign="S";
        }
        
        //make string
        converted = sign+string("REG #(.DATAWIDTH(") + std::to_string(this->datawidth) +
        string(")) REG_") + std::to_string(this->Common::REG_moduleNum) +
        string("(") +string("clk, rst,")+ string(mod_vars[mod_varsNum + 1]) + string(", ") +
        string(mod_vars[mod_varsNum]) + string(");");
        this->Common::REG_moduleNum++;
        mod_varsNum += 2;
        setoperation("REG");
    }
    setline(to_parse);
    setdelay(calcpath(this->operation,this->datawidth));//get the delay from the table
   // op_in.clear();
}

}
//determine sign and bitwidth of each module,according to the info from variables
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
          //datawidth of the module depends on the output variable
            }
        }
        if(op_in.at(0)==(var.at(i).getName())){//input 1
            if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                y=int(i);
            }}
        if(op_in.at(1)==(var.at(i).getName())){//input 2
            if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"||var.at(i).getdummy()=="reg"){
                z=int(i);
            }}
    }
    if(var.at(z).getSigned()=="signed"||var.at(y).getSigned()=="signed"){
        setsign("signed");//either one input is signed, the module is signed
        //sign="S";
    }
}
double Common::calcpath(string op,int width){
    double latency;
    map<string, vector<double>> m;
    m["REG"] = { 2.616, 2.644, 2.879, 3.061, 3.602, 3.966 };
    m["ADD"] = { 2.704, 3.713, 4.924, 5.638, 7.270, 9.566 };
    m["SUB"] = { 3.024, 3.412, 4.890, 5.569, 7.253, 9.566 };
    m["MUL"] = { 2.438, 3.651, 7.453, 7.811, 12.395, 15.354, };
    m["COMP"] = { 3.031, 3.934, 5.949, 6.256, 7.264, 8.416 };
    m["MUX"] = { 4.083, 4.115, 4.815, 5.623, 8.079, 8.766 };
    m["SHR"] = { 3.644, 4.007, 5.178, 6.460, 8.819, 11.095 };
    m["SHL"] = { 3.614, 3.980, 5.152, 6.549, 8.565, 11.220 };
    m["DIV"] = { 0.619, 2.144, 15.439, 33.093, 86.312, 243.233 };
    m["MOD"] = { 0.758, 2.149, 16.078, 35.563, 88.142, 250.583 };
    m["INC"] = { 1.792, 2.218, 3.111, 3.471, 4.347, 6.200 };
    m["DEC"] = { 1.792, 2.218, 3.108, 3.701, 4.685, 6.503 };
    
    switch (width) {
        case 1:
            return m[op][0];
        case 2:
            return m[op][1];
        case 8:
            return m[op][2];
        case 16:
            return m[op][3];
        case 32:
            return m[op][4];
        case 64:
            return m[op][5];
        default:
            return 0;
    }
    
    
    return latency;
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
//vector of 3 integers,first is scheduled time in asap,second is scheduled time in alap,third is forced schedule time
{
    return this->timeFrame;
}
void Common::setTimeFrame(int edge){//
        timeFrame.push_back(edge);
}
