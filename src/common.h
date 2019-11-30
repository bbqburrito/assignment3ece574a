#include <iostream>
#include <string>
#include <list>
#include <vector>


class Operation
{
    public:
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
    void displayCDFG(int size);
    vector<vector<Operation>>getCDFG() const;
    
    
    
    protected:

    vector<vector<Operation>> CDFG;




};