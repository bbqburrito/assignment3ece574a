#include "common.h"

using namespace std;
using std::string;

Operation::Operation():type(0), inputs {}, output(0), cycles(0) {}

Operation::Operation(const Operation& to_copy)
{
    type = to_copy.getType();
    inputs = to_copy.getInputs();
    output = to_copy.getOutput();
    cycles = to_copy.getCycles();
}

void Operation::operator = (const Operation& to_copy)
{
    type = to_copy.getType();
    inputs = to_copy.getInputs();
    output = to_copy.getOutput();
    cycles = to_copy.getCycles();
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
}

void Common::operator = (const Common& to_copy)
{
    CDFG = to_copy.getCDFG();
}

void Common::add_edge(Operation u, Operation v)
{
    
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