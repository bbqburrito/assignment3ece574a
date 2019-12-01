//
//  variables.cpp
//  dpgen
//
//  Created by wym123 on 2019/11/1.
//

#include "variables.h"
variables::variables()
{
    this->name = "";
    this->type = "";
    this->bitWidth = 0;
    this->issigned="";
    this->line="";
     this->dummy="";
}
vector<variables> variables::parse(string input){
    stringstream str(input);
    vector<variables> var;
    variables temp;
    string name="";
    string type="";
    string dummy="";
   stringstream converted;
   // string converted;
    str >> dummy >> type;//read variable type
    if(dummy=="variable") dummy="reg";
    while (str >> name)
    {
        // if statement removes commas between inputs
        if ((!isalpha(name.back()))&&!isdigit(name.back()))
        {
            name = name.substr(0, name.length() - 1);
        }
        temp = variables(name,type);
        if(temp.getSigned()=="Unsigned"){
            temp.setsign("");
        }
        if(temp.getBitWidth()==1){//if it's 1 bit variable

            converted<<dummy<<" "<<temp.getSigned()<<" "<<name<<";"<<endl;
            temp.setname(name);
            temp.setdummy(dummy);
            temp.setline(converted.str());
           
               converted.str("");
                    }
        else if(dummy=="output"){// if more than 1 bit

            converted<<dummy<<" "<<"reg"<<" "<<temp.getSigned()<<" "<<"["<<(temp.getBitWidth() - 1)<< ":0] "<<name<<";";//converted line
            temp.setline(converted.str());//store the converted line
            temp.setdummy(dummy);//input/output
            temp.setname(name);
      
            converted.str("");//clear stringstream converted
        }
        else if(dummy=="reg"||dummy=="input"){
            converted<<dummy<<" "<<temp.getSigned()<<" "<<"["<<(temp.getBitWidth() - 1)<< ":0] "<<name<<";";//converted line
            temp.setline(converted.str());//store the converted line
            temp.setdummy(dummy);//input/output
            temp.setname(name);
            
            converted.str("");//clear stringstream converted
        }
        
        
        var.push_back(temp);//add each variable to a vector
    }
    return var;
}
variables::variables(string name, string type){
 

    string issigned;
    int bitwidth=0;
    string temp;

   
    if (type.find("UInt") != string::npos) {
       setsign("Unsigned");
       temp = type.substr(4);//
        bitwidth = stoi(temp);
     
    }
    else if (type.find("Int") != string::npos) {
        setsign("signed");
        temp = type.substr(3);
        bitwidth = stoi(temp);
    }

    this->type = type;
    this->bitWidth = bitwidth;
  
   // this->line=converted.str();

}
string variables::getName()
{
    return this->name;
}

string variables::getType()
{
    return this->type;
}

int variables::getBitWidth()
{
    return this->bitWidth;
}
string variables::getSigned()
{
    return this->issigned;
}
string variables::getline()
{
    return this->line;
}
string variables::getdummy()
{
    return this->dummy;
}
void variables::setline(string line){
    this->line=line;
}
void variables::setname(string name){
    this->name=name;
}
void variables::setdummy(string dummy){
    this->dummy=dummy;
}
void variables::setsign(string issigned){
    this->issigned=issigned;
}
