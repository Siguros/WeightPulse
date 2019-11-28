#include "WeightTest.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include "formula.h"
#include "Param.h"
#include "Array.h"
#include "Mapping.h"
#include <random>

extern Param *param;
extern Array *arrayIH;
extern Array *arrayHO;

void WeightTest(){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0,param->NumcellPerSynapse);
    
    double NL_LTP = static_cast<RealDevice*>(arrayIH->cell[0][0])->NL_LTP;
    double NL_LTD = static_cast<RealDevice*>(arrayIH->cell[0][0])->NL_LTD;
    int totalStep = static_cast<RealDevice*>(arrayIH->cell[0][0])->maxNumLevelLTP;
    double maxCoductance = static_cast<AnalogNVM*>(arrayIH->cell[0][0])->maxConductance;
    int selectcounter=0;
    int NumCell=0;
    for(int s=0; s< totalStep; s++){
        double ProbabilityIH=0;
        double ProbabilityHO=0;
        int count = 0;
        int satcount=0;
        double weight=0;
        selectcounter = dist(mt);
        for(int i=0; i < param->nInput; i++){
            
            for(int j=0; j< param->nHide; j++){
                count +=1;
                NumCell=selectcounter%param->NumcellPerSynapse;
                if(static_cast<AnalogNVM*>(arrayIH->cell[j][i])->conductanceN[NumCell] == maxCoductance){
                    satcount+=1;
                }
              arrayIH->WritePulseToWeight(j,i,s,NumCell);
              weight = arrayIH->ConductanceToWeight(j,i,1.0,0);
        }
    }
    ProbabilityIH = (double)(count -satcount) / count ; 
    std::cout << weight << " " << ProbabilityIH << std::endl;

    }
 

}