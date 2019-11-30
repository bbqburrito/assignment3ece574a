#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>


using std::vector;


class Operation
{
    public:
    Operation();
    Operation(const Operation& to_copy);
    void operator = (const Operation& to_copy);
    ~Operation();
    inline int getType() const{
        return type;
    }
    inline vector<int> getInputs() const{
        return inputs;
    }
    inline int getOutput() const{
        return output;
    }
    inline int getCycles() const{
        return cycles;
    }
    inline void setType(int to_set){
        type = to_set;
    }
    inline void setInputs(vector<int> to_set){
        inputs = to_set;
    }
    inline void setOutput(int to_set){
        output = to_set;
    }
    inline void setCycles(int to_set){
        cycles = to_set;
    }
    void add_input(int to_add);
    void list_inputs() const;

    protected:
    int type;
    vector<int> inputs;
    int output;
    int cycles;

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
    inline vector<int> getVertices() const{
        return vertices;
    }
    inline void setVertices(vector<int> to_set){
        vertices = to_set;
    }
    inline void setCDFG(vector<vector<Operation>> to_set){
        CDFG = to_set;
    }
    
    
    
    protected:

    vector<vector<Operation>> CDFG;
    vector<int> vertices;





};