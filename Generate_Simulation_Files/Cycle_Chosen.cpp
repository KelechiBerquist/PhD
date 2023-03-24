#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>


#include "CommonFunctions.h"
#include "WritingLammpsFiles.h"
#include "InteractionParameters.h"


using std::vector;
using std::string;
using std::string;
using std::cout;
using std::ofstream;
using std::ifstream;


#define PaInMPa              1e6 
#define convDefRate          1e-6 
#define FemtoSecInSec        1e15 
#define FemtoSecToNanosec    1e-6
#define deltaT               40
#define StrainLongTrj        0.5       
#define StrainShortTrj       0.02    
#define StrainLog            0.01
#define StrainLogHyst        0.001
#define StrainRestart        1
#define StrainStop           1
#define Encore               3
#define RelaxTime            6


#define BntBead          4  
#define ClBead           1  
#define BtnBead          2  
#define CcnBead          3  
#define LcnBead          4  
#define PcnBead          5  
#define McnBead          7  
#define HcnBead          10  
#define OtaBead          3  
#define CtaBead          5  
#define BtaBead          7  

string SoluteType,       ReadWriteFolderName,    SharedGromacsFiles,   ReadFolderName,   WriteFolderName,        ReadWriteFolderName2;
string ReadFileName1,    ReadFileName2,    ReadFileName3,    ReadFileName4,    ReadFileName5;    
string WriteFileName1,   WriteFileName2,   WriteFileName3,   WriteFileName4,   WriteFileName5;
	
	


int main ()
{
	
	string  LocatePC = "/media/KaceHDD1/";
	// string  LocatePC = "/media/OkwuchisHDD/";
	
	
	string RunPC = LocatePC + "Struct/";
	int RunCores;
	
	
	if (LocatePC == "/media/KaceHDD1/")
	{
		//RunCores = 6;
		RunCores = 4;
		SharedGromacsFiles = "/home/kelechi/SharedGromacsFiles/";
	}
	if (LocatePC == "/media/OkwuchisHDD/")
	{
		RunCores = 4;
		SharedGromacsFiles = "/home/okwuchi/SharedGromacsFiles/";
	}
	
	
	int ACheckEnd = 2,  NCheckEnd = 2; 
	
	// vector <string> Direction        = {"xyzX",  "yxzY"};
	vector <string> Direction        = {"xyzX"};
	vector <string> MainVector       = {"MCN",  "PCN",  "PCN", "CTA"};
	vector <string> SoluteVector     = {"Ota",  "Mcn",  "Mcn", "NaSal"};
	
	vector <int> MainBeadVector   = {McnBead,  PcnBead,  PcnBead,  CtaBead};
	vector <int> SoluteBeadVector = {OtaBead,  McnBead,  McnBead,  BntBead};
	vector <int> SystemMain       = {612,    64,     623,    598};
	vector <int> SystemSolute     = {99,     471,    96,     184};
	vector <int> SystemWater;

	// vector <double> StrainRate = {1, 3, 10, 30};
	vector <double> StrainRate = {30, 10, 3, 1};
	
	
	#define CommonMultipleLongTrj         ceil(StrainLongTrj/(StrainRate[StrainRate.size()-1]*deltaT) )
	#define CommonMultipleShortTrj        ceil(StrainShortTrj/(StrainRate[StrainRate.size()-1]*deltaT) )
	#define CommonMultipleLog             ceil(StrainLog/(StrainRate[StrainRate.size()-1]*deltaT) )
	#define CommonMultipleLogHyst         ceil(StrainLogHyst/(StrainRate[StrainRate.size()-1]*deltaT))
	#define CommonMultipleRestart         ceil(StrainRestart/(StrainRate[StrainRate.size()-1]*deltaT) )

	string ForWriteFile4            =   "";
	ForWriteFile4                  +=   "#!/bin/bash \n \n \n \n \n";
	
	
	for (int chosen = 0;  chosen < MainVector.size();  chosen++)
	{
		mysystem CurrentSystem;
		CurrentSystem.mainTop           =   MainVector[chosen];
		CurrentSystem.soluteTop         =   SoluteVector[chosen];
		CurrentSystem.writeFolderName   =   RunPC + itos(chosen+1) + "/BashScripts/";
		string ForWriteFile2            =   "";
		ForWriteFile2                  +=   "#!/bin/bash \n \n \n \n \n";


		for (int forDirection = 0;  forDirection < Direction.size() ;  forDirection++)
		{
			string Direct = string(1, Direction[forDirection][Direction[forDirection].size()-1] );
			
			if (Direct =="X") 
			{
			   SystemWater = {291500*10/11, 303600*10/11, 405900*10/11, 533390*10/11};
			}
			else
			{
				SystemWater   = {290950*10/11, 292600*10/11, 405900*10/11, 529100*10/11};
			}


			
			CurrentSystem.readFolderName       =   RunPC + itos(chosen+1) + "/" +  Direct + "/Prelim/";
			string  RunSimulationFolderName    =   RunPC + itos(chosen+1) + "/" +  Direct + "/Cycle/";

			cout <<CurrentSystem.writeFolderName<<"|"<<CurrentSystem.readFolderName<<"|"<<Direct<<"   \n";

			
			for (auto rate:  StrainRate)
			{
				string ForWriteFile1 = "";

				CurrentSystem.rate                =   rate;
				CurrentSystem.elongRate           =   CurrentSystem.rate * convDefRate;
				CurrentSystem.multiplierForVisc   =   PaInMPa /(CurrentSystem.elongRate * FemtoSecInSec);
				CurrentSystem.direction           =   Direction[forDirection];
				CurrentSystem.readFileNameEnd     =   "1-" + itos(chosen+1);


				

				for (int encore = 0;  encore < Encore;  encore++)
				{
					CurrentSystem.runType             =   "Tension";
					CurrentSystem.fileNameEnd         =   "Load-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-En" + itos(encore+1);
					CurrentSystem.restartFreq         =   int(StrainRestart/(deltaT*CurrentSystem.elongRate));
					CurrentSystem.trjDumpLong         =   int(StrainLongTrj/(deltaT*CurrentSystem.elongRate));
					CurrentSystem.trjDumpShort        =   int(StrainShortTrj/(deltaT*CurrentSystem.elongRate));
					CurrentSystem.logDump             =   int(StrainLog/(deltaT*CurrentSystem.elongRate));
					CurrentSystem.runStep             =   int(StrainStop/(deltaT*CurrentSystem.elongRate));

					if (encore == 0)
					{
						ForWriteFile1           += (
						LogToPairModify (CurrentSystem) + 
						FormatString("read_data ", "left", 20) + CurrentSystem.readFolderName +  "ForElongation.dat   \n" +
						InteractionNormalLipidAshishCta (CurrentSystem.mainTop, CurrentSystem.soluteTop) +  
						VariableLxToScale (deltaT) + 
						FormatString("variable ", "left", 20) + FormatString("viscMultiplier ", "left", 20) + FormatString("equal", "left", 10) + "   " + dtos(CurrentSystem.multiplierForVisc) + " \n" +  
						TimeStepToThermoModify (deltaT)
						);
					}
					ForWriteFile1                    +=   TensionRelax (CurrentSystem, deltaT) + " \n";


					CurrentSystem.fileNameEnd         =   "UnLoad-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-En" + itos(encore+1);
					CurrentSystem.restartFreq         =   int(10000);
					CurrentSystem.trjDumpLong         =   int(10000);
					CurrentSystem.trjDumpShort        =   int(1000);
					CurrentSystem.logDump             =   int(100);
					CurrentSystem.runStep             =   int(RelaxTime/(deltaT*FemtoSecToNanosec));
					CurrentSystem.runType             =   "Relax";
					ForWriteFile1                    +=   TensionRelax (CurrentSystem, deltaT) + " \n";
				}//forstrain

				
				WriteFileName1   =  CurrentSystem.writeFolderName  + "Cycle-Rate" + dtos(CurrentSystem.rate) + "_" + CurrentSystem.direction[0] + ".pm";
				ofstream WriteFile1 (WriteFileName1.c_str());
				WriteFile1<<ForWriteFile1 <<"\n";
				WriteFile1.close();
					
				ForWriteFile2 += "cd    "  + RunSimulationFolderName + "\n" +
				"mpirun -np " + itos(RunCores) + " /usr/local/lammps-11Aug17/src/lmp_mpi -in " + WriteFileName1  + " -sf gpu -pk gpu 1  \n" + "\n";

			} //For Rate
			
		} //  For direction 
		
		WriteFileName2   =  CurrentSystem.writeFolderName + "runCycle_Chosen" + itos(chosen+1);
		ofstream WriteFile2 (WriteFileName2.c_str());
		WriteFile2<<ForWriteFile2 <<"\n";
		WriteFile2.close();
		
		
		ForWriteFile4 += "sh  " + WriteFileName2 + " \n";
		
	}//For Chosen
	
	WriteFileName4   =   LocatePC + "/Misc/runCycle_AllChosen";
	ofstream WriteFile4 (WriteFileName4.c_str());
	WriteFile4<<ForWriteFile4 <<"\n";
	WriteFile4.close();
	
	return 0;
}
 