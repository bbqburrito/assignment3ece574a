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

vector<string> Operation::getInputs() const
{
    return inputs;
}

void Operation::setInputs(vector<string> to_set)
{
    inputs = to_set;
}

void Operation::add_input(string to_add)
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
    v0 = to_copy.getV0();
    v_n = to_copy.getVn();
}

void Common::operator = (const Common& to_copy)
{
    CDFG = to_copy.getCDFG();
    vertices = to_copy.getVertices();
    v0 = to_copy.getV0();
    v_n = to_copy.getVn();
}


//add edge to directed graph
void Common::add_edge(Operation from, Operation to)
{
    bool exists = false;        //temp to test whether need to create new row
    vector<Operation> to_row;   //temp to create new row
    //place first element adjacency list
    for(auto i: CDFG)
    {
        if(i.front() == from)  //add v to adjacency list for u
        {
            exists = true;
            i.push_back(to);
        }
        
    }
    //if not in graph, add new row to graph
    if(!exists)
    {
        to_row.push_back(from);
        to_row.push_back(to);
        CDFG.push_back(to_row);
    }    
}

void Common::addRow()
{

}

void Common::displayCDFG() const
{
    for(auto i: CDFG)
    {
        for (auto j: i)
        {
            cout << j.getNumber() << endl;
        }
    }
}

void Common::addVertex(Operation to_add)
{
    bool is_present = false;
    for (auto i: vertices)      //check if already in vector
    {
        if(i == to_add)         //if to_add found, can stop iterating
        {
            is_present = true;
            cout << i.getNumber() << " is already present" << endl;
            break;
        }
    }
    if(!is_present)
    {
        vertices.push_back(to_add);
    }
}

void Common::buildCDFG()
{
    bool is_inserted = false;
    bool is_present = false;
    vector<string> v0_inputs;   //first level inputs
    vector<string> v_inputs;  //hold inputs for each level
    vector<string> v_next_inputs;   //hold inputs for next level
    vector<string> final_outputs;   //hold outputs of netlist
    vector<string> to_outputs;  //hold all outputs
    vector<Operation> level;    //hold nodes for each level
    
    //find all outputs
    for (auto i: vertices)
    {
        for(auto j: to_outputs)
        {
            if(j == i.getOutput())
            {
                is_present = true;
                break;
            }
        }
        if(!is_present)
        {
            to_outputs.push_back(i.getOutput());
        }
        is_present = false;
    }
    
    is_present = false;

    //find final outputs
    for(decltype(to_outputs.size()) it = 0; it < to_outputs.size(); ++it)         //for each output
    {
        for(auto j: vertices)       //for each vertex
        {
            for(auto k: j.getInputs())  //for each input in vertex
            {
                if(k == to_outputs[it])
                {
                    is_present = true;
                    break;
                }
            }
        }
        if(!is_present)     //if not an input, place in final_outputs and remove from to_outputs
        {
            final_outputs.push_back(to_outputs[it]);
            to_outputs.erase(remove(to_outputs.begin(), to_outputs.end(), to_outputs[it]), to_outputs.end());
        }
        is_present = false;             //reset present test
    }

    is_present = false;

    //build v_n
    for(auto i: final_outputs)
    {
        v_n.add_input(i);
        v_n.setNumber(0);           
        for(auto j: vertices)                       
        {
            if(j.getNumber() > v_n.getNumber())         //set v_n identifier greater than all other nodes
            {
                v_n.setNumber(j.getNumber() + 1);
            }
        }
    }
    
    //build final nodes in CDFG
    for (auto i: final_outputs)
    {
        for (auto j: vertices)
        {
            if(i == j.getOutput())
            {
                add_edge(j, v_n);
                for(auto k: j.getInputs())      //fill v_inputs
                {
                    if(find(v_inputs.begin(), v_inputs.end(), k) != v_inputs.begin())
                    {
                        v_inputs.push_back(k);
                    }
                }
            }
        }
    }

    //find v(0) inputs
    for (auto i: vertices)
    {
        for(auto j: i.getInputs())
        {
            for(auto k: to_outputs)
            {
                if(j == k)
                {
                    is_present = true;
                    break;
                }
            }
            if(!is_present)
            {
                v0_inputs.push_back(j);
            }
        }   
    }

    is_present = false;
    
    //iterate backwords through each level until find v0, 
    while(!v_inputs.empty())
    {
        for(auto i: vertices)   //iterate through all vertices
                                //if any input of that vertex is on output list
                                //place vertex in CDFG
        {
            for(auto j: i.getInputs())  //
            {
                for(auto k: to_outputs)
                {
                    if()
                }

            }
        }
    }
}