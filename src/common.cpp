#include "common.h"

using namespace std;
using std::string;

Operation::Operation():number(0), type(0), inputs {}, output(0), cycles(0), alap(0), asap(0) {}

Operation::Operation(const Operation& to_copy)
{
    type = to_copy.getType();
    inputs = to_copy.getInputs();
    output = to_copy.getOutput();
    cycles = to_copy.getCycles();
    alap = to_copy.getAlap();
    asap = to_copy.getAsap();
}

void Operation::operator = (const Operation& to_copy)
{
    type = to_copy.getType();
    inputs = to_copy.getInputs();
    output = to_copy.getOutput();
    cycles = to_copy.getCycles();
    alap = to_copy.getAlap();
    asap = to_copy.getAsap();
}

void Operation::add_input(int to_add)
{
    inputs.push_back(to_add);
}

void Operation::list_inputs() const
{
    for(auto i: inputs)
    {
        cout << i << endl;
    }
}

Common::Common():CDFG{}, vertices{} {}

Common::Common(const Common& to_copy)
{
    CDFG = to_copy.getCDFG();
    vertices = to_copy.getVertices();
}

void Common::operator = (const Common& to_copy)
{
    CDFG = to_copy.getCDFG();
    vertices = to_copy.getVertices();
}

void Common::add_edge(Operation u, Operation v)
{
    bool exists = false;        //temp to test whether need to create new row
    vector<Operation> to_row;   //temp to create new row
    //place first element adjacency list
    for(auto i: CDFG)
    {
        if(i.front() == u)  //add v to adjacency list for u
        {
            exists = true;
            i.push_back(v);
        }
        
    }
    //if not in graph, add to graph
    if(!exists)
    {
        to_row.push_back(u);
        to_row.push_back(v);
        CDFG.push_back(to_row);
    }

    //reset temporary variables for next element
    exists = false;
    to_row.erase(to_row.begin(), to_row.end());
    //place next element adjacency list
    for(auto i: CDFG)
    {
        if(i.front() == v)  //add u to adjacency list for vkk
        {
            exists = true;
            i.push_back(u);
        }
    }
    //if not in graph, add to graph
    if(!exists)
    {
        to_row.push_back(v);
        to_row.push_back(u);
        CDFG.push_back(to_row);
    }

    //CDFG[i].push_back(v);
    //CDFG[v].push_back(u);
}

void Common::displayCDFG() const
{
    for(auto i: CDFG)
    {
        for (auto j: i)
        {
            cout << j.getType() << endl;
        }
    }
}