#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>


using std::vector;

//Operations are vertices of CDFG
class Operation
{
    public:
    Operation();
    Operation(const Operation& to_copy);
    void operator= (const Operation& to_copy);
    inline bool operator== (const Operation& to_compare){   //inline == comparator. 
                                                            //checks Operation numbers to see if equal)
        return (to_compare.getNumber() == number);
    }
    ~Operation();
    inline int getNumber() const{
        return number;
    }
    inline int getType() const{
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
    inline void setType(int to_set){
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
    void setInputs(vector<string> to_set);
    void add_input(string to_add);
    void list_inputs() const;

    protected:
    int number;             // int identifier for operation node
    int type;               // type of operation - type is number of cycles for operation
    vector<string> inputs;  // inputs to the operation
    string output;             // outputs of the operation
    int cycles;             // number of cycles for operation. same as type, so not used
    int alap;               // alap schedule time for operation
    int asap;               // asap scedule time for operation

};

class Common
{
    public:
    Common();
    Common(const Common& to_copy);
    void operator = (const Common& to_copy);
    ~Common();
    void add_edge(Operation u, Operation v);
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
    void addRow();
    void buildCDFG();
    void addVertex(Operation to_add);
    
    
    
    protected:

    vector<vector<Operation>> CDFG;
    vector<Operation> vertices;





};