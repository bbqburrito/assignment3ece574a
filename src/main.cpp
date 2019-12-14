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
#include "schedule.h"
//#include "common.h"
#include <map>
using namespace std;
//int errortest(vector<Common> mod,vector<variables> var){
//    unsigned int i;
//     unsigned int k;
//     unsigned int j;
//    int found;
//    for(i=0;i<mod.size();i++){
//        found=0;
//        for(k=0;k<mod.at(i).getopin().size();k++){
//            for(j=1;j<var.size();j++){
//                if(mod.at(i).getopin().at(k)==var.at(j).getName()){
//                    found=1;
//                    //continue;
//                }
//
//            }
////            if(found==0){
////                cout<<"Error file"<<endl;
////                return 0;
////            }
//        }
//        found=0;
//        for(j=1;j<var.size();j++){
//            if(mod.at(i).getopout()==var.at(j).getName()){
//                found=1;
//               // continue;
//            }
//
//       }
//
//    }
//    return found;
//}
int main(int argc, char* argv[]){
    //argv[1]="474a_circuit3.txt";//need to change it back!!!!!!!!!!!!!!!!!
    if (argc != 4)
    {
        
        cout << "Usage:\thlsyn cFile latency verilogFile" << endl;
        
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
     schedule toplevel;
    bool error=false;
    Common to_test;
    vector<Common> mod;
    unsigned int i;

    unsigned int j;
    unsigned int k;
     int found;
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
                //                netlist.seekg(0, ios::end);
                //                if (netlist.tellg() == 0) {
                //                    cout<<"Empty file"<<endl;
                //                }
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
                }
            }
            mod=to_test.convert(lines, var);//module parser
            
            //  mod.push_back(to_test);//mod is a vector of modules
            for(i=0;i<mod.size();i++){
                toplevel.asap(mod);
                if(stoi(argv[2])<mod.at(i).getTimeFrame().at(0)){
                    cout<<"Error: Latency is too small for this netlist"<<endl;
                    
                    return 0;
                }
            }
            netlist.close();
            /////////////////////determine error//////////////
//            if(errortest(mod, var)!=1){
//                cout<<"Error file"<<endl;
//                                                   return 0;
//            }
//                        for(i=0;i<mod.size();i++){
//                            found=0;
//                            for(k=0;k<mod.at(i).getopin().size();k++){
//                                for(j=1;j<var.size();j++){
//                                    if(mod.at(i).getopin().at(k)==var.at(j).getName()){
//                                        found=1;
//                                        continue;
//                                    }
//
//                                } if(found==0){
//                                        cout<<"Error file"<<endl;
//                                      return 0;
//                                    }
//                            }
//                            found=0;
//                            for(j=1;j<var.size();j++){
//                                if(mod.at(i).getopout()==var.at(j).getName()){
//                                    found=1;
//                                    continue;
//                               }
//
//                            }if(found==0){
//                                cout<<"Error file"<<endl;
//                                return 0;
//                            }
//                        }
            for(i=0;i<mod.size();i++){
                
                for(k=0;k<mod.at(i).getopin().size();k++){
                    found=0;
                    for(j=1;j<var.size();j++){
                        if(mod.at(i).getopin().at(k)==var.at(j).getName()){
                            found=1;
                            break;
                        }
//                        else{
//                            found=0;
//
//                        }
                    } if(found==0){
                        cout<<"Error file"<<endl;
                        return 0;
                    }
                }
                found=0;
                for(j=1;j<var.size();j++){
                    if(mod.at(i).getopout()==var.at(j).getName()){
                        found=1;
                        break;
                 }
                        //else{
//                        found=0;
//                    }
                }if(found==0){
                    cout<<"Error file"<<endl;
                    return 0;
                }
            }
            if(Contents.size()==0){
                 cout<<"Empty file"<<endl;
                return 0;
            }
        }
        else{
            cout<<"File doesn't exist"<<endl;
        }
        
    }
    
//////////////////////////////////////////////writing///////////////////////////////////////////////////
    ofstream verilog;
    char* outfile=argv[3];
    string circuit(outfile);
    unsigned int size;
    found=0;
    unsigned int numstate=latency;//for test
    if(outfile!=NULL&&error==false){
        verilog.open(outfile);
        if(verilog.is_open()){
            for(size=mod.size();size>0;size--){//asap schedule for everynode
               toplevel.asap(mod);
            }
            for(size=mod.size();size>0;size--){//alap schedule for everynode
                toplevel.alap(mod, latency);
            }
           for(i=0;i<mod.size();i++){
                toplevel.forceschedule(mod, latency);
           }
            verilog << "module HLSM" <<"("<<"clk,rst,Start,Done";
            for(i=0;i<var.size();i++){
                if(var.at(i).getdummy()=="input"||var.at(i).getdummy()=="output"){
                    verilog<<",";
                    verilog<<var.at(i).getName();// add all input/output variable,
                    //eg:module out1(a,b,c,d);
                    
                }
            }
            verilog<<");"<< endl;
            verilog<<"input clk,rst,Start;"<< endl;
            verilog << "output reg Done;" << endl;
            //            for(int i=0;i<var.size();i++){
            //                verilog << var.at(i).getline();}
            //write to file
            for(j=0;j<Contents.size();j++){//write all the converted line
                verilog << Contents.at(j)<<endl;
            }
            //            for(int i=0;i<mod.size();i++){
            //                       verilog << mod.at(i).getline()<<endl;}
            verilog << "parameter Wait=0,Final=0";
            for(i=1;i<=numstate;i++){
                verilog<<", S"<<i<<"="<<i;
                
            }
            verilog<<";"<<endl;
            verilog<<"reg [5:0] State, StateNext;"<<endl;
            verilog<<"always @(posedge clk) begin"<<endl;
            verilog<<"if (rst == 1) begin"<<endl;
          
            verilog<<"Done<=0;"<<endl;
            verilog<<"State <= Wait;"<<endl;
             verilog<<"end"<<endl;
            verilog<<"else begin"<<endl;
            verilog<<"State <= StateNext;"<<endl;
            verilog<<"end"<<endl;
             verilog<<"end"<<endl;
            verilog<<endl;
            verilog<<"always @(*) begin"<<endl;
             verilog<<"Done<=0;"<<endl;
            verilog<<"case (State)"<<endl;
       
            verilog<<"Wait: begin"<<endl;
            verilog<<"if(Start==1)"<<endl;
            verilog<<"StateNext <= S1;"<<endl;
            verilog<<"else"<<endl;
            verilog<<"StateNext <= Wait;"<<endl;
            verilog<<"end"<<endl;
            j=1;
            verilog<<"S"<<j<<": begin"<<endl;
            for(i=1;i<numstate;i++){
                for(k=0;k<mod.size();k++){
                    size=mod.at(k).getTimeFrame().at(0);
                    if (i == size) {
                        verilog << "\t" << mod.at(k).getline() << ";" << endl;
                    }
                    
                }
//                if(i<numstate){
                    verilog<<"StateNext <= S"<<i+1<<";"<<endl;
                                    verilog<<"end"<<endl;
                    verilog<<"S"<<i+1<<": begin"<<endl;
                
                   // j++;}
//                //print each node
//                //if node j scheduled at this time
//                for(k=0;k<mod.size();k++){
//                    if(mod.at(k).getTimeFrame().at(2)==i){
//                        verilog<<mod.at(k).getline();
//                        verilog<<endl;
//                        found=1;
//                    }
//                }
//                if (found ==1)
//                {
//          //verilog<<mod.at(j).getline();
//                    verilog<<"StateNext <= S"<<j+1<<";"<<endl;
//                    verilog<<"end"<<endl;
//                    verilog<<"S"<<j+1<<": begin"<<endl;
//                    j++;
//                }
//                // j++;
//                found =0;
            }
            // verilog<<"S"<<numstate<<": begin"<<endl;
            verilog<<"StateNext <= Final;"<<endl;
              verilog<<"end"<<endl;
              verilog<<"Final"<<": begin"<<endl;
            verilog << "Done <= 1;" << endl;
            verilog << "StateNext <= Wait;" << endl;
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
