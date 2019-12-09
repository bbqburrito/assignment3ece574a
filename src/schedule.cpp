//
//  schedule.cpp
//  hlsyn
//
//
//



#include "schedule.h"
schedule::schedule(){
    this->aluprob = vector<float>(0);
    this->mulprob = vector<float>(0);
    this->divmodprob = vector<float>(0);
}

//always call ASAP before ALAP
void schedule::asap(vector<Common> &mod){
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int edge = 1;    //latest scheduled predecessor module time
    bool ready;                //determine if module is ready to be scheduled
    unsigned int tempedge;
    for (i = 0; i < mod.size(); i++)
    {
        //if the module haven't been scheduled
        if (mod.at(i).getTimeFrame().size() == 0)
        {
            ready = true;
            //iterate through inputs of current module
            for (j = 0; j < mod.at(i).getopin().size(); j++)
            {
                //if one of the inputs has no predecessor node, check next input
                int pre=haspredecessor(mod, mod.at(i).getopin().at(j));
                if (pre==-1);
                //if predecessor node is not yet scheduled, move on to next node
                else if (mod.at(pre).getTimeFrame().size() == 0)
                {
                    ready = false;
                    break;
                }
                //if predecessor node has been scheduled
                else
                {
                    if ((mod.at(pre).getoperation() == "DIV") || (mod.at(pre).getoperation() == "MOD"))
                        tempedge=mod.at(pre).getTimeFrame().at(0) + 3;
                    else if (mod.at(pre).getoperation() == "MUL")
                        tempedge=mod.at(pre).getTimeFrame().at(0) + 2;
                    
                    else
                        tempedge=mod.at(pre).getTimeFrame().at(0) + 1;
                    if(tempedge>edge)  edge=tempedge;
                }
            }
            if (ready)
            {
                mod.at(i).setTimeFrame(edge);
                edge=1;
            }
        }
    }
    return;
}
void schedule::alap(vector<Common> &mod,int latency){//void or int???
    unsigned int i = 0;
    //  unsigned int j = 0;
    unsigned int k;
    unsigned int edge = latency;    //earliest successor node alap time
    bool ready;                //determine if module is ready to be scheduled
    vector<int> next;
    unsigned int tempedge;
    //unsigned int index;
    for (i = 0; i < mod.size(); i++)
    {
        //if the module haven't been scheduled
        if (mod.at(i).getTimeFrame().size() == 1)
        {
            ready = true;
            
            //if the output has no successor node, check next output
            next=hassuccessor(mod, mod.at(i).getopout());
            if (next.size()==0);
            else{
                for(k=0;k<next.size();k++){
                    //if a successor module is not scheduled move on to next module
                    if (mod.at(next.at(k)).getTimeFrame().size() == 1)
                    {
                        ready = false;
                        break;
                    }
                    //if successor node has been scheduled
                    else
                    {
                        //                    if ((mod.at(next.at(k)).getoperation() == "DIV") || (mod.at(next.at(k)).getoperation() == "MOD"))
                        if ((mod.at(i).getoperation() == "DIV") || (mod.at(i).getoperation() == "MOD"))
                            tempedge=mod.at(next.at(k)).getTimeFrame().at(1) - 3;
                        
                        //                    else if (mod.at(next.at(k)).getoperation() == "MUL")
                        else if (mod.at(i).getoperation() == "MUL")
                            tempedge=mod.at(next.at(k)).getTimeFrame().at(1) - 2;
                        else
                            tempedge=mod.at(next.at(k)).getTimeFrame().at(1) - 1;
                        
                        //                    index=next.at(k);
                        if(tempedge<=edge)  edge=tempedge;
                    }}}
            if (ready)
            {
                mod.at(i).setTimeFrame(edge);
                edge=latency;
            }
        }
    }
    return;
}
int schedule::haspredecessor(vector<Common>mod,string current){
    int i=0;
    for(i=0;i<mod.size();i++){
        if(mod.at(i).getopout()==current)   {
            return i;
        }
        
    }
    return -1;
}
vector<int> schedule::hassuccessor(vector<Common>mod,string current){
    unsigned int i=0;
    unsigned int j=0;
    vector<int> num={};//the index of each successors in a vector
    for(i=0;i<mod.size();i++){
        for(j=0;j<mod.at(i).getopin().size();j++){
            if(mod.at(i).getopin().at(j)==current)   {
                num.push_back(i);
            }}
        
    }
    
    return num;
}
void schedule::probability(vector<Common>&mod,int latency)
{
    unsigned int i = 0;
    unsigned int j = 0;
    float probability;
    vector<float> aluGraph = vector<float>(latency);
    vector<float> mulGraph = vector<float>(latency);
    vector<float> divGraph = vector<float>(latency);
    
    for (i = 0; i < mod.size(); i++)
    {
        //propablity for each time is 1/(timeframewidth)
        probability = 1/((float)mod.at(i).getTimeFrame().at(1)-(float)mod.at(i).getTimeFrame().at(0) + 1);
        
        j = mod.at(i).getTimeFrame().at(0)-1;
        for (j; j < mod.at(i).getTimeFrame().at(1); j++)
        {
            //Sum up the probabilities of the operations implemented by a specific resource at each
            // time step
            if ((mod.at(i).getoperation() == "DIV") || (mod.at(i).getoperation() == "MOD"))
                divGraph.at(j) = divGraph.at(j) + probability;
            
            else if ((mod.at(i).getoperation() == "MUL"))
                mulGraph.at(j) = mulGraph.at(j) + probability;
            
            else                aluGraph.at(j) = aluGraph.at(j) + probability;
            
        }
    }
    this->aluprob = aluGraph;
    this->mulprob = mulGraph;
    this->divmodprob = divGraph;
}
//start is the timeframe.at(0)-1(because the index of probability start from 0)
//end is  timeframe.at(1)
vector<float> schedule:: selfForce(Common &tempmod, int start, int end){
    
    unsigned int i;
    unsigned int j;
    float probability;
    float tempforce = 0;
    vector<float> selfForce = vector<float>(0);
    vector<float> graph = vector<float>(0);
    
    
    //propablity for each time is 1/(timeframewidth)
    probability = 1/((float)tempmod.getTimeFrame().at(1)-(float)tempmod.getTimeFrame().at(0) + 1);
    
    if ((tempmod.getoperation() == "DIV") || (tempmod.getoperation() == "MOD"))
        graph = divmodprob;
    
    else if (tempmod.getoperation() == "MUL")
        graph = mulprob;
    
    else
        graph = aluprob;
    
    //iterate through the possible times this node can be scheduled at
    //in order to get the force assuming it is scheduled at time i
    i = start;
    for (i ; i < end; i++)
    {
        tempforce = 0;
        //iterate through the possibe times this node can be scheduled at
        //in order to add the forces
        j = tempmod.getTimeFrame().at(0)-1;
        for (j; j < tempmod.getTimeFrame().at(1); j++)
        {
            //if schedule at assumed time, factor += (distribution to time j)*(1 - probability)
            if (i == j)
                tempforce += graph.at(j)*(1-probability);
            
            //if it's not the assumed time, force+= (distribution to time j)*(0 - probability)
            else
                tempforce += graph.at(j)*(0-probability);
            
        }
        //add self force at assumed time i to the vector of self force
        selfForce.push_back(tempforce);
    }
    return selfForce;
    
    
}


float schedule::predecessorForce(vector<Common>&mod,Common prenode, int scheduleTime)
{
    unsigned int i = 0;
    float preForce = 0;        // predecessor forces of current node
    vector<float> tempForce;
    
    if (prenode.getoperation() == "DIV" || prenode.getoperation() == "MOD")
    {
        if (scheduleTime > (prenode.getTimeFrame().at(1) +1))
            scheduleTime = prenode.getTimeFrame().at(1) - 1;
        else
            scheduleTime = scheduleTime - 3;
    }
    else if (prenode.getoperation() == "MUL")
    {
        if(scheduleTime > (prenode.getTimeFrame().at(1)))
            scheduleTime = prenode.getTimeFrame().at(1) - 1;
        else
            scheduleTime = scheduleTime - 2;
    }
    else
    {
        if (scheduleTime  > (prenode.getTimeFrame().at(1)-1))
            scheduleTime = prenode.getTimeFrame().at(1)-1;
        else
            scheduleTime = scheduleTime - 1;
    }
    
    for (i = 0; i < prenode.getopin().size(); i++)
    {
        //if has predecessor, calculate the predecessor force the current predecessor node
        int pre=haspredecessor(mod, prenode.getopin().at(i));
        
        if (pre != -1)
            preForce = preForce + predecessorForce(mod,mod.at(pre), scheduleTime);
    }
    //calculate the self force of the predecessor node
    tempForce = selfForce(prenode, prenode.getTimeFrame().at(0)-1, scheduleTime+1);
    
    //calculated total predecessor force
    for (i = 0; i < tempForce.size(); i++)
        preForce = preForce + tempForce.at(i);
    
    return preForce;
    
}

float schedule::successorForce(vector<Common>&mod,Common currnode,Common nextnode, int scheduleTime)
{
    unsigned int i = 0;
    unsigned int k = 0;
    float succForce = 0;        //sum of successive forces of this node
    vector<float> tempForce;
    
    if (currnode.getoperation() == "DIV" || currnode.getoperation() == "MOD")
    {
        if ((scheduleTime + 2) < (nextnode.getTimeFrame().at(0) - 1))
            scheduleTime = nextnode.getTimeFrame().at(0) - 1;
        else
            scheduleTime = scheduleTime + 3;
    }
    else if (currnode.getoperation() == "MUL")
    {
        if((scheduleTime + 1) < (nextnode.getTimeFrame().at(0) - 1))
            scheduleTime = nextnode.getTimeFrame().at(0) - 1;
        else
            scheduleTime = scheduleTime + 2;
    }
    else
    {
        if (scheduleTime < (nextnode.getTimeFrame().at(0) - 1))
            scheduleTime = nextnode.getTimeFrame().at(0) - 1;
        else
            scheduleTime = scheduleTime + 1;
    }
    
    vector<int> next=hassuccessor(mod,nextnode.getopout());
    if(next.size()!=0){
        for(k=0;k<next.size();k++){
            //if has successor node, calculate the successor force the current successor node
            
            succForce = succForce + successorForce(mod,nextnode,mod.at(next.at(k)), scheduleTime);
        }}
    //calculate the self forces of the successor node
    tempForce = selfForce(nextnode, scheduleTime, nextnode.getTimeFrame().at(1));
    //+1???????
    //calculated total successor force
    for (i = 0; i < tempForce.size(); i++)
        succForce = succForce + tempForce.at(i);
    
    return succForce;
    
    
}

void schedule::forceschedule(vector<Common>&mod,int latency)
{//unsigned int n;
   //  for(n = 0; n < mod.size(); n++){
    int size=mod.size();
    for(size;size>0;size--){//asap schedule for everynode
        asap(mod);
    }
    for(size=mod.size();size>0;size--){//alap schedule for everynode
        alap(mod, latency);
    }
    
    
    unsigned int scheduleTime;      //assumed schedule time
    int nodeindex;                //module with lowest force
    int time;                //final schedule time
    int tempTime;            //current schedule time
    float tempForce;
   unsigned int temp;
    float minForce = 10000;    //random lowest force
    vector<float> force;    //total force for each node
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    unsigned int m = 0;
    unsigned int flag= 0;

     unsigned int pre = 0;
    probability(mod,latency);//calculate the probability for each operation type of each time
   
    for (i = 0; i < mod.size(); i++)
    {minForce = 10000;minForce = 10000;flag=0;
        //check if is already scheduled
        if(mod.at(i).getTimeFrame().size() == 2)
        {
             scheduleTime =  mod.at(i).getTimeFrame().at(0) - 1;
            //calculate self force for each assumed time
            tempForce = 10000;
           
            for(m = 0; m < mod.at(i).getopin().size(); m++){
                pre=haspredecessor(mod, mod.at(i).getopin().at(m));
                if(pre!=-1){
                    if(mod.at(pre).getTimeFrame().size()==3){
                        temp =  mod.at(pre).getTimeFrame().at(2);
                        if(scheduleTime<temp) scheduleTime=temp;
                        else temp=scheduleTime;
                        flag=1;
                    }
                }
            }
//            force = selfForce(mod.at(i), mod.at(i).getTimeFrame().at(0)-1, mod.at(i).getTimeFrame().at(1));
            force = selfForce(mod.at(i), scheduleTime, mod.at(i).getTimeFrame().at(1));

            //iterate thorugh the self forces, add predecessor and successor forces to the self forces
            
//           scheduleTime =  mod.at(i).getTimeFrame().at(0) - 1;
            for (j = 0; j < force.size(); j++)
            {
               // scheduleTime = j + mod.at(i).getTimeFrame().at(0) - 1;
                //iterate through the successor nodes
                vector<int> next=hassuccessor(mod, mod.at(i).getopout());
                //add successorForce of each successor node
                for(k=0;k<next.size();k++){
                    
                    force.at(j) = force.at(j) + successorForce(mod,mod.at(i),mod.at(next.at(k)), scheduleTime);
                    
                }
                for (m = 0; m < mod.at(i).getopin().size(); m++)
                {
                    //add predecessor  forces of each predecessor node to the total force
                   pre=haspredecessor(mod, mod.at(i).getopin().at(m));
                    if(pre!=-1)
                        force.at(j) = force.at(j) + predecessorForce(mod,mod.at(pre), scheduleTime);
                }
                
                //update the force to be minimum
                if (force.at(j) < tempForce)
                {
                    tempForce = force.at(j);
                    if(flag==1){
                        tempTime = temp+1+ j;}
                    else{
                        tempTime = mod.at(i).getTimeFrame().at(0) + j;
                    }
                }
                scheduleTime+=1;
            }
            //determine the final schedule time to be the one with minimum force
            
            if (tempForce < minForce)
            {
                minForce = tempForce;
                nodeindex = i;
                time = tempTime;
                mod.at(i).setforce(tempForce);
            }
        }
//       
   // }
//            for ( m = 0; m < mod.at(nodeindex).getopin().size(); m++)
//            {
//
//                pre=haspredecessor(mod, mod.at(nodeindex).getopin().at(m));
//                if(pre!=-1){
//                    if(mod.at(pre).getTimeFrame().size()==3){
//                    if(time==mod.at(pre).getTimeFrame().at(2)){
//                        time=time+1;
//                        break;
//                    }
//                    }
//                }}

        
    mod.at(i).setTimeFrame(time);
  }
   
}

//public wrapper for follow branch to end function. 
int schedule::navigateBranch(int path_length, Operation branch_at, vector<Operation>& to_end)
{
    if(branch_at == schedulerGraph.getV0())
    {
        return path_length;
    }

    if(branch_at.getBranches().size())
    {
        to_end = ifScheduler(branch_at, path_length);
        return path_length;
    }

    return navigateBranch(path_length, branch_at, to_end, branch_at.getPath());
}

//private function to recursively follow branch to end.
int schedule::navigateBranch(int path_length, Operation branch_at, 
                                        vector<Operation>& to_end, vector<Operation> which_branch)
{
    Operation next_branch;
    to_end.push_back(branch_at);
    path_length += branch_at.getCycles();
    if(branch_at.getBranches().size())
    {
        to_end = ifScheduler(branch_at, path_length);
        return path_length;
    }

    //find Operations in graph on branch with branch_at output as input
    for(auto it0: schedulerGraph.getVertices())
    {
        for(auto it1: it0.getInputs())
        {
            if(it1 == branch_at.getOutput())
            {
                if(it1.getPath)
            }
        }
    }

    return navigateBranch(path_length, branch_at.getOutput(), to_end, branch_at.getPath());
}

//walk possible paths of if statement. throws "not_there" and "not_mux"
//path parameter defaults to zero.
vector<Operation> schedule::ifScheduler(Operation branch_at, int& total_path_length)
{
    Operation current;
    Operation next;
    vector<Operation> scheduled_branch;
    vector<Operation> temp_branch;
    string exist_error = "not_there";
    string type_error = "not_mux";
    int path_length = 0;


    //if parameter not in Operations vector, throw error 
    if(find(schedulerGraph.getVertices().begin(), schedulerGraph.getVertices().end(), branch_at) == 
            schedulerGraph.getVertices().end())
    {
        throw exist_error;
    }

    //ensure parameter is a mux for an if statement
    if(branch_at.getType() != "MUX2X1" || (branch_at.getBranches().size() == 0))
    {
        throw type_error;
    }
  
    //find Operation in graph and place at front of return vector
    for (auto it: schedulerGraph.getVertices())
    {
        if(it == branch_at)
        {
            scheduled_branch.push_back(it);
        }
    }

    //get each path in if statement, place longest path in return vector

    //add cycles for branch_at operation
    path += scheduled_branch.front().getCycles();
    current = scheduled_branch.front();
    for(auto it: scheduled_branch.front().getBranches())
    {
        current = it;
        //iterate through path from each output
        while(current != schedulerGraph.getV0())
        {

        }
    }

        
}