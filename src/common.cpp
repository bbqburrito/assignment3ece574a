
#include "common.h"
#include "variables.h"
#include "schedule.h"
#include "CDFG_graph.h"
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
    this->force={};
}
//to do: throw exceptions when empty and when any line cannot be converted to verilog
//evaluate whether to divide parsing routines into functions
vector<Common> Common::convert(vector<string> lines, vector<variables> var, int latency)
{unsigned int i;
    vector<Common> module;
    Common eachmod;
    
    string no_input = "//nothing";            //error message
    string converted;                       //contains return value

    string to_lines_ifparser;               //change vector<string> to string for if parser
    vector<vector<Common>> store_branches;
    vector<Common> hold_mod;

    for(auto it: lines)                     //prepare lines vector for ifparser
    {
        to_lines_ifparser += it + "\n";
    }
    //    if(lines.empty())                    //check for empty
    //    {
    //        return no_input;                    //to do: set error codes
    //    }
    for(i=0;i<lines.size();i++){
        // if(to_parse.find("=") != string::npos){
        if (lines.at(i).find("if") != string::npos)
        {
            hold_mod = ifparser(to_lines_ifparser, store_branches, var, i, latency);
            //i = ifparser(i, lines, module);//
        }
        else{
            eachmod=Common(lines.at(i), var);
            converted = eachmod.getline();//get converted line
            module.push_back(eachmod);//add each module into a vector
        }}
    return module;
  
}

//parses if statements and schedules them into Common vector
//recursively parses if statements inside if statement. Returns Common vector with 
//compares with else and parses if statements recursively with else statements
//throws "not if" if not an if statement. throws "not operation" if operation is not
//in correct format. Throws "illegal" if doesn't find else where expected
//throws "nothing found" if no if statement in passed string
vector<Common> Common::ifparser(string lines,  vector<vector<Common>>& store_brancnhes,
                                    vector<variables> var, int& long_branch_latency, int req_latency = 0)
{
    string not_if_error = "not if";
    string if_out = "if_out";
    stringstream string2;
    string next;
    stringstream content(lines);
    string branch;
    string if_dummy, parenthesis, if_var;//variable in condition
    string var_dummy, op_dummy, input_dummy; 
    unsigned int j = 0;
    string elsestate = "";
    vector<Common> make_module;
    vector<Common> to_branch;
    vector<vector<Common>> store_branches;
    Common to_module;
    vector<string> to_inputs;
    size_t it, it_next;
    vector<string> to_branches;
    string remaining;
    schedule toplevel;
    vector<Common> longest_branch;

   
    content >> if_dummy >> parenthesis >> if_var;

    if(if_dummy != "if")
    {
        throw not_if_error;
    }

    content.ignore(100, '\n');

    //place if statement at front of Operation vector - leave if statement out of vector
//   to_inputs.push_back(if_var);
//
//   content.ignore(100, '\n');
//
//    to_module.setoperation(if_dummy);
//
//    to_module.setopin(to_inputs);
//
//    to_module.setopout(if_out);
//
//    make_module.push_back(to_module);

    //check if an else statement or another if statement exists
    //if there is neither an if statement nor an else statement,
    //schedule code as is. Also checks for for statement.

    if((content.str().find("if") == string::npos) && 
            (content.str().find("else") == string::npos) &&
            (content.str().find("for") == string::npos))
    {
        //parse all modules and place in Common vector
        to_inputs.clear();
        to_module.clear();
        to_inputs.push_back(if_out);            //place if in inputs of first operation
        content >> var_dummy;

        while(var_dummy != "}")
        {
            content >> op_dummy;
            if(op_dummy != "=")
            {
                throw "not operation";
            }

            //put next line into string, then parse line
            std::getline(content, remaining);

            remaining = var_dummy + " " + op_dummy + " " + remaining;
            Common next_op(remaining, var);

            //add if_out to inputs if first operation in if statement
            if(!to_inputs.empty())            
            {
                to_inputs.clear();
                to_inputs = next_op.getopin();
                to_inputs.push_back(if_out);
            }

            to_inputs.clear();
            make_module.push_back(next_op);          //place parsed line into Common vector
            content >> var_dummy;
        }
    }

    //check for no ifs or fors but else statement
    else if((content.str().find("if") == string::npos) && 
                (content.str().find("else") != string::npos) &&
                (content.str().find("for") == string::npos))
    {
        //parse all modules and place in Common vector
        to_inputs.clear();
        to_module.clear();
        to_inputs.push_back(if_out);            //place if in inputs of first operation
        content >> var_dummy;

        while(var_dummy != "}")
        {
            content >> op_dummy;
            if(op_dummy != "=")
            {
                throw "not operation";
            }

            //put next line into string, then parse line
            std::getline(content, remaining);

            line = var_dummy + " " + op_dummy + " " + remaining;
            Common next_op(remaining, var);

            //add if_out to inputs if first operation in if statement
            if(!to_inputs.empty())            
            {
                to_inputs.clear();
                to_inputs = next_op.getopin();
                to_inputs.push_back(if_out);
            }

            to_inputs.clear();
            make_module.push_back(next_op);          //place parsed line into Common vector

            content >> var_dummy;
        }

        store_branches.push_back(make_module);
        make_module.clear();

        content.ignore(100, '\n');
        content >> if_dummy;

        if(if_dummy != "else")
        {
            throw "illegal";
        }

        content.ignore(256, '\n');

        to_module.clear();
        to_inputs.clear();
        to_inputs.push_back(if_out);            //place if in inputs of first operation
        
        while(var_dummy != "}")
        {
            content >> op_dummy;
            if(op_dummy != "=")
            {
                throw "not operation";
            }

            //put next line into string, then parse line
            std::getline(content, remaining);

            remaining = var_dummy + " " + op_dummy + " " + remaining;
            Common next_op(remaining, var);

            //add if_out to inputs if first operation in if statement
            if(!to_inputs.empty())            
            {
                to_inputs.clear();
                to_inputs = next_op.getopin();
                to_inputs.push_back(if_out);
            }

            to_inputs.clear();
            make_module.push_back(next_op);          //place parsed line into Common vector
            content >> var_dummy;
        }
        store_branches.push_back(make_module);
        make_module.clear();       
    }

    //check for ifs and no else but no for statement
    else if((content.str().find("if") == string::npos) && 
                (content.str().find("else") != string::npos) &&
                (content.str().find("for") != string::npos))
    {
        //parse all modules and place in Common vector
        to_inputs.clear();
        to_module.clear();
        to_branch.clear();
        to_inputs.push_back(if_out);            //place if in inputs of first operation
        content >> var_dummy;

        

        while(var_dummy != "}")
        {
            if((var_dummy == "if"))             //if line is if statement, parse recursively
            {
                remaining.clear();
                std::getline(content, remaining, '\0');
                remaining = var_dummy + " " + remaining;
                to_branch = ifparser(remaining, store_branches, var);
                //append to vector
                make_module.insert(make_module.end(), to_branch.begin(), to_branch.end());
            }

            content >> op_dummy;
            if(op_dummy != "=")
            {
                throw "not operation";
            }

            //put next line into string, then parse line
            std::getline(content, remaining);

            remaining = var_dummy + " " + op_dummy + " " + remaining;
            Common next_op(remaining, var);

            //add if_out to inputs if first operation in if statement
            if(!to_inputs.empty())            
            {
                to_inputs.clear();
                to_inputs = next_op.getopin();
                to_inputs.push_back(if_out);
            }

            to_inputs.clear();
            make_module.push_back(next_op);          //place parsed line into Common vector

            content >> var_dummy;
        }

        store_branches.push_back(make_module);
        make_module.clear();

        content.ignore(100, '\n');     
    }


    //check for ifs and else but no for statement
    else if((content.str().find("if") == string::npos) && 
                (content.str().find("else") == string::npos) &&
                (content.str().find("for") != string::npos))
    {
        //parse all modules and place in Common vector
        to_inputs.clear();
        to_module.clear();
        to_branch.clear();
        to_inputs.push_back(if_out);            //place if in inputs of first operation
        content >> var_dummy;

        

        while(var_dummy != "}")
        {
            if((var_dummy == "if"))             //if line is if statement, parse recursively
            {
                remaining.clear();
                std::getline(content, remaining, '\0');
                remaining = var_dummy + " " + remaining;
                to_branch = ifparser(remaining, store_branches, var);
                //append to vector
                make_module.insert(make_module.end(), to_branch.begin(), to_branch.end());
            }

            content >> op_dummy;
            if(op_dummy != "=")
            {
                throw "not operation";
            }

            //put next line into string, then parse line
            std::getline(content, remaining);

            remaining = var_dummy + " " + op_dummy + " " + remaining;
            Common next_op(remaining, var);

            //add if_out to inputs if first operation in if statement
            if(!to_inputs.empty())            
            {
                to_inputs.clear();
                to_inputs = next_op.getopin();
                to_inputs.push_back(if_out);
            }

            to_inputs.clear();
            make_module.push_back(next_op);          //place parsed line into Common vector

            content >> var_dummy;
        }

        store_branches.push_back(make_module);
        make_module.clear();

        content.ignore(100, '\n');
        content >> if_dummy;

        if(if_dummy != "else")
        {
            throw "illegal";
        }

        to_module.clear();
        to_inputs.clear();
        to_inputs.push_back(if_out);            //place if in inputs of first operation
        var_dummy = if_dummy;
        while(var_dummy != "}")
        {
            if((var_dummy == "if"))             //if line is if statement, parse recursively
            {
                remaining.clear();
                std::getline(content, remaining, '\0');
                remaining = var_dummy + " " + remaining;
                to_branch = ifparser(remaining, store_branches, var);
                //append to vector
                make_module.insert(make_module.end(), to_branch.begin(), to_branch.end());
            }

            content >> op_dummy;
            if(op_dummy != "=")
            {
                throw "not operation";
            }

            //put next line into string, then parse line
            std::getline(content, remaining);

            remaining = var_dummy + " " + op_dummy + " " + remaining;
            Common next_op(remaining, var);

            //add if_out to inputs if first operation in if statement
            if(!to_inputs.empty())            
            {
                to_inputs.clear();
                to_inputs = next_op.getopin();
                to_inputs.push_back(if_out);
            }

            to_inputs.clear();
            make_module.push_back(next_op);          //place parsed line into Common vector
            content >> var_dummy;
        }
        store_branches.push_back(make_module);
        make_module.clear();       
    }

    //if there is nothing in the Common 2d vector, throw "nothing founc"
    if(store_branches.empty())
    {
        throw "nothing found";
    }

    //schedule each branch
    for(auto it: store_branches)
    {
        for(auto jt: it)
        {
            toplevel.forceschedule(it, req_latency);
        }
    }

    //set longest branch to first branch
    longest_branch = store_branches.at(0);

    //get length of each branch
    for(auto it: store_branches)
    {
        //compare to temp branch, store longest branch in temp
        if((it.back().getTimeFrame().at(0)) > (longest_branch.back().getTimeFrame().at(0)))
        {
            longest_branch = it;
        }
    }

    long_branch_latency = longest_branch.back().getTimeFrame().at(0);

    return longest_branch;

    /*for (j ; j < lines.size(); j++) {
        stringstream string2(lines.at(j));
        string next;
        string2 >> next;
       // module.at(i).setline(next);//read each line in if statement
        if (next.compare("}") == 0) {//find the end of this if statement
            line_number = j + 1;//move to the next
            if (lines.size() > line_number) {
                stringstream string3(lines.at(line_number));
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
    }*/
    //return j;
    
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
            if(op_in.at(1)==(var.at(i).getName())){//input 2
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="wire"){
                    y=i;
                     width2=var.at(i).getBitWidth();
                }}
        }  this->datawidth=max(width1,width2);//datawidth of COMP depends on the largest input width
        
        //make string
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

string Common::getopout() const {
    return this->op_out;
}
string Common::getoperation() const {
    return this->operation;
}
int Common::getdatawidth() const {
    return this->datawidth;
    
}
int Common::gettimewidth() const {
    //this->timewidth=this->timeFrame.at(1)-this->timeFrame.at(0)+1;
    return this->timeFrame.at(1)-this->timeFrame.at(0)+1;
    
}

string Common::getSigned() const {
    return this->issigned;
}
string Common::getline() const{
    return this->line;
}
vector<string> Common::getopin() const {
    return this->op_in;
}
double Common::getlatency() const {
    return this->latency;
}
vector<int> Common::getTimeFrame() const
//vector of 3 integers,first is scheduled time in asap,second is scheduled time in alap,third is forced schedule time
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

void Common::clear()
{
    uFlag = 0;
    datawidth = 0;
    latency = 0;
    timewidth = 0;
    op_out.clear();
    issigned.clear();
    branches.clear();
    line.clear();
    op_in.clear();
    timeFrame.clear();
    force.clear();

}