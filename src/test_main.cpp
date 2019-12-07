#include "CDFG_graph.h"

using namespace std;

int main()
{
    vector<string> test_inputs;
    vector<string> test_outputs;
    vector<string> make_inputs;
    vector<string> test_types;
    vector<Operation> ops;
    Operation* temp;
    CDFG_graph make_graph;
    int i = 0;

    //generate types
    test_types.resize(10);
    for(decltype(test_types.size()) it = 0; it < 10; ++it)
    {
        if(!(i%3))
        {
            test_types[it] = "mul";
        }
        else if(!(i%2))
        {
            test_types[it] = "div";
        }
        else test_types[it] = "add";
        ++i;
    }

    //generate inputs
    test_inputs.resize(20);
    for(decltype(test_inputs.size()) it = 0; it < 20; ++it)
    {
        test_inputs[it] = "a" + to_string(it);
    }

    //generate outputs. shift by 3 so v0 has three outputs, v_n has 13 inputs
    test_outputs.resize(10);
    for(decltype(test_outputs.size()) it = 0; it < 10; ++it)
    {
        test_outputs[it] = "a" + to_string(it + 13);
    }

    //generate operation vector
    make_inputs.resize(2);
    ops.resize(10);
    for (decltype(ops.size()) it = 0; it < 10; ++it)
    {
        //generate input vector
        for(decltype(make_inputs.size()) si = 0; si < 2; ++si)
        {
            make_inputs[si] = test_inputs[2*it + si];
        }
        //initialize new Operation
        //temp[it] = new Operation(it, test_types[it], make_inputs, test_outputs[it],
        //                            1, 1, 1);
        ops[it].setNumber(it);
        ops[it].setType(test_types[it]);
        ops[it].setInputs(make_inputs);
        ops[it].setOutput(test_outputs[it]);
    }

    make_inputs.clear();
    make_inputs.resize(3);
    make_inputs[0] = "a20";
    make_inputs[1] = "a21";
    make_inputs[2] = "a22";

    temp = new Operation(100, "null", make_inputs, "null", 0, 0, 0);

    make_graph.setVn(*temp);

    make_graph.setVertices(ops);
    make_graph.displayVertices();

    make_graph.buildCDFG();

    make_graph.displayVertices();
    make_graph.displayCDFG();

    return 0;
}