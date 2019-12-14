#pragma once
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>


using std::vector;
using std::string;

//Operations are vertices of CDFG
class Operation
{
    public:
    Operation();
    Operation(int to_number, string to_type, vector<string> to_inputs, string to_output,
                    int to_cycles, int to_alap, int to_asap, vector<Operation> to_branches, 
                    vector<Operation> to_path);
    Operation(const Operation& to_copy);
    void operator= (const Operation& to_copy);
    inline bool operator== (const Operation& to_compare) const{   //inline == comparator. 
                                                            //checks Operation numbers to see if equal)
        return (to_compare.getNumber() == number);
    }
    inline bool operator!= (const Operation& to_compare) const{     //inline != operator
                                                            //returns conplement of == operator
        return(to_compare.getNumber() != number);
    }
    ~Operation();
    inline int getNumber() const{
        return number;
    }
    inline string getType() const{
        return type;
    }
    
    inline string getOutput() const{
        return output;
    }
    inline int getCycles() const{
        return cycles;
    }
    inline int getAlap() const{
        return alap;
    }
    inline int getAsap() const{
        return asap;
    }
    inline void setNumber(int to_set){
        number = to_set;
    }
    inline void setType(string to_set){
        type = to_set;
    }
    
    inline void setOutput(string to_set){
        output = to_set;
    }
    inline void setCycles(int to_set){
        cycles = to_set;
    }
    inline void setAlap(int to_set){
        alap = to_set;
    }
    inline void setAsap(int to_set){
        asap = to_set;
    }
    vector<string> getInputs() const;
    vector<Operation> getPath() const;
    vector<Operation> getBranches() const;
    void setInputs(vector<string> to_set);
    void setBranches(vector<Operation> to_set);
    void setPath(vector<Operation> to_set);
    void add_input(string to_add);
    void list_inputs() const;
    void clear();
    

    protected:
    int number;             // int identifier for operation node
    string type;               // type of operation 
    vector<string> inputs;  // inputs to the operation
    string output;             // outputs of the operation
    int cycles;             // number of cycles for operation. 
    int alap;               // alap schedule time for operation
    int asap;               // asap scedule time for operation
    vector<Operation> branches;    //if Operation is an if statement, holds next operations. blank otherwise
    vector<Operation> path;     //holds branches in Operation's path

};

class CDFG_graph
{
    public:
    CDFG_graph();
    CDFG_graph(const CDFG_graph& to_copy);
    void operator = (const CDFG_graph& to_copy);
    ~CDFG_graph();
    void add_edge(Operation from, Operation to);
    void displayCDFG() const;
    inline vector<vector<Operation>>getCDFG() const{
        return CDFG;
    }
    inline vector<Operation> getVertices() const{
        return vertices;
    }
    inline void setVertices(vector<Operation> to_set){
        vertices = to_set;
    }
    inline void setCDFG(vector<vector<Operation>> to_set){
        CDFG = to_set;
    }
    inline Operation getV0() const {
        return v0;
    }
    inline Operation getVn() const {
        return v_n;
    }
    inline void setV0(Operation to_set) {
        v0 = to_set;
    }
    inline void setVn(Operation to_set) {
        v_n = to_set;
    }
    inline int getNumber() const
    {
        return number;
    }
    inline void setNumber(int to_set) {
        number = to_set;
    }
    inline vector<vector<Operation>> getIfVectors() const   {
        return if_vectors;
    }
    inline void setIfVectors(vector<vector<Operation>> to_set)  {
        if_vectors = to_set;
    }
    
    void buildCDFG();
    void addVertex(Operation to_add);
    void displayVertices() const;
    void findIfVectors();
    
    
    
    protected:

    vector<vector<Operation>> CDFG;
    vector<vector<Operation>> if_vectors;
    vector<Operation> vertices;
    Operation v0;                   //begin null node - may not need
    Operation v_n;                  //end null node
    int number;                     //number of nodes on graph





};