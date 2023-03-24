#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

#include "main.h"
#include "OtherFunctions.h"
#include "../CommonFunctions.h"



//1
void AssignmentSequence (mySystem &CurrSystem)
{
	CurrSystem.mainChaB            =   CurrSystem.mainChain * CurrSystem.mainBead; 
	CurrSystem.counterMainChain    =   CurrSystem.mainChain;
	
	CurrSystem.soluteChaB          =  CurrSystem.soluteChain *  CurrSystem.soluteBead;
	CurrSystem.counterSoluteChain  =  CurrSystem.soluteChain;    
	 
	CurrSystem.chaB                =   CurrSystem.mainChaB   +  CurrSystem.soluteChaB; 
	CurrSystem.size                =   CurrSystem.chaB + CurrSystem.counterSoluteChain +  CurrSystem.counterMainChain  + CurrSystem.cumm1;
	
	
	CurrSystem.cumm2 = CurrSystem.cumm1+CurrSystem.mainChaB;    
	CurrSystem.cumm3 = CurrSystem.cumm2+CurrSystem.counterMainChain;
	CurrSystem.cumm4 = CurrSystem.cumm3+CurrSystem.soluteChaB;                                 
	CurrSystem.cumm5 = CurrSystem.cumm4+CurrSystem.counterSoluteChain;
	
	CurrSystem.fileHeader = 2;
	
	
	if (CurrSystem.soluteChain > 0)
	{
		CurrSystem.surfacIndexSize  =  (CurrSystem.mainBead-1)*CurrSystem.mainChain + (CurrSystem.soluteBead-1)*CurrSystem.soluteChain; 
	}
	if (CurrSystem.soluteChain == 0)
	{ 
		CurrSystem.surfacIndexSize  =  (CurrSystem.mainBead-1)*CurrSystem.mainChain;
	}
	
	
	CurrSystem.AllClusters.clear();
	CurrSystem.AllAtoms.clear();
	CurrSystem.VarSurfacIndex.clear();
	
	
	CurrSystem.AllAtoms.resize(CurrSystem.size - CurrSystem.cumm1);
	CurrSystem.VarSurfacIndex.resize(CurrSystem.surfacIndexSize,0);
}



//2
void PopulateBeadTypeLipids (mySystem &CurrSystem)
{
	int index = CurrSystem.countAllBead;
	
	int checkMain     = CurrSystem.countAllBead+1;
	int checkMainType = checkMain%CurrSystem.mainBead;
	
	int checkSolute     = CurrSystem.countAllBead+1-CurrSystem.cumm3+CurrSystem.cumm1;
	int checkSoluteType = checkSolute%CurrSystem.soluteBead;
	
	
	
		 if (CurrSystem.countAllBead < CurrSystem.cumm2-CurrSystem.cumm1 and checkMainType == 1)   { CurrSystem.AllAtoms[index].beadType = "Bead3";}
	else if (CurrSystem.countAllBead < CurrSystem.cumm2-CurrSystem.cumm1 and checkMainType != 1)   { CurrSystem.AllAtoms[index].beadType = "Bead4";}
	
	
	else if (CurrSystem.countAllBead < CurrSystem.cumm3-CurrSystem.cumm1)    { CurrSystem.AllAtoms[index].beadType = "Bead5";}
	
	
	else if (CurrSystem.countAllBead < CurrSystem.cumm4-CurrSystem.cumm1 and CurrSystem.soluteBead > 1 and checkSoluteType == 1) { CurrSystem.AllAtoms[index].beadType = "Bead6";}
	else if (CurrSystem.countAllBead < CurrSystem.cumm4-CurrSystem.cumm1 and CurrSystem.soluteBead > 1 and checkSoluteType != 1) { CurrSystem.AllAtoms[index].beadType = "Bead7";}
	
	
	else     { CurrSystem.AllAtoms[index].beadType = "Bead8";}
	
}



//3
void ReadFileSequence (mySystem &CurrSystem, std::string line)
{
	CurrSystem.countLineInFile++;
	
	float f1, f2, f3;
	
	int index1 = CurrSystem.fileHeader+CurrSystem.cumm1;
	int index2 = CurrSystem.fileHeader+CurrSystem.size;
	
	int index = CurrSystem.countAllBead;
	
	
	if(CurrSystem.countLineInFile > index1 and CurrSystem.countLineInFile <= index2)
	
	{
		PopulateBeadTypeLipids (CurrSystem);
		
		if (CurrSystem.AllAtoms[index].beadType == "Bead4" or CurrSystem.AllAtoms[index].beadType == "Bead7")
		{ 
			CurrSystem.VarSurfacIndex[CurrSystem.countCtaFat++] = CurrSystem.countAllBead;
		}
		
		
		std::string TruncLine = line.substr(20,24);
		std::istringstream iss(TruncLine);
		
		if(iss>>f1>>f2>>f3)
		{
			CurrSystem.AllAtoms[index].xCorr = f1*10;
			CurrSystem.AllAtoms[index].yCorr = f2*10;
			CurrSystem.AllAtoms[index].zCorr = f3*10;
			CurrSystem.AllAtoms[index].id    = CurrSystem.cumm1+CurrSystem.countAllBead+1;
			
			++CurrSystem.countAllBead;
		}
	}
	
	else if (CurrSystem.countLineInFile > index2)
	{
		std::istringstream iss(line);
		
		if(iss>>f1>>f2>>f3)
		{
			CurrSystem.xBoxLength = f1*10;
			CurrSystem.yBoxLength = f2*10;
			CurrSystem.zBoxLength = f3*10;
		}
	}
}



//4
void DataForWriting(mySystem &CurrSystem, int fix, cluster &tempCluster)
{
	//int index = fix-CurrSystem.cumm1;
	int index = fix;
	
	
	CurrSystem.allClusterInfo += (itosPW(index+1,0,8) +  "  " + itosPW(CurrSystem.micelleNumber,0,4) + "\n");
	
	tempCluster.dataXyz += (CurrSystem.AllAtoms[index].beadType + "  "
		+ ftosPW( CurrSystem.AllAtoms[index].xCorr,3,8)  + "  "
		+ ftosPW( CurrSystem.AllAtoms[index].yCorr,3,8)  + "  "
		+ ftosPW( CurrSystem.AllAtoms[index].zCorr,3,8)  + "  " 
		+ " \n");
		  
	
	tempCluster.dataIndexDat += (itosPW(CurrSystem.AllAtoms[index].id,0,8) + "\n" );
	tempCluster.dataIndexNdx += itosPW(CurrSystem.AllAtoms[index].id,0,8); 
	CurrSystem.countColumnIndex++;
	
	if ( CurrSystem.countColumnIndex == CurrSystem.indexFileCol)
	{
		tempCluster.dataIndexNdx += "\n";
		CurrSystem.countColumnIndex = 0;
	}
}



//5
void PlaceBead (mySystem &CurrSystem, int fix, cluster &tempCluster)
{
	//int index = fix - CurrSystem.cumm1;
	int index = fix;
	
	atom tempAtom; 
	
	tempAtom.id       = CurrSystem.AllAtoms[index].id;
	tempAtom.xCorr    = CurrSystem.AllAtoms[index].xCorr;
	tempAtom.yCorr    = CurrSystem.AllAtoms[index].yCorr;
	tempAtom.zCorr    = CurrSystem.AllAtoms[index].zCorr;
	tempAtom.beadType = CurrSystem.AllAtoms[index].beadType;
	
	tempCluster.content.push_back(tempAtom);
	
	
		 if (tempAtom.beadType == "Bead3") {tempCluster.mainCh +=1;}
	else if (tempAtom.beadType == "Bead4") {tempCluster.mainHy +=1;}
	
	else if (tempAtom.beadType == "Bead6") {tempCluster.soluteCh +=1;}
	else if (tempAtom.beadType == "Bead7") {tempCluster.soluteHy +=1;}
	
	
	DataForWriting(CurrSystem, fix, tempCluster);
}



//6
void FixCharged (mySystem &CurrSystem, int fix, cluster &tempCluster)
{
	int index = fix;
	//int index = fix-CurrSystem.cumm1;
	
	if ( CurrSystem.AllAtoms[index-1].beadType == "Bead3" or CurrSystem.AllAtoms[index-1].beadType == "Bead6") 
	{
		double x1 = CurrSystem.AllAtoms[index].xCorr;
		double y1 = CurrSystem.AllAtoms[index].yCorr;
		double z1 = CurrSystem.AllAtoms[index].zCorr;
		
		double x2 = CurrSystem.AllAtoms[index-1].xCorr;
		double y2 = CurrSystem.AllAtoms[index-1].yCorr;
		double z2 = CurrSystem.AllAtoms[index-1].zCorr;
		
		
		CurrSystem.AllAtoms[index-1].xCorr  = x1 + ReturnShortestDistance (x1, x2, CurrSystem.xBoxLength);
		CurrSystem.AllAtoms[index-1].yCorr  = y1 + ReturnShortestDistance (y1, y2, CurrSystem.yBoxLength);
		CurrSystem.AllAtoms[index-1].zCorr  = z1 + ReturnShortestDistance (z1, z2, CurrSystem.zBoxLength);
		
		PlaceBead (CurrSystem, fix-1, tempCluster);
	}
}



//8
void FindMicelleAlgorithm (mySystem &CurrSystem)
{
	CurrSystem.micelleNumber = 0,       CurrSystem.countColumnIndex = 0;
	
	for (int a0 = 0;  a0 < CurrSystem.VarSurfacIndex.size();  a0++)
	{
		cluster tempCluster;
		tempCluster.name = ++CurrSystem.micelleNumber;
		
		std::string ForDataXyz = "", ForDataIndexDat = "", ForDataIndexNdx = "";
		
		int currentSurfac = CurrSystem.VarSurfacIndex[a0];
		CurrSystem.countColumnIndex = 0;
		
		
		std::vector <int> ChargedIndex,   CurrentMicelle;
		
		CurrentMicelle.push_back(currentSurfac);
		PlaceBead (CurrSystem, currentSurfac, tempCluster);
		FixCharged(CurrSystem, currentSurfac, tempCluster);
		
		CurrSystem.VarSurfacIndex.erase(CurrSystem.VarSurfacIndex.begin() + a0);
		
		for (int a1 = 0;  a1 < CurrentMicelle.size();  a1++)
		{
			for (int a2 = CurrSystem.VarSurfacIndex.size() - 1;  a2 >= 0;  a2--)
			{
				//Run Close Algorithm
				int index1 = CurrentMicelle[a1];
				int index2 = CurrSystem.VarSurfacIndex[a2];
				
				double x1 = CurrSystem.AllAtoms[index1].xCorr;
				double y1 = CurrSystem.AllAtoms[index1].yCorr;
				double z1 = CurrSystem.AllAtoms[index1].zCorr;
				
				double x2 = CurrSystem.AllAtoms[index2].xCorr;
				double y2 = CurrSystem.AllAtoms[index2].yCorr;
				double z2 = CurrSystem.AllAtoms[index2].zCorr;
				
				double xMin = ReturnShortestDistance (x1, x2, CurrSystem.xBoxLength);
				double yMin = ReturnShortestDistance (y1, y2, CurrSystem.yBoxLength);
				double zMin = ReturnShortestDistance (z1, z2, CurrSystem.zBoxLength);
				
				
				if (xMin*xMin + yMin*yMin + zMin*zMin <= CurrSystem.rCut)
				{
					CurrentMicelle.push_back(index2);
					
					CurrSystem.AllAtoms[index2].xCorr = x1 + xMin;
					CurrSystem.AllAtoms[index2].yCorr = y1 + yMin;
					CurrSystem.AllAtoms[index2].zCorr = z1 + zMin;
					
					PlaceBead (CurrSystem, index2, tempCluster);
					FixCharged(CurrSystem, index2, tempCluster);
					
					CurrSystem.VarSurfacIndex.erase(CurrSystem.VarSurfacIndex.begin() + a2);
				}
			} //For a2
		} //For a1
		
		
		//std::cout <<MainPhase<<" " <<SoluteType<<" " <<R[r]<<" "  <<CurrentMicelle.size()<<" " <<ChargedIndex.size()<<" " <<"\n";
		
		
		CurrSystem.AllClusters.push_back(tempCluster);
		
		ChargedIndex.clear(),     CurrentMicelle.clear();
	 
	} //For a0
}





//9
void SortClusters (mySystem &CurrSystem)
{
	//Sort the Micelle according to the size in descending order
	std::vector <atom> tempContent = {};
	int temp;
	std::string tempA;
	
	
	for(int i = 0;  i < CurrSystem.AllClusters.size();  i++)
	{
		for(int j = 0;  j < CurrSystem.AllClusters.size();  j++)
		{
			if(i != j && CurrSystem.AllClusters[i].content.size() > CurrSystem.AllClusters[j].content.size())
			{
				tempContent = {};
				
				tempContent                       =  CurrSystem.AllClusters[i].content;
				CurrSystem.AllClusters[i].content =  CurrSystem.AllClusters[j].content;
				CurrSystem.AllClusters[j].content =  tempContent;
				
				temp                            =  CurrSystem.AllClusters[i].name;
				CurrSystem.AllClusters[i].name  =  CurrSystem.AllClusters[j].name;
				CurrSystem.AllClusters[j].name  =  temp;
				
				temp                             =  CurrSystem.AllClusters[i].mainHy;
				CurrSystem.AllClusters[i].mainHy =  CurrSystem.AllClusters[j].mainHy;
				CurrSystem.AllClusters[j].mainHy =  temp;
				
				temp                             =  CurrSystem.AllClusters[i].mainCh;
				CurrSystem.AllClusters[i].mainCh =  CurrSystem.AllClusters[j].mainCh;
				CurrSystem.AllClusters[j].mainCh =  temp;
				
				temp                               =  CurrSystem.AllClusters[i].soluteHy;
				CurrSystem.AllClusters[i].soluteHy =  CurrSystem.AllClusters[j].soluteHy;
				CurrSystem.AllClusters[j].soluteHy =  temp;
				
				temp                               =  CurrSystem.AllClusters[i].soluteCh;
				CurrSystem.AllClusters[i].soluteCh =  CurrSystem.AllClusters[j].soluteCh;
				CurrSystem.AllClusters[j].soluteCh =  temp;
				
				
				tempA                             =  CurrSystem.AllClusters[i].dataXyz;
				CurrSystem.AllClusters[i].dataXyz =  CurrSystem.AllClusters[j].dataXyz;
				CurrSystem.AllClusters[j].dataXyz =  tempA;
				
				tempA                                  =  CurrSystem.AllClusters[i].dataIndexDat;
				CurrSystem.AllClusters[i].dataIndexDat =  CurrSystem.AllClusters[j].dataIndexDat;
				CurrSystem.AllClusters[j].dataIndexDat =  tempA;
				
				tempA                                  =  CurrSystem.AllClusters[i].dataIndexNdx;
				CurrSystem.AllClusters[i].dataIndexNdx =  CurrSystem.AllClusters[j].dataIndexNdx;
				CurrSystem.AllClusters[j].dataIndexNdx =  tempA;
			}
		}
	}
}


//10
void WriteToFiles (mySystem &CurrSystem)
{
	std::string OuterFileNameEnd = CurrSystem.mainTop + "-"  + CurrSystem.soluteTop + "-" + CurrSystem.rValue;
	
	std::string WriteFileName5 = CurrSystem.WriteFolder + "WStats-"  + OuterFileNameEnd + ".dat";
	std::ofstream WriteOutput5 (WriteFileName5.c_str());
	
	
	for (int micelleLoop = 0;  micelleLoop < CurrSystem.AllClusters.size();  micelleLoop++)
	{
		if ( CurrSystem.AllClusters[micelleLoop].content.size() >= CurrSystem.cutOffMicelleSize)
		{
			std::string FileNameEnd = OuterFileNameEnd + "_M" + ftos(micelleLoop+1);
			
			std::string WriteFileName1 = CurrSystem.WriteFolder + FileNameEnd + ".xyz";
			std::string WriteFileName2 = CurrSystem.WriteFolder + "Y-"    + FileNameEnd + ".dat";
			std::string WriteFileName3 = CurrSystem.WriteFolder + "ZNdx-" + FileNameEnd + ".ndx";
			
			
			std::ofstream WriteOutput1(WriteFileName1.c_str());
			std::ofstream WriteOutput2(WriteFileName2.c_str());
			std::ofstream WriteOutput3(WriteFileName3.c_str());
			
			
			WriteOutput1<<CurrSystem.AllClusters[micelleLoop].content.size()<<"  "  <<"\n";
			WriteOutput1<<CurrSystem.xBoxLength<<"  " <<CurrSystem.yBoxLength<<"  " <<CurrSystem.zBoxLength<<"  "  <<"\n";
			WriteOutput3<<"[ Micelle"<<micelleLoop+1<<" ]"  <<"\n";
			
			WriteOutput1<<CurrSystem.AllClusters[micelleLoop].dataXyz;
			WriteOutput2<<CurrSystem.AllClusters[micelleLoop].dataIndexDat;
			WriteOutput3<<CurrSystem.AllClusters[micelleLoop].dataIndexNdx;
			
			
			WriteOutput1.close();
			WriteOutput2.close();
			WriteOutput3.close();
			
			
			double mainHyNumber   = CurrSystem.AllClusters[micelleLoop].mainHy*pow(CurrSystem.mainBead-1,-1);
			double soluteHyNumber = CurrSystem.AllClusters[micelleLoop].soluteHy*pow(CurrSystem.soluteBead-1,-1);
			
			double mainChNumber   =  CurrSystem.AllClusters[micelleLoop].mainCh;
			double soluteChNumber =  CurrSystem.AllClusters[micelleLoop].soluteCh;
			
			double mainHyChRatio    =  mainHyNumber*pow(mainChNumber,-1);
			double soluteHyChRatio  =  soluteHyNumber*pow(soluteChNumber,-1);
			
			double mainSoluteHyRatio = mainHyNumber*pow(soluteHyNumber,-1);
			double mainSoluteChRatio = mainChNumber*pow(soluteChNumber,-1);
		
			WriteOutput5<<CurrSystem.mainTop<<":"<<CurrSystem.soluteTop<<"="<<CurrSystem.r<<" "
			<<std::left<<"M:"         <<std::setw(5) <<itosPW(micelleLoop+1,0,3)<<" "
			<<std::left<<"ClustSize:" <<std::setw(10)<<itosPW(CurrSystem.AllClusters[micelleLoop].content.size(),0,3) <<" "
			<<std::left<<"MHy#:"      <<std::setw(8) <<itosPW(mainHyNumber,0,3) <<" " 
			<<std::left<<"MCh#:"      <<std::setw(8) <<itosPW(mainChNumber,0,3) <<" " 
			<<std::left<<"SHy#:"      <<std::setw(8) <<itosPW(soluteHyNumber,0,3) <<" " 
			<<std::left<<"SCh#:"      <<std::setw(8) <<itosPW(soluteChNumber,0,3)<<" "
			<<std::left<<"MHy/Ch="    <<std::setw(5) <<ftosPW(mainHyChRatio,2,3) <<" "   
			<<std::left<<"SHy/Ch#:"   <<std::setw(5) <<ftosPW(soluteHyChRatio,2,3) <<" " 
			<<std::left<<"M/SHy="     <<std::setw(5) <<ftosPW(mainSoluteHyRatio,2,3) <<" "        
			<<std::left<<"M/SCh#:"    <<std::setw(5) <<ftosPW(mainSoluteChRatio,2,3) <<" "
			<<"\n";
			
			
		} //If Large
	} //Micelle Loop
	
	
	WriteOutput5.close();
	
	
	std::string WriteFileName4 = CurrSystem.WriteFolder + "XClusterInfo-"  + OuterFileNameEnd + ".dat";
	std::ofstream WriteOutput4 (WriteFileName4.c_str());
	
	WriteOutput4<<CurrSystem.allClusterInfo;
	
	WriteOutput4.close();
}
