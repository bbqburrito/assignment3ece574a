#include "CDFG_graph.h"

using namespace std;
using std::string;

Operation::~Operation()
{}

Operation::Operation():number(0), type(""), inputs {}, output(""), cycles(0), alap(0), asap(0) {}

Operation::Operation(int to_number, string to_type, vector<string> to_inputs, string to_output, 
                    int to_cycles, int to_alap, int to_asap, vector<Operation> to_branches, 
                    vector<Operation> to_path):number(to_number), type(to_type), 
                    inputs(to_inputs), output(to_output), cycles(to_cycles), alap(to_alap), 
                    asap(to_asap), branches(to_branches), path(to_path)  {}

Operation::Operation(const Operation& to_copy)
{
    number = to_copy.getNumber();
    type = to_copy.getType();
    inputs = to_copy.getInputs();
    output = to_copy.getOutput();
    cycles = to_copy.getCycles();
    alap = to_copy.getAlap();
    asap = to_copy.getAsap();
    branches = to_copy.getBranches();
}

void Operation::operator = (const Operation& to_copy)
{
    number = to_copy.getNumber();
    type = to_copy.getType();
    inputs = to_copy.getInputs();
    output = to_copy.getOutput();
    cycles = to_copy.getCycles();
    alap = to_copy.getAlap();
    asap = to_copy.getAsap();
    branches = to_copy.getBranches();
}

vector<string> Operation::getInputs() const
{
    return inputs;
}

vector<Operation> Operation::getBranches() const
{
    return branches;
}

vector<Operation> Operation::getPath() const
{
    return path;
}

void Operation::setInputs(vector<string> to_set)
{
    inputs = to_set;
}

void Operation::setBranches(vector<Operation> to_set)
{
    branches = to_set;
}

void Operation::setPath(vector<Operation> to_set)
{
    path = to_set;
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

CDFG_graph::CDFG_graph():CDFG{}, vertices{} {}

CDFG_graph::~CDFG_graph()
{}

CDFG_graph::CDFG_graph(const CDFG_graph& to_copy)
{
    CDFG = to_copy.getCDFG();
    vertices = to_copy.getVertices();
    v0 = to_copy.getV0();
    v_n = to_copy.getVn();
}

void CDFG_graph::operator = (const CDFG_graph& to_copy)
{
    CDFG = to_copy.getCDFG();
    vertices = to_copy.getVertices();
    v0 = to_copy.getV0();
    v_n = to_copy.getVn();
}


//add edge to directed graph
void CDFG_graph::add_edge(Operation from, Operation to)
{
    bool exists = false;        //temp to test whether need to create new row
    vector<Operation> to_row;   //temp to create new row
    //place first element adjacency list
    for(auto i: CDFG)
    {
        if(i.front() == from)   //add v to adjacency list for u: place in temp vector
                                //then delete iterator vector from CDFG. place temp vector in CDFG
        {
            exists = true;
            to_row = i;
            to_row.push_back(to);
            CDFG.erase(remove(CDFG.begin(), CDFG.end(), i), CDFG.end());
            CDFG.push_back(to_row);

        }
        int x = 1;              //operation for debugging
        
    }
    //if not in graph, add new row to graph
    if(!exists)
    {
        to_row.push_back(from);
        to_row.push_back(to);
        CDFG.push_back(to_row);
    }    
}

void CDFG_graph::displayCDFG() const
{
    for(auto i: CDFG)
    {
        for (auto j: i)
        {
            cout << j.getNumber() << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void CDFG_graph::displayVertices() const
{
    for(auto i: vertices)
    {
        cout << i.getNumber() << '\t' << i.getType() << '\t';
        cout << i.getOutput() << '\t' << i.getInputs()[0] << ',';
        cout << i.getInputs()[1] << '\t' << endl;
    }

    cout << v_n.getNumber() << '\t' << v_n.getType() << '\t';
    cout << v_n.getOutput() << '\t' << v_n.getInputs()[0] << ',';
    cout << v_n.getInputs()[1] << ',' << v_n.getInputs()[2] << endl;
}

//add to vertices vector
void CDFG_graph::addVertex(Operation to_add)
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

//builds CDFG from vertices in CDFG_graph class. does not throw. 
//vertices vector must be complete before this funcion is called
void CDFG_graph::buildCDFG()
{
    bool is_inserted = false;
    bool is_present = false;
    vector<string> v0_inputs;   //first level inputs
    vector<string> v_inputs;  //hold inputs for each level
    vector<string> v_next_inputs;   //hold inputs for next level
    vector<string> final_outputs;   //hold outputs of netlist
    vector<string> to_outputs;  //hold all outputs
    vector<Operation> level;    //hold nodes for each level
    vector<Operation> next_level;   //hold nodes for next level
    
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
        if(!is_present)     //if not an input, place in final_outputs 
        {
            final_outputs.push_back(to_outputs[it]);
            //to_outputs.erase(remove(to_outputs.begin(), to_outputs.end(), to_outputs[it]), to_outputs.end());
        }
        is_present = false;             //reset present test
    }

    is_present = false;

    //erase final_outputs from to_outputs
    for(auto i: final_outputs)
    {
        to_outputs.erase(remove(to_outputs.begin(), to_outputs.end(), i), to_outputs.end());
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
                add_edge(j, v_n);               //add to CDFG
                level.push_back(j);             //add to level
                for(auto k: j.getInputs())      //fill v_inputs
                {
                    if((find(v_inputs.begin(), v_inputs.end(), k) == v_inputs.end()) &&   //if input is not already in v_inputs and not in v0_inputs, place in v_inputs
                            (find(v0_inputs.begin(), v0_inputs.end(), k) == v0_inputs.end()))
                    {
                        v_inputs.push_back(k);
                    }
                }
            }
        }
    }

    is_present = false;
    
    //iterate backwords through each level until find v0, 
    while(!v_inputs.empty())        //each level, v_inputs is emptied and replaced with 
                                    //inputs for next level. v0 inputs are not placed in
                                    //v_inputs next level.
    {
        for(auto i: vertices)   //iterate through all vertices
                                //if any input of that vertex is in v_inputs
                                //place vertex in CDFG, place vertex in next_level 
        {
            if(find(v_inputs.begin(), v_inputs.end(), i.getOutput()) != v_inputs.end())   //if the vertex's output is in v_inputs
            {
                for(auto j: level)          //find connected node and add edge
                {
                    //bool test = find(j.getInputs().begin(), j.getInputs().end(), i.getOutput()) != j.getInputs().end();
                    //if(!(find(j.getInputs().begin(), j.getInputs().end(), i.getOutput()) == (j.getInputs().end())))
                    for(auto k: j.getInputs())
                    {
                        if(k == i.getOutput())
                        {
                            add_edge(i, j);
                        }
                    }
                }       
                is_present = false;
                //if node is not already in next_level, add to next_level
                for(auto j: next_level)
                {
                    if(j == i)
                    {
                        is_present = true;
                    }     
                }
                if(!is_present)
                {
                    next_level.push_back(i);
                }
                is_present = false;

                for(auto j: i.getInputs())          //iterate through node's inputs and place in v_next_inputs
                                                    //unless input is in v0_inputs or already in v_next_inputs
                {
                    if((find(v_next_inputs.begin(), v_next_inputs.end(), j) == v_next_inputs.end()) &&
                                find(v0_inputs.begin(), v0_inputs.end(), j) == v0_inputs.end())
                    {
                        v_next_inputs.push_back(j);
                    }
                    
                }
            }
        }
        level.clear();                  //clear level and replace with next level
        level = next_level;
        next_level.clear();             //clear next_level for next iteration
        v_inputs.clear();               //clear v_inputs and replace with next level
        v_inputs = v_next_inputs;
        v_next_inputs.clear();          //clear v_next_inputs for next iteration

    }
}