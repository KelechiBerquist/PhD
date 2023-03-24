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
using std::cout;
using std::ifstream;
using std::ofstream;
using std::left;
using std::right;






#define PaInMPa          1e6 
#define FemtoSecInSec    1e15 

#define deltaT                        30
#define CommonMultipleLongTrj         0.5       
#define CommonMultipleShortTrj        0.02    
#define CommonMultipleLog             0.005
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

string ReadFolderName,   WriteFolderName,        SharedGromacsFiles;

string ReadFileName1,    ReadFileName2,    ReadFileName3,    ReadFileName4,    ReadFileName5;
string WriteFileName1,   WriteFileName2,   WriteFileName3,   WriteFileName4,   WriteFileName5;



int main ()
{
	
	// string  LocatePC = "/media/KaceHDD1/";
	string  LocatePC = "/media/OkwuchisHDD/";
	
	
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

	vector <double> StrainRate = {0.3, 3, 30};
	
	
	for (int chosen = 0;  chosen < MainVector.size();  chosen++)
	{
		mysystem CurrentSystem;
		
		
		for (int forDirection = 0;  forDirection < Direction.size();  forDirection++)
		{
			string Direct = string(1, Direction[forDirection][Direction[forDirection].size()-1] );
			CurrentSystem.mainTop   = MainVector[chosen];
			CurrentSystem.soluteTop = SoluteVector[chosen];
			
			CurrentSystem.readFolderName  = RunPC + itos(chosen+1) + "/" + Direct + "/Load/";
			CurrentSystem.writeFolderName = RunPC + itos(chosen+1) + "/" + Direct + "/RetrievedCoords/";

			WriteFileName1 =  CurrentSystem.writeFolderName + "RetrieveDat.pm";
			ofstream WriteFile1 (WriteFileName1.c_str());
				

			//for (int forRate = StrainRate.size()-1;  forRate >= 0 ;  forRate--)
			for (int forRate = 0;  forRate < StrainRate.size();  forRate++)
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
				
				
				
				
				CurrentSystem.runType == "Relax";
				
				for (int timeDump = StrainRestart;  timeDump < Strain; timeDump++)
				{
					int CurStrain  = timeDump;
					int timeDumpStep = timeDump*CurrentSystem.restartFreq;

					
					CurrentSystem.readFileNameEnd  = "Load-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-En" + itos(encore) + "-" + itos(timeDumpStep) ;
					CurrentSystem.fileNameEnd  =  string(1,CurrentSystem.direction[0]) +".dat" "-Rate" + dtos(CurrentSystem.rate) + "-Spn" + itos(chosen+1) + "-S" + itos(CurStrain) ;
					
										
					WriteFile1<<FormatString("clear ", "left", 20)<<"   \n";
					
					WriteFile1<<LogToPairModify (CurrentSystem);
					WriteFile1<<FormatString("read_restart ", "left", 20)<<(CurrentSystem.readFolderName  + "Restart"+CurrentSystem.readFileNameEnd+".equil")<<"   \n";
					WriteFile1<<FormatString("write_data ", "left", 20)<<"InConfig_"<<CurrentSystem.fileNameEnd +".dat"<<"   \n";
					
					
					cout <<CurrentSystem.direction[0] <<"|" <<CurrentSystem.readFolderName <<"|"<<CurrentSystem.writeFolderName <<"|" <<CurrentSystem.rate <<"|" <<timeDump <<"     ";
					//cout <<dtosPW(CurStrain,1,2)<<"   ";
					//cout <<timeDump<<"|"<<StartStep[forRate]<<"   ";
					
				} //For Timedump
				
			} //For Rate

			cout<<"\n";
		
		WriteFile1.close();
			
		} //  For direction 
		
	}//For Chosen
	
	
	return 0;
}
 
