#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <set>
#include <list>
#include <ctime>
#include <deque>
#include <tuple>

//using std::tuple;
//using std::vector;
//using std::list;
//using std::deque;
//using std::string;
////using namespace std;
//using std::cout;
//using std::swap;



#include "main.h"
#include "OtherFunctions.h"
#include "CommonFunctions.h"



#define AFW    40000     
#define WA     400000
#define WAf    WA + AFW

#define TimeStep       1
#define WritngNum      5

#define StartTimeStep   0           
#define EndTimeStep     1              
#define SkipTimeStep    5


int i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12;

float  f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;

std::string s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12;
std::string rValue,  SoluteTop,  MainTop,  Folder,  ReadFolderName,  WriteFolderName;
std::string ReadFileName1,   ReadFileName2,   ReadFileName3,   ReadFileName4,   ReadFileName5,   ReadFileName6;
std::string WriteFileName1,  WriteFileName2,  WriteFileName3,  WriteFileName4,  WriteFileName5,  WriteFileName6;


std::vector <int> MainChainVector  = {2};
std::vector <int> MainBeadVector   = {2, 4, 6};
std::vector <int> SoluteBeadVector = {1, 2, 3, 4, 6};


std::vector <double> R = {0.3, 1.0, 3.0};

std::vector <std::string> SystemIndex  =  {"PN", "ZZ"};

std::string  LocatePC = "/media/okwuchi/Kay3TBMob/Lipids/";




bool fncomp (int lhs, int rhs)
{
	return lhs<rhs;
}

struct classcomp  {  bool operator() (const int& lhs, const int& rhs) const{ return lhs<rhs;} };


template <typename T> 
const bool Contains( std::vector<T>& Vec, const T& Element ) 
{
    if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
        return true;

    return false;
}



int main()
{
	for (int iMainChain = 0;  iMainChain < MainChainVector.size();  iMainChain++)
	{
		
		for (int iMainBead = 0;  iMainBead < MainBeadVector.size();  iMainBead++)
		{
			for (int iSystemIndex = 0;  iSystemIndex < SystemIndex.size();  iSystemIndex++)
			{
				MainTop  =  "B" + itos(MainBeadVector[iMainBead]) + SystemIndex[iSystemIndex][0];
				
				
				//for (int iSoluteBead = 0;  iSoluteBead < SoluteBeadVector.size();  iSoluteBead++)
				for (int iSoluteBead = SoluteBeadVector.size()-1;  iSoluteBead >= 0;  iSoluteBead--)
				{
					SoluteTop  =  "B" + itos(SoluteBeadVector[iSoluteBead]) + SystemIndex[iSystemIndex][1];
					
					clock_t begin1 = clock();
					
					for (int r = 0;  r < R.size();  r++)
					//for (int r = R.size()-1;  r >= 0;  r--)
					{
						rValue = dtosPW(R[r],1,3);
						
						Folder  =  LocatePC + itosPW(MainChainVector[iMainChain],1,0) + "/" + MainTop +  "/" + SoluteTop + "/" + rValue + "/" + rValue  ;
						
						ReadFolderName   =  Folder + "-EquilibriumGromacs/";
						WriteFolderName  =  Folder + "-StructuresInEquilibrium/";
						
						
						mySystem CurrSystem;
						
						
						CurrSystem.mainBead    = MainBeadVector[iMainBead] + 1;
						CurrSystem.mainChain   = MainChainVector[iMainChain]*1000;
						CurrSystem.soluteBead  = SoluteBeadVector[iSoluteBead] + 1;
						CurrSystem.soluteChain = int(R[r] * CurrSystem.mainChain);
						
						CurrSystem.r         = R[r];
						CurrSystem.rValue    = rValue;
						CurrSystem.mainTop   = MainTop;
						CurrSystem.soluteTop = SoluteTop;
						
						CurrSystem.cumm0 = WA;        
						CurrSystem.cumm1 = WA+ AFW;     
						CurrSystem.ReadFolder  = ReadFolderName;     
						CurrSystem.WriteFolder = WriteFolderName;     
						
						
						AssignmentSequence (CurrSystem);
						
						//std::cout <<"Before Reading File: MainTop:" <<MainTop <<"   SoluteTop:" <<SoluteTop<<"   R-Loop:" <<R[r] <<"   xBoxLength:"<<CurrSystem.xBoxLength<<"  "<<"  " <<"\n";
						
						
						
						ReadFileName1  = ReadFolderName + "MainNPT.gro";
						std::ifstream ReadFile1 (ReadFileName1.c_str());
						
						std::string line;
						
						while (std::getline(ReadFile1, line))
						{
							ReadFileSequence (CurrSystem, line);
							
						}
						ReadFile1.close();
						///// ********************* End Reading Xyz File ********************* /// 
						
						
						//std::cout <<"Chain:"<<MainChainVector[iMainChain]<<"   "  <<"MainTop:"<<MainTop<<"   "  <<"SoluteTop:"<<SoluteTop<<"   "  <<"Rvalue:"<<rValue<<"   " <<"TotalMicelle:"<<CurrSystem.micelleNumber<<"   " <<"Last line of File:"<<CurrSystem.AllAtoms[CurrSystem.AllAtoms.size()-1].xCorr <<" "<<CurrSystem.AllAtoms[CurrSystem.AllAtoms.size()-1].yCorr <<" "<<CurrSystem.AllAtoms[CurrSystem.AllAtoms.size()-1].zCorr <<" " <<"\n";
						
						//std::cout <<"Chain:"<<MainChainVector[iMainChain]<<"   "  <<"MainTop:"<<MainTop<<"   "  <<"SoluteTop:"<<SoluteTop<<"   "  <<"Rvalue:"<<rValue<<"   " <<"TotalMicelle:"<<CurrSystem.micelleNumber<<"   " <<"BoxLengths:"<<CurrSystem.xBoxLength <<" "<<CurrSystem.yBoxLength <<" "<<CurrSystem.zBoxLength <<" " <<"\n";
						
						
						clock_t begin2 = clock();
						
						//Curent Strategy: David"s Second Pop Algorithm
						FindMicelleAlgorithm (CurrSystem);
						
						
						//Sort Micelles
						SortClusters (CurrSystem);
						
						
						//Write To File
						WriteToFiles (CurrSystem);
						
						
						clock_t end2 = clock();
						double FirstElapsed_secs = double(end2 - begin2) / CLOCKS_PER_SEC;
						
						
						std::cout <<"Chain:"<<MainChainVector[iMainChain]<<"   "  <<"MainTop:"<<MainTop<<"   "  <<"SoluteTop:"<<SoluteTop<<"   "  <<"Rvalue:"<<rValue<<"   " <<"TotalMicelle:"<<CurrSystem.micelleNumber<<"   " <<"BottleNeckElapsedTimeInSecs:"<<FirstElapsed_secs <<"\n";
						
					} //For R
					
					clock_t end1 = clock();
					double SecondElapsed_secs = double(end1 - begin1) / CLOCKS_PER_SEC;
					std::cout <<"Chain:"<<MainChainVector[iMainChain]<<"   "  <<"MainTop:"<<MainTop<<"   "  <<"SoluteTop:"<<SoluteTop<<"   "  <<"Rvalue:"<<rValue<<"   " <<"TotalElapsedTimeInSecs:"<<SecondElapsed_secs<<"   " <<"\n";
					std::cout<<"\n \n";
					
				} //For SoluteBead
			} // For Index
		} //For MainBead
	} //For Chain
	
	return 0;
}
