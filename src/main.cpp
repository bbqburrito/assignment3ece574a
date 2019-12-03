//
//  main.cpp
//  
//
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "variables.h"
#include "common.h"
#include <map>
using namespace std;
vector<double> findit(int index, vector<string>vs, vector<Common> mod, vector<variables> var,double templatency, vector<double> path);
int main(int argc, char* argv[]){
    //argv[1]="474a_circuit3.txt";//need to change it back!!!!!!!!!!!!!!!!!
    if (argc != 4)
    {
        
        cout << "Usage:\thlsyn cFile latency verilogFile" << endl;
//        cout << "\tcFile - The  netlist you wish to convert." << endl;
//        cout << "\tlatency - How long the graph has to be scheduled." << endl;
//        cout << "\tverilogFile - The path to for the output verilogFile" << endl;
        return 0;
    }
    int latency;
    try
    {
        latency = stoi(argv[2]);
    }
    catch (invalid_argument&)
    {
        cout << "Latency should be an integer, you entered: " << argv[2] << endl;
        return 0;
    }

    string line;
    ifstream netlist;
    vector <string> Contents;//contents are writing into files
   // string convertvar;
    char* filename=argv[1];
    //variables *var=new variables;
    vector<variables> var;
    variables convertvar;
    var.push_back(convertvar);
    bool error=false;
    Common to_test;
    vector<Common> mod;
    unsigned int i;
     vector<string> lines;//store all the module lines together,then parse them one by one(needed for if parser)
    if(filename!=NULL){
    netlist.open(argv[1]);
        if(netlist.is_open()){//validating we can open the file
            while(!netlist.eof()){
                getline(netlist,line);
                if((line.find("//")!= string::npos)||(line.find("$")!= string::npos)){
                    //detect error file
                    cout<<"error in netlist"<<endl;
                    error=true;
                    return EXIT_FAILURE;
                }
                for (i=0;i<convertvar.parse(line).size();i++){
                    //parse variable when we read each line, since there might be several variables initialized in the same line, parse function will return a vector<variables>
            if((convertvar.parse(line).at(i).getdummy()==("input"))||(convertvar.parse(line).at(i).getdummy()==("output"))||(convertvar.parse(line).at(i).getdummy()==("wire"))||(convertvar.parse(line).at(i).getdummy()==("reg"))){
                   
               //determine it's a variable declaration, converting the variables first
                      var.push_back(convertvar.parse(line).at(i));
                  //add ith variable into a vector<variables> named var;
                       Contents.push_back(convertvar.parse(line).at(i).getline());
                    //add the converted line in to a vector <string> named Contents;
                    }
                }
                    //             start converting modules//////////////////////////
            //identify that the line we read is a module, not variable anymore
                if(line.find("=") != string::npos){
                    lines.push_back(line);
//                 to_test=Common(line, var);//module parser
               // Contents.push_back(to_test.getline());//add the converted line into Contents;
//                    mod.push_back(to_test);//mod is a vector of modules
                    
                }
                    }
            mod=to_test.convert(lines, var);//module parser
     
          //  mod.push_back(to_test);//mod is a vector of modules

             netlist.close();
        }
    }
    if (Contents.size() == 0) {
        cout << "Cannot open file or empty file: " << argv[1] << endl;

    }

//////////////////////////////////////////////writing///////////////////////////////////////////////
    ofstream verilog;
    char* outfile=argv[3];
    string circuit(outfile);
    unsigned int j;
        unsigned int numstate=5;
    if(outfile!=NULL&&error==false){
        verilog.open(outfile);
        if(verilog.is_open()){
         for(i=0;i<mod.size();i++){
                toplevel.forceschedule(mod, latency);//apply force schedule for each node
                
            }
           
            verilog << "module HLSM" <<"("<<"clk,rst";
            for(i=0;i<var.size();i++){
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="output"){
                verilog<<",";
                    verilog<<var.at(i).getName();// add all input/output variable,
                    //eg:module out1(a,b,c,d);
                    
                }
            }
            verilog<<");"<< endl;
            verilog<<"input clk,rst;"<< endl;
            verilog << "output reg Done;" << endl;
//            for(int i=0;i<var.size();i++){
//                verilog << var.at(i).getline();}
        //write to file
            for(j=0;j<Contents.size();j++){//write all the converted line
                verilog << Contents.at(j)<<endl;
            }
//            for(int i=0;i<mod.size();i++){
//                       verilog << mod.at(i).getline()<<endl;}
            verilog << "parameter S0=0";
            for(i=1;i<numstate;i++){
                verilog<<", S"<<i<<"="<<i;
                
            }
            verilog<<";"<<endl;
            verilog<<"reg [3:0] State, StateNext;"<<endl;
            verilog<<"always @(posedge Clk) begin"<<endl;
            verilog<<"if (Rst == 1)"<<endl;
            verilog<<"Start<=0;"<<endl;
            verilog<<"Done<=0;"<<endl;
            verilog<<"State <= S0;"<<endl;
            verilog<<"else"<<endl;
            verilog<<"State <= StateNext;"<<endl;
            verilog<<"end"<<endl;
            verilog<<endl;
            verilog<<"always @(*) begin"<<endl;
            verilog<<"case (State)"<<endl;
            verilog<<"Done<=0;"<<endl;
            verilog<<"S0: begin"<<endl;
            verilog<<"if(Start==1)"<<endl;
            verilog<<"StateNext <= S1;"<<endl;
            verilog<<"else"<<endl;
            verilog<<"StateNext <= S0;"<<endl;
            verilog<<"end"<<endl;
            for(i=1;i<numstate-1;i++){
                verilog<<"S"<<i<<": begin"<<endl;
                //print each node
                //if node j scheduled at this time
                //verilog<<mod.at(j).getline();
                verilog<<"StateNext <= S"<<i+1<<";"<<endl;
                verilog<<"end"<<endl;
            }
             verilog<<"S"<<numstate-1<<": begin"<<endl;
            verilog << "Done <= 1;" << endl;
            verilog << "StateNext <= S0;" << endl;
              verilog<<"end"<<endl;
              verilog << "endcase" << endl;
            
            verilog << "end" << endl;
            
            verilog << "\nendmodule " << endl << endl;

         }
         else{
               cout<<"Coudn't open file."<<endl;
         }
     }
    verilog.close();

}
