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
using std::to_string;




#define PaInMPa          1e6 
#define FemtoSecInSec    1e15 

#define deltaT                        30
#define CommonMultipleLongTrj         0.5       
#define CommonMultipleShortTrj        0.02    
#define CommonMultipleLog             0.005
#define CommonMultipleLogHyst         0.005
#define StrainRestart                 1
#define encore    2
#define Strain    5


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

std::string ReadFolderName,   WriteFolderName,  ReadWriteFolderName,    SharedGromacsFiles;
std::string ReadFileName1,    ReadFileName2,    ReadFileName3,    ReadFileName4,    ReadFileName5;
std::string WriteFileName1,   WriteFileName2,   WriteFileName3,   WriteFileName4,   WriteFileName5;


int main ()
{
	
	std::string  LocatePC = "/media/KaceHDD1/";
	// std::string  LocatePC = "/media/OkwuchisHDD/";
	
	
	std::string RunPC = LocatePC + "Struct/";
	int RunCores;
	
	
	if (LocatePC == "/media/KaceHDD1/")
	{
		RunCores = 6;
		// RunCores = 4;
		SharedGromacsFiles = "/home/kelechi/SharedGromacsFiles/";
	}
	if (LocatePC == "/media/OkwuchisHDD/")
	{
		RunCores = 3;
		SharedGromacsFiles = "/home/okwuchi/SharedGromacsFiles/";
	}
	
	
	int ACheckEnd = 2,  NCheckEnd = 2; 
	
	
	vector <string> Direction        = {"xyzX",  "yxzY"};
	vector <string> MainVector       = {"MCN",  "PCN",  "PCN", "CTA"};
	vector <string> SoluteVector     = {"Ota",  "Mcn",  "Mcn", "NaSal"};
	
	vector <int> MainBeadVector   = {McnBead,  PcnBead,  PcnBead,  CtaBead};
	vector <int> SoluteBeadVector = {OtaBead,  McnBead,  McnBead,  BntBead};
	vector <int> SystemMain       = {612,    64,     623,    598};
	vector <int> SystemSolute     = {99,     471,    96,     184};
	vector <int> SystemWater;

	vector <double> StrainRate = {0.3, 3, 30};
	
	
	WriteFileName4   =   RunPC + "/Misc/runRelaxContd_AllChosen";
	std::ofstream WriteFile4 (WriteFileName4.c_str());
	
	WriteFile4<<"#!/bin/bash \n";
	WriteFile4<<" \n \n";
	WriteFile4<<" \n \n";
	
	
	for (int chosen = 0;  chosen < MainVector.size();  chosen++)
	{

		for (int forDirection = 0;  forDirection < Direction.size() ;  forDirection++)
		{
			mysystem CurrentSystem;
			
			string Direct = string(1, Direction[forDirection][Direction[forDirection].size()-1] );

			
			if (Direct =="X") {
			   SystemWater = {291500*10/11, 303600*10/11, 405900*10/11, 533390*10/11};
			}
			else{
				SystemWater   = {290950*10/11, 292600*10/11, 405900*10/11, 529100*10/11};
			}


			string  RunSimulationFolderName   =  RunPC + itos(chosen+1) + "/" + Direct + "/UnLoadContd/";
			CurrentSystem.readFolderName  = RunPC + itos(chosen+1) + "/" + Direct + "/UnLoad/";
			CurrentSystem.writeFolderName = RunPC + itos(chosen+1) + "/BashScripts/";

			CurrentSystem.direction = Direction[forDirection];
			CurrentSystem.mainTop   = MainVector[chosen];
			CurrentSystem.soluteTop = SoluteVector[chosen];
			
			CurrentSystem.mainBead   =  MainBeadVector[chosen];
			CurrentSystem.soluteBead =  SoluteBeadVector[chosen];
			CurrentSystem.mainChain  =  SystemMain[chosen];
			CurrentSystem.soluteChain=  SystemSolute[chosen];  
			
			CurrentSystem.WA   =   SystemWater[chosen];  
			CurrentSystem.AFW  =   0.1*SystemWater[chosen];  
			CurrentSystem.WAf  =   CurrentSystem.WA  +  CurrentSystem.AFW;
			
			CurrentSystem.runType  = "Relax";
			
			AssignConfig (CurrentSystem);
			
			
			WriteFileName2   =  CurrentSystem.writeFolderName + "runRelaxContd_Chosen" + "_" +  Direct + itos(chosen+1);
			std::ofstream WriteFile2 (WriteFileName2.c_str());
			
			WriteFile2<<"#!/bin/bash \n";
			WriteFile2<<" \n \n";
			WriteFile2<<" \n \n";
		
			
			for (int forRate = 0;  forRate < StrainRate.size();  forRate++)
			{
				CurrentSystem.rate      = StrainRate[forRate];
				CurrentSystem.elongRate = CurrentSystem.rate * 1e-6;
				CurrentSystem.multiplierForVisc = PaInMPa /(CurrentSystem.elongRate * FemtoSecInSec);
				
				
				CurrentSystem.restartFreq  = 5e4;
				CurrentSystem.trjDumpLong  = 5e4;
				CurrentSystem.trjDumpShort = 1e4;
				CurrentSystem.logDump = ceil(CommonMultipleLogHyst/(deltaT*CurrentSystem.elongRate));
				CurrentSystem.runStep = 15e5;
				
				
				
				for (int timeDump = StrainRestart;  timeDump <= Strain; timeDump+=2)
				{
					int CurStrain  = timeDump;
					
					CurrentSystem.readFileNameEnd  = "UnLoad-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-S" + itos(CurStrain);
					CurrentSystem.fileNameEnd  = "UnLoadContd-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-S" + itos(CurStrain);
					
					cout<<string(1, CurrentSystem.direction[0])<<"   ";
					WriteFileName3 = CurrentSystem.readFolderName + "Restart" + CurrentSystem.readFileNameEnd + "-500000.equil";
					
					
					WriteFileName1   =  CurrentSystem.writeFolderName  + "RelaxContd-Rate" + dtos(CurrentSystem.rate) + "_S" + itos(CurStrain) + "_" + CurrentSystem.direction[0] + ".pm";
					ofstream WriteFile1 (WriteFileName1.c_str());
					
					
					WriteFile1<<LogToPairModify (CurrentSystem);
					WriteFile1<<FormatString("#read_data ", "left", 20)<<WriteFileName3<<"   \n";
					WriteFile1<<FormatString("read_restart ", "left", 20)<<WriteFileName3<<"   \n";
					WriteFile1<<InteractionNormalLipidAshishCta (CurrentSystem.mainTop, CurrentSystem.soluteTop);
					WriteFile1<<VariableLxToScale (deltaT);
					WriteFile1<<FormatString("variable ", "left", 20)<<FormatString("viscMultiplier ", "left", 20)   <<FormatString("equal", "left", 10)<<"   "<<CurrentSystem.multiplierForVisc <<" \n";
					WriteFile1<<TimeStepToThermoModify (deltaT);
					
					
					WriteFile1<<TensionRelax (CurrentSystem, deltaT);
					
					WriteFile1.close();
										
					
					WriteFile2<<"cd    " <<RunSimulationFolderName <<"\n";
					WriteFile2<<"mpirun -np "<<RunCores <<" /usr/local/lammps-11Aug17/src/lmp_mpi -in "<<WriteFileName1 <<" -sf gpu -pk gpu 1  \n";
					WriteFile2<<"\n";
					
				} //for Strain
			} //For Rate
			
			WriteFile2.close();
		} //  For direction 
		
		
		
		
		WriteFile4 <<"sh  " <<WriteFileName2 <<" \n";
		
	}//For Chosen
	
	
	WriteFile4.close();
	
	
	return 0;
}
