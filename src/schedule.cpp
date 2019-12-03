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
                    else if (mod.at(i).getoperation() == "MUL")
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
                    if ((mod.at(next.at(k)).getoperation() == "DIV") || (mod.at(next.at(k)).getoperation() == "MOD"))
                        tempedge=mod.at(next.at(k)).getTimeFrame().at(1) - 3;
                    else if (mod.at(next.at(k)).getoperation() == "MUL")
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
        for (j=0; j < mod.at(i).getTimeFrame().at(1); j++)
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
    
    unsigned int i = 0;
    unsigned int j = 0;
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
    vector<float> tempForce;    //self forces of implicitly scheduled nodes
    
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
    
    //iterate through predecessor nodes if there are any
    for (i = 0; i < prenode.getopin().size(); i++)
    {
        //if thas predecessor
        int pre=haspredecessor(mod, prenode.getopin().at(i));
  
        if (pre != -1)
            preForce = preForce + predecessorForce(mod,mod.at(pre), scheduleTime);
    }
    //calculate the self forces of implicitly shceduled nodes
    tempForce = selfForce(prenode, prenode.getTimeFrame().at(0)-1, scheduleTime+1);
    
    //add up self forces
    for (i = 0; i < tempForce.size(); i++)
        preForce = preForce + tempForce.at(i);
    
    return preForce;
    
}

float schedule::successorForce(vector<Common>&mod,Common nextnode, int scheduleTime)
{
    unsigned int i = 0;
        unsigned int k = 0;
    float succForce = 0;        //sum of successive forces
    vector<float> tempForce;    //self forces of implicitly scheduled nodes
    
    if (nextnode.getoperation() == "DIV" || nextnode.getoperation() == "MOD")
    {
        if ((scheduleTime + 2) < (nextnode.getTimeFrame().at(0) - 1))
            scheduleTime = nextnode.getTimeFrame().at(0) - 1;
        else
            scheduleTime = scheduleTime + 3;
    }
    else if (nextnode.getoperation() == "MUL")
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
    
    //iterate through successor nodes if there are any
    vector<int> next=hassuccessor(mod,nextnode.getopout());
    if(next.size()!=0){
    for(k=0;k<next.size();k++){
        //if has successor node
       
            succForce = succForce + successorForce(mod,mod.at(next.at(k)), scheduleTime);
    }}
    //calculate the self forces of implicitly shceduled nodes
    tempForce = selfForce(nextnode, scheduleTime, nextnode.getTimeFrame().at(1));
    
    //add up self forces
    for (i = 0; i < tempForce.size(); i++)
        succForce = succForce + tempForce.at(i);
    
    return succForce;
    
    
}

void schedule::forceschedule(vector<Common>&mod,int latency)
{
    int size=mod.size();
    for(size;size>0;size--){
          asap(mod);
    }
    for(size=mod.size();size>0;size--){
   alap(mod, latency);//issue:f没有alap time
    }
    
    //alap(mod, latency);//issue:f没有alap time
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    unsigned int assumedTime;
    int module;                //module with lowest force
    int time;                //time with lowest force
    int tempTime;            //current module's time with lowest force
    float tempForce;            //current module's lowest force
    float minForce = 10000;    //lowest force
    vector<float> force;    //module's forces
    
    probability(mod,latency);
    //iterate through modules
    for (i = 0; i < mod.size(); i++)
    {
        //check if is already scheduled
        if(mod.at(i).getTimeFrame().size() == 2)
        {
            //calculate self forces for each time in a module
            tempForce = 10000;
            force = selfForce(mod.at(i), mod.at(i).getTimeFrame().at(0)-1, mod.at(i).getTimeFrame().at(1));
            
            //iterate thorugh the self forces
            for (j = 0; j < force.size(); j++)
            {
                assumedTime = j + mod.at(i).getTimeFrame().at(0) - 1;
                //iterate through the successor nodes
                vector<int> next=hassuccessor(mod, mod.at(i).getopout());
                for(k=0;k<next.size();k++){
          
                    //add predecessor and successor forces to the self forces
                   
                        force.at(j) = force.at(j) + successorForce(mod,mod.at(next.at(k)), assumedTime);
                
                }
                for (k = 0; k < mod.at(i).getopin().size(); k++)
                {
                    //add predecessor and successor forces to the self forces
                     int pre=haspredecessor(mod, mod.at(i).getopin().at(k));
                    if(pre!=-1)
                        force.at(j) = force.at(j) + predecessorForce(mod,mod.at(pre), assumedTime);
                }
                
                //if this force is the minimum, update minimum
                //and update its time
                if (force.at(j) < tempForce)
                {
                    tempForce = force.at(j);
                    tempTime = mod.at(i).getTimeFrame().at(0) + j;
                }
            }
            //if the min force of the module is less then the current min,
            //update min, the module, and the time it is to be scheduled
            if (tempForce < minForce)
            {
                minForce = tempForce;
                module = i;
                time = tempTime;
            }
        }
    }
 
    mod.at(module).setTimeFrame(time);

}
