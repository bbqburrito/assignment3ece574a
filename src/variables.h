//
//  variables.hpp
//  dpgen
//
//  Created by wym123 on 2019/11/1.
//

#ifndef variables_h
#define variables_h

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class variables {
private:
    string name;
    string type;
    int bitWidth;
    string issigned;
    string line;
    string dummy;
    
public:
   
    
    variables();
    variables(string name, string type);
   vector<variables> parse(string input);
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

#endif /* variables_h */
