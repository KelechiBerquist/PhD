#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>


#include "../CommonFunctions.h"
#include "WritingLammpsFiles.h"
#include "InteractionParameters.h"


using std::vector;
using std::string;
using std::string;
using std::cout;
using std::ofstream;
using std::ifstream;


#define PaInMPa          1e6 
#define FemtoSecInSec    1e15 

#define deltaT                        30
#define CommonMultipleLongTrj         0.5       
#define CommonMultipleShortTrj        0.02    
#define CommonMultipleLog             0.01
#define CommonMultipleLogHyst         0.001
#define StrainRestart                 1
#define encore    1
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
	
	vector <string> Direction        = {"xyzX",  "yxzY"};
	vector <string> MainVector       = {"MCN",  "PCN",  "PCN", "CTA"};
	vector <string> SoluteVector     = {"Ota",  "Mcn",  "Mcn", "NaSal"};
	
	vector <int> MainBeadVector   = {McnBead,  PcnBead,  PcnBead,  CtaBead};
	vector <int> SoluteBeadVector = {OtaBead,  McnBead,  McnBead,  BntBead};
	vector <int> SystemMain       = {612,    64,     623,    598};
	vector <int> SystemSolute     = {99,     471,    96,     184};
	vector <int> SystemWater;

	vector <double> StrainRate = {0.1, 0.3, 1, 3, 10, 30};
	
	
	WriteFileName4   =   RunPC + "/Misc/runElong_AllChosen";
	ofstream WriteFile4 (WriteFileName4.c_str());
	
	WriteFile4<<"#!/bin/bash \n";
	WriteFile4<<" \n \n";
	WriteFile4<<" \n \n";
	
	
	for (int chosen = 0;  chosen < MainVector.size();  chosen++)
	{
		mysystem CurrentSystem;
		CurrentSystem.mainTop   = MainVector[chosen];
		CurrentSystem.soluteTop = SoluteVector[chosen];
		CurrentSystem.writeFolderName = RunPC + itos(chosen+1) + "/BashScripts/";


		WriteFileName2   =  CurrentSystem.writeFolderName + "runElong_Chosen" + itos(chosen+1);
		ofstream WriteFile2 (WriteFileName2.c_str());
		
		WriteFile2<<"#!/bin/bash \n";
		WriteFile2<<" \n \n";
		WriteFile2<<" \n \n";

		for (int forDirection = 0;  forDirection < Direction.size() ;  forDirection++)
		{
			

			string Direct = string(1, Direction[forDirection][Direction[forDirection].size()-1] );
			
			if (Direct =="X") {
			   SystemWater = {291500*10/11, 303600*10/11, 405900*10/11, 533390*10/11};
			}
			else{
				SystemWater   = {290950*10/11, 292600*10/11, 405900*10/11, 529100*10/11};
			}


			
			CurrentSystem.readFolderName  = RunPC + itos(chosen+1) + "/" +  Direct + "/Prelim/";
			CurrentSystem.runType  = "Tension";
			
			string  RunSimulationFolderName   =  RunPC + itos(chosen+1) + "/" + Direct + "/Load/";

			cout <<CurrentSystem.writeFolderName<<"|"<<CurrentSystem.readFolderName<<"|"<<Direct<<"   \n";


			
			
			
			for (int forRate = StrainRate.size()-1;  forRate >= 0 ;  forRate--)
			{
				CurrentSystem.rate      = StrainRate[forRate];
				CurrentSystem.elongRate = CurrentSystem.rate * 1e-6;
				CurrentSystem.multiplierForVisc = PaInMPa /(CurrentSystem.elongRate * FemtoSecInSec);
				
				CurrentSystem.restartFreq  = ceil(StrainRestart/(deltaT*CurrentSystem.elongRate));
				CurrentSystem.trjDumpLong  = ceil(CommonMultipleLongTrj/(deltaT*CurrentSystem.elongRate));
				CurrentSystem.trjDumpShort = ceil(CommonMultipleShortTrj/(deltaT*CurrentSystem.elongRate));
				CurrentSystem.logDump = ceil(CommonMultipleLog/(deltaT*CurrentSystem.elongRate));
				CurrentSystem.runStep = ceil(Strain/(deltaT*CurrentSystem.elongRate));
				
				
				CurrentSystem.direction  = Direction[forDirection];


				
				WriteFileName1   =  CurrentSystem.writeFolderName  + "Elong-Rate" + dtos(CurrentSystem.rate) + "_" + CurrentSystem.direction[0] + ".pm";
				// WriteFileName1   =  CurrentSystem.writeFolderName  + "Elong-Rate" + dtos(CurrentSystem.rate) + "_" + CurrentSystem.direction[0] + "_2.pm";
				ofstream WriteFile1 (WriteFileName1.c_str());
				
				

				CurrentSystem.fileNameEnd= "Load-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-En" + itos(encore);

				CurrentSystem.readFileNameEnd = "1-" + itos(chosen+1);
				// CurrentSystem.readFileNameEnd = "Load-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-En" + itos(encore-1);
				
				
				WriteFile1<<LogToPairModify (CurrentSystem);
				WriteFile1<<FormatString("read_data ", "left", 20)<<CurrentSystem.readFolderName<<"ForElongation.dat   \n";
				WriteFile1<<FormatString("#read_data ", "left", 20)<<RunSimulationFolderName<<"WrittenData"<<CurrentSystem.readFileNameEnd<<"-"<<CurrentSystem.runStep<<".dat   \n";

				WriteFile1<<InteractionNormalLipidAshishCta (CurrentSystem.mainTop, CurrentSystem.soluteTop);
				WriteFile1<<VariableLxToScale (deltaT);
				WriteFile1<<FormatString("variable ", "left", 20)<<FormatString("viscMultiplier ", "left", 20)   <<FormatString("equal", "left", 10)<<"   "<<CurrentSystem.multiplierForVisc <<" \n";
				WriteFile1<<TimeStepToThermoModify (deltaT);
				
				WriteFile1<<FormatString("#run ", "left", 20)  <<"2000 \n";
				WriteFile1<<FormatString("#unfix ", "left", 20) <<"ensemble \n";
				WriteFile1<<TensionRelax (CurrentSystem, deltaT);
				
				WriteFile1.close();
				
				
				WriteFile2<<"cd    " <<RunSimulationFolderName <<"\n";
				WriteFile2<<"mpirun -np "<<RunCores <<" /usr/local/lammps-11Aug17/src/lmp_mpi -in "<<WriteFileName1 <<" -sf gpu -pk gpu 1  \n";
				WriteFile2<<"\n";
			} //For Rate
			
		} //  For direction 
		
		WriteFile2.close();
		
		
		
		
		WriteFile4 <<"sh  " <<WriteFileName2 <<" \n";
		
	}//For Chosen
	
	
	WriteFile4.close();
	
	
	return 0;
}
 
				//double Rate       =  StrainRate[forRate];
				//double ElongRate  =  Rate * 1e-6;
				
				
				//double ElongRateInSec     =  ElongRate * FemtoSecInSec;
				//double CurrentSystem.multiplierForVisc  =  PaInMPa / ElongRateInSec;
				
				
				
				//int RestartFreq      = ceil(StrainRestart/(deltaT*ElongRate));
				//int TrjDumpLong      = ceil(CommonMultipleLongTrj/(deltaT*ElongRate));
				//int TrjDumpShort     = ceil(CommonMultipleShortTrj/(deltaT*ElongRate));
				//int LogDump          = ceil(CommonMultipleLog/(deltaT*ElongRate));
				//int LogHystDump      = ceil(CommonMultipleLogHyst/(deltaT*ElongRate));
				//int RunStep          = ceil(Strain/(deltaT*ElongRate));
				
				
				
				
				
				
