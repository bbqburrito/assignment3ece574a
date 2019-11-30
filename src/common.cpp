#include "common.h"

using namespace std;
using std::string;

Common::Common() {}

Common::Common(const Common& to_copy)
{
    CDFG = to_copy.getCDFG();
}

vector<vector<Operation>> Common::getCDFG() const
{
    return CDFG;
}

void Common::operator = (const Common& to_copy)
{
    CDFG = to_copy.getCDFG();
}

void Common::add_edge(Operation u, Operation v)
{
    
}