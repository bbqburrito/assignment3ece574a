//
//  schedule.cpp
//  hlsyn
//
//
//

#include "schedule.h"
schedule::schedule(){
    
}

//always call ASAP before ALAP
void schedule::asap(vector<Common> mod){
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int edge = 1;    //latest scheduled predecessor module time
    bool ready;                //determine if module is ready to be scheduled
 
    for (i = 0; i < mod.size(); i++)
    {
        //if the module haven't been scheduled
        if (mod.at(i).getTimeFrame().size() == 0)
        {
            ready = true;
            //iterate through inputs of current module
            for (j = 0; j < mod.at(j).getopin().size(); j++)
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
                        edge=mod.at(pre).getTimeFrame().at(0) + 3;
                    else if (mod.at(i).getoperation() == "MUL")
                        edge=mod.at(pre).getTimeFrame().at(0) + 2;
                    
                    else
                        edge=mod.at(pre).getTimeFrame().at(0) + 1;
                }
            }
            if (ready)
            {
                mod.at(i).setTimeFrame(edge);
            }
        }
    }
    return;
}
void schedule::alap(vector<Common> mod,int latency){//void or int???
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int edge = latency;    //earliest successor node alap time
    bool ready;                //determine if module is ready to be scheduled
    
    for (i = 0; i < mod.size(); i++)
    {
        //if the module haven't been scheduled
        if (mod.at(i).getTimeFrame().size() == 0)
        {
            ready = true;
           
           //if the output has no successor node, check next output
                int next=hassuccessor(mod, mod.at(i).getopout());
                if (next==-1);
                
            //if a successor module is not scheduled move on to next module
                else if (mod.at(next).getTimeFrame().size() == 0)
                {
                    ready = false;
                    break;
                }
                //if successor node has been scheduled
                else
                {
                    if ((mod.at(next).getoperation() == "DIV") || (mod.at(next).getoperation() == "MOD"))
                        edge=mod.at(next).getTimeFrame().at(1) - 3;
                    else if (mod.at(i).getoperation() == "MUL")
                        edge=mod.at(next).getTimeFrame().at(1) - 2;
                    else
                        edge=mod.at(next).getTimeFrame().at(1) - 1;
            }
            if (ready)
            {
                mod.at(i).setTimeFrame(edge);
                
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
int schedule::hassuccessor(vector<Common>mod,string current){
    unsigned int i=0;
    unsigned int j=0;
    for(i=0;i<mod.size();i++){
        for(j=0;j<mod.at(i).getopin().size();j++){
        if(mod.at(i).getopin().at(j)==current)   {
            return i;
        }}
      
    }
    
      return -1;
}

