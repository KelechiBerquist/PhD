#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm> 


#include "../CommonFunctions.h"
#include "WritingLammpsFiles.h"
#include "InteractionParameters.h"



using std::vector;
using std::string;
using std::string;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::left;
using std::right;



//This is an updated lammps script that puts Water first, Antifreeze water next, Cta and CounterMainChain 3rd and 4th, Solute and CounterSoluteChain 5th and 6th.


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

#define deltaT       30



float  xBoxLength,       yBoxLength,       zBoxLength;
string SoluteType,       ReadWriteFolderName,    SharedGromacsFiles;
string ReadFolderName,   WriteFolderName,        ReadWriteFolderName2;

string ReadFileName1,   ReadFileName2,   ReadFileName3,   ReadFileName4,   ReadFileName5;

string WriteFileName1,   WriteFileName2,   WriteFileName3,   WriteFileName4,   WriteFileName5;
	

int main ()
{
	//This reverses The order of CTA i.e. Q0 = Bead5 and Type 3; C1 and C2 are Beads 1-4 and condensed to type 4.
	
	
	string  LocatePC = "/media/KaceHDD1/";
	//string  LocatePC = "/media/OkwuchisHDD/";
	
	
	int RunCores;
	string RunPC = LocatePC + "Struct/";
	
	if (LocatePC == "/media/KaceHDD1/")
	{
		RunCores = 6;
		//RunCores = 3;
		SharedGromacsFiles = "/home/kelechi/SharedGromacsFiles/";
	}
	if (LocatePC == "/media/OkwuchisHDD/")
	{
		RunCores = 4;
		SharedGromacsFiles = "/home/okwuchi/SharedGromacsFiles/";
	}
	
	
	
	int ACheckEnd = 2,  NCheckEnd = 2; 
	
	vector <string> Direction   = {"xyzX",  "yxzY"};
	vector <string> MainVector  = {"MCN",  "PCN",  "PCN", "CTA"};
	vector <string> SoluteVector= {"Ota",  "Mcn",  "Mcn", "NaSal"};
	vector <int> MainBeadVector  = {McnBead,  PcnBead,  PcnBead,  CtaBead};
	vector <int> SoluteBeadVector= {OtaBead,  McnBead,  McnBead,  BntBead};
	vector <int> SystemMain   = {612,    64,     623,    598};
	vector <int> SystemSolute = {99,     471,    96,     184};
	vector <int> SystemWater;
	
	
	
	WriteFileName4   =    RunPC + "Misc/runAllChosenEquilibriumLammps";
	ofstream WriteFile4 (WriteFileName4.c_str());
	
	WriteFile4<<"#!/bin/bash \n";
	WriteFile4<<" \n \n";
	
	
	
	for (int chosen = 0;  chosen < MainVector.size();  chosen++)
	{
		for (int forDirection = 0;  forDirection < Direction.size() ;  forDirection++)
		{
			string Direct = string(1, Direction[forDirection][Direction[forDirection].size()-1] );

			if (Direct == "X") 
			{
			    SystemWater = {291500*10/11, 303600*10/11, 405900*10/11, 533390*10/11};
			}
			else{
				SystemWater   = {290950*10/11, 292600*10/11, 405900*10/11, 529100*10/11};
			}


			mysystem CurrentSystem;
			
			CurrentSystem.mainTop   = MainVector[chosen];
			CurrentSystem.soluteTop = SoluteVector[chosen];
			
			CurrentSystem.mainBead   =  MainBeadVector[chosen];
			CurrentSystem.soluteBead =  SoluteBeadVector[chosen];
			CurrentSystem.mainChain  =  SystemMain[chosen];
			CurrentSystem.soluteChain  =   SystemSolute[chosen];  
			
			CurrentSystem.WA     =   SystemWater[chosen];  
			CurrentSystem.AFW    =   0.1*SystemWater[chosen];  
			CurrentSystem.WAf    =   CurrentSystem.WA  +  CurrentSystem.AFW;

			
			

			CurrentSystem.writeFolderName = RunPC + itos(chosen+1) + "/" +  Direct + "/Prelim/";
			CurrentSystem.readFolderName  = RunPC + itos(chosen+1) + "/" + Direct + "/Prelim/";
			

			cout <<CurrentSystem.writeFolderName<<"|"<<CurrentSystem.readFolderName<<"|"<<Direct<<"   \n";
			
			CurrentSystem.runType = "Equilibrium";
			
			
			AssignConfig (CurrentSystem);
			
			GetCoordConfig (CurrentSystem);
			
			int timeDump = 0;
			WriteBeadInConfig (CurrentSystem, timeDump);
			
			WriteBondInConfig (CurrentSystem, timeDump);
			
			WriteAngleInConfig (CurrentSystem, timeDump);
			
			TopConfig (CurrentSystem, timeDump);
			
			
			WriteFileName2  = CurrentSystem.writeFolderName + "InConfig.dat";
			ofstream WriteFile2 (WriteFileName2.c_str());
			
			WriteFile2<<CurrentSystem.MultiTrj[timeDump].topInConfig;
			WriteFile2<<CurrentSystem.MultiTrj[timeDump].beadsInConfig;
			WriteFile2<<CurrentSystem.MultiTrj[timeDump].bondsInConfig;
			WriteFile2<<CurrentSystem.MultiTrj[timeDump].anglesInConfig;
			WriteFile2.close();
			
			
			
			for (float restart = 1;  restart < 2;  restart++)
			{
				int TrjDump  =  50000;
				
				
				CurrentSystem.fileNameEnd = ftos(restart) + "-" + itos(chosen+1);
				
				
				WriteFileName1 =  CurrentSystem.writeFolderName + "Equilibrium.pm";
				ofstream WriteFile1 (WriteFileName1.c_str());
				
				
				WriteFile1<<LogToPairModify (CurrentSystem);
				WriteFile1<<FormatString("read_data ", "left", 20)<<CurrentSystem.readFolderName<<"InConfig.dat   \n";
				WriteFile1<<InteractionNormalLipidAshishCta (CurrentSystem.mainTop, CurrentSystem.soluteTop);
				WriteFile1<<VariableLxToScale (deltaT);
				WriteFile1<<TimeStepToThermoModify (deltaT);
				
				WriteFile1<<FormatString("dump ","left",20) <<FormatString("m1 ","left",5)<<FormatString("all","left",5) <<FormatString("custom","left",10) <<FormatString(itos(TrjDump),"left",10) <<FormatString("Wrap"+CurrentSystem.fileNameEnd+".lammpstrj","left",25) <<"  id   type   x   y   z   vx   vy   vz \n";
				WriteFile1<<FormatString("dump ","left",20) <<FormatString("m2 ","left",5)<<FormatString("all","left",5) <<FormatString("custom","left",10) <<FormatString(itos(TrjDump),"left",10) <<FormatString("UnWrap"+CurrentSystem.fileNameEnd+".lammpstrj","left",25) <<"  id   type   xu   yu   zu   c_atomprop1   c_atomprop2   c_atomprop4 \n";
				
				WriteFile1<<FormatString("restart ","left",20) <<"100000  Restart" <<CurrentSystem.fileNameEnd <<"-*.equil \n"; 
				WriteFile1<<FormatString("run_style ","left",20) <<"verlet \n";
				WriteFile1<<FormatString("run ","left",20) <<itos(200000)<<" \n";
				WriteFile1<<FormatString("write_data ","left",20) <<"ForElongation.dat"<<" \n";
				
				WriteFile1.close();
				
			} // For Restart
			
			
			WriteFile4<<"cd    "<<CurrentSystem.writeFolderName<<" \n";
			WriteFile4<<"mpirun -np "<<RunCores <<" /usr/local/lammps-11Aug17/src/lmp_mpi -in "<<WriteFileName1 <<" -sf gpu -pk gpu 1  \n";
			WriteFile4<<" \n \n";
			
			
			//WriteFile3 <<"sh  " <<WriteFileName4 <<" \n \n";
			  
			//WriteFile4.close();
		} //For Main Phase
	}

	WriteFile4.close();
	//WriteFile3.close();
	
	return 0;
	
}
