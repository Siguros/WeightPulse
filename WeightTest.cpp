#include "WeightTest.h"
#include <cstdio>
#include <iostream>
#include <fstream>
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
using namespace std;
void WeightTest(){
       ofstream mywriteoutfile;
	                                                                                                          
	double correctav=0;
	double NL = static_cast<RealDevice*>(arrayIH->cell[0][0])->NL_LTP;
	int N = param->NumcellPerSynapse;
	int CS = static_cast<RealDevice*>(arrayIH->cell[0][0])->maxNumLevelLTP;
	double LA = param->alpha1;
	printf("NL:%.1f N: %d CS: %d LA: %.2f\n", NL, N, CS, LA);
	string filename;
	char tempfile[10];
	if(param->Mode_Arbit){
		filename.append("Arbit");
	}
	else{
		filename.append("Rand");
	}
	
	sprintf(tempfile, "%.1f", NL);
	filename.append(tempfile);

	sprintf(tempfile, "%d", N);
	filename.append(tempfile);

	sprintf(tempfile, "%d", CS);
	filename.append(tempfile);

	sprintf(tempfile, "%.2f", LA);
	filename.append(tempfile);

	mywriteoutfile.open(filename+".csv");
	cout<< filename+".csv" << endl;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0,param->NumcellPerSynapse);
    
    double NL_LTP = static_cast<RealDevice*>(arrayIH->cell[0][0])->NL_LTP;
    double NL_LTD = static_cast<RealDevice*>(arrayIH->cell[0][0])->NL_LTD;
    int totalStep = static_cast<RealDevice*>(arrayIH->cell[0][0])->maxNumLevelLTP;
    totalStep = totalStep * param->NumcellPerSynapse;
    double maxCoductance = static_cast<AnalogNVM*>(arrayIH->cell[0][0])->maxConductance;
    int selectcounter=0;
    int NumCell=0;
    int maxNumLevelLTP = maxNumLevelLTP / param->NumcellPerSynapse;
    
    for(int s=1; s<= totalStep*4; s++){
        double ProbabilityIH=0;
        double ProbabilityHO=0;
        int count = 0;
        int satcount=0;
        double weight=0; 
        for(int i=0; i < param->nInput; i++){
            
            for(int j=0; j< param->nHide; j++){
                count +=1;
                //static_cast<AnalogNVM*>(arrayIH->cell[j][i])->maxNumLevelLTP = maxNumLevelLTP;
                // uniform random
                if(!param->Mode_Arbit){
                selectcounter = dist(mt);
                NumCell=selectcounter%param->NumcellPerSynapse;
                }
             
                // counter arbitration
            
                if(static_cast<AnalogNVM*>(arrayIH->cell[j][i])->conductanceN[NumCell] == maxCoductance){
                    satcount+=1;
                   //std::cout<<static_cast<AnalogNVM*>(arrayIH->cell[j][i])->conductance;
                }

              arrayIH->WritePulseToWeight(j,i,1.0,NumCell);
              //cout << arrayIH->ConductanceToWeight(j,i,1.0,0)<<endl;
              weight += arrayIH->ConductanceToWeight(j,i,1.0,0);
            // if(s== totalStep*10){
            //     // std::cout<<static_cast<AnalogNVM*>(arrayIH->cell[j][i])->conductance;
            //     // std::cout << static_cast<eNVM*>(arrayIH->cell[j][i])->avgMaxConductance;
            //                 }
            //   std:: cout << static_cast<AnalogNVM*>(arrayIH->cell[j][i])->conductanceN[0];
             //std:: cout << static_cast<RealDevice*>(arrayIH->cell[j][i])->xPulse;
    
        }

    }
    weight = weight / (param->nInput * param->nHide);
    if(param->Mode_Arbit){
        NumCell +=1;
        NumCell = NumCell % param->NumcellPerSynapse;
    }
    ProbabilityIH = (double)(count -satcount) / count ; 
    std::cout << s << " "<< weight << " " << ProbabilityIH << std::endl;
    mywriteoutfile << s << ", " << weight << ", "<< ProbabilityIH << endl;
    }

 
}
