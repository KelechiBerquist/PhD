#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h> 
#include <unordered_map>    
#include <algorithm>


#include "CommonFunctions.h"
#include "FixTrjFunctions.h"

using std::string;
using std::vector;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::pair;
using std::map;
using std::to_string;




#define x                       0
#define y                       1
#define z                       2
#define energy                  3
#define pairE                   4
#define bondE                   5
#define angleE                  6
#define lammpsTimeDump          2
#define lammpsTotalBead         4
#define lammpsXBox              6
#define lammpsYBox              7
#define lammpsZBox              8
#define lammpsHeadRow           9
#define clustCutOff             1000
#define longestMicelleLength    200
#define rCut                    9.0*9.0
#define initRate                0.3
#define skipStrain              0.5
#define timeStep                30
#define nanosecToSec            1e9
#define femtosecToSec           1e15
#define convDeformRate          1e-6


int    i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12;
float  f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;	
string s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, WriteFileName, ReadFileName;


float ShortDist(float x1, float x2, float x3)
{
	vector <float> Dist    = {x2-x1-x3,  x2-x1,  x2-x1+x3};
	float shortestDistance = Dist [0];

	for (int a = 1;  a < Dist.size();  a++)
	{
		if (abs(shortestDistance) > abs(Dist[a]))
		{
			shortestDistance = Dist [a];
		}
	}
	return shortestDistance;
	



	// vector <float> Dist    = {x2-x1-x3,  x2-x1,  x2-x1+x3};
	// float shortestDistance = Dist [0];

	// for (int a = 1;  a < Dist.size();  a++)
	// {
	// 	if (shortestDistance*shortestDistance > Dist[a]*Dist[a])
	// 	{
	// 		shortestDistance = Dist [a];
	// 	}
	// }
	// return shortestDistance;
}
// 
// 
// 
void AddFix (int timeDmp, int indx, vector <float> Coords,  eachCluster &Item)
{ 
	Item.dump[timeDmp].atomFix[indx].rx = Coords[x];
	Item.dump[timeDmp].atomFix[indx].ry = Coords[y];
	Item.dump[timeDmp].atomFix[indx].rz = Coords[z];
}
// 
// 
// 
string WriteTraj (int timeDmp, int indx, eachLayer &ThisLayer, eachCluster &ThisCluster, std::vector <float> Cod)
{
	float xFix = 0,     yFix = 0,     zFix = 0;

	if (ThisLayer.direction == "Y" and ThisLayer.chosen != 4 and ThisLayer.chosen != 3)
	{
		xFix = Cod[y]; 
		yFix = Cod[x] + ThisLayer.forRate*longestMicelleLength; 
	}
	else
	{
		xFix = Cod[x]; 
		yFix = Cod[y] + ThisLayer.forRate*longestMicelleLength; 
	}
	zFix = Cod[z];

	if (ThisCluster.dump[timeDmp].MinCoord[x] < Cod[x]){ ThisCluster.dump[timeDmp].MinCoord[x] = Cod[x]; }
	if (ThisCluster.dump[timeDmp].MinCoord[y] < Cod[y]){ ThisCluster.dump[timeDmp].MinCoord[y] = Cod[y]; }
	if (ThisCluster.dump[timeDmp].MinCoord[z] < Cod[z]){ ThisCluster.dump[timeDmp].MinCoord[z] = Cod[z]; }
	
	if (ThisCluster.dump[timeDmp].MaxCoord[x] > Cod[x]){ ThisCluster.dump[timeDmp].MaxCoord[x] = Cod[x]; }
	if (ThisCluster.dump[timeDmp].MaxCoord[y] > Cod[y]){ ThisCluster.dump[timeDmp].MaxCoord[y] = Cod[y]; }
	if (ThisCluster.dump[timeDmp].MaxCoord[z] > Cod[z]){ ThisCluster.dump[timeDmp].MaxCoord[z] = Cod[z]; }

	return (itos(indx) + "    " + "Bd" + itos(ThisCluster.dump[timeDmp].atomOrig[indx].type) + "    "  +
		ftos(xFix) + "    "  + ftos(yFix) + "    "  + ftos(zFix) + "    "  +
		ftos(0.0)   + "    "  + ftos(0.0)   + "    "  + ftos(0.0)   + "    "  + "\n");
}
// 
// 
// 
string WriteEnergy (int timeDump, std::vector <float> Coords, std::string atomType)
{
	float thatX    =   Coords[x],
	      thatY    =   Coords[y],
	      thatZ    =   Coords[z],
	      r        =   pow(thatX*thatX+thatY*thatY+thatZ*thatZ, 0.5);
	      
	return (itos(timeDump) + "    " + ftos(r) + "    " + ftos(thatX) + "    " + ftos(thatY) + "    " + ftos(thatZ) + "    " + atomType + "    "  + ftos(Coords[energy])   + "    " + ftos(Coords[pairE])   + "     "+ ftos(Coords[bondE])   + "    " + ftos(Coords[angleE])   + "    "  + "\n");
}
// 
// 
// 
void FindEachMicelle(eachLayer &ThisLayer, eachCluster &ThisClusterW)
{
	vector <int> GdCluster   =   {};

	while (ThisClusterW.VarSurfac.size() > 0)
	{
		int k0 = 0;
		GdCluster.push_back(ThisClusterW.VarSurfac[k0]);

		float thatX = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].rx,
		      thatY = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].ry,
		      thatZ = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].rz;

		AddFix  (0, ThisClusterW.VarSurfac[k0], {thatX, thatY, thatZ}, ThisClusterW);

		ThisLayer.writeTrj    += WriteTraj (0, ThisClusterW.VarSurfac[k0], ThisLayer, ThisClusterW, {thatX, thatY, thatZ});

		ThisClusterW.VarSurfac.erase(ThisClusterW.VarSurfac.begin()+k0);


		for (int k1 = 0;  k1 < GdCluster.size();  k1++)
		{	
			float x1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].rx,
			      y1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].ry,
			      z1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].rz;

			for (int k2 = ThisClusterW.VarSurfac.size()-1;  k2 >= 0;  k2--)
			{
				float x2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].rx,
				      y2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].ry,
				      z2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].rz,
				      xShort   =  ShortDist(x1, x2, ThisClusterW.dump[0].BoxLength[x]), 
				      yShort   =  ShortDist(y1, y2, ThisClusterW.dump[0].BoxLength[y]), 
				      zShort   =  ShortDist(z1, z2, ThisClusterW.dump[0].BoxLength[z]);

			
				if (xShort*xShort + yShort*yShort + zShort*zShort <= rCut)
				{
					GdCluster.push_back(ThisClusterW.VarSurfac[k2]);

					ThisLayer.writeTrj    += WriteTraj (0, ThisClusterW.VarSurfac[k2], ThisLayer, ThisClusterW, {x1+xShort, y1+yShort, z1+zShort});
					
					AddFix  (0, ThisClusterW.VarSurfac[k2], {x1+xShort, y1+yShort,  z1+zShort}, ThisClusterW);
					ThisClusterW.VarSurfac.erase(ThisClusterW.VarSurfac.begin()+k2);
				}
			}
		}

		cout<<"GdCluster.size(): " <<GdCluster.size()<<" "   <<ThisClusterW.VarSurfac.size()<<" " <<"\n";

		if (GdCluster.size() >= clustCutOff)
		{
			ThisLayer.writeTrj = WritePreTrj (ThisClusterW.dump[0].header, GdCluster.size()) + ThisLayer.writeTrj;
			FixOtherStep(ThisLayer, ThisClusterW, GdCluster);
		}

		GdCluster.clear();
	}
}
// 
// 
// 
std::string WritePreTrj (std::vector <std::string> HeaderVector, int ClusterSize)
{
	std::string preTrj = "";

	for (int hIdx = 0;  hIdx < HeaderVector.size();  hIdx++)
	{
		if (hIdx+1 != lammpsTotalBead) { preTrj +=  HeaderVector[hIdx];}
		else                           { preTrj +=  itos(ClusterSize) + "\n";}
	}

	return  preTrj;
}
// 
// 
// 
void FixOtherStep(eachLayer &ThisLayer, eachCluster &ThisClusterW, std::vector <int> GdCluster)
{	
	for (int k0 = 1;  k0 < ThisClusterW.dump.size();  k0++)
	{
		ThisLayer.writeTrj += WritePreTrj (ThisClusterW.dump[k0].header, GdCluster.size());

		for (int it = 0;  it < GdCluster.size();  it++)
		{
			eachBead CoordOrigW1  =   ThisClusterW.dump[k0-1].atomOrig[GdCluster[it]];
			eachBead CoordOrigW2  =   ThisClusterW.dump[k0].atomOrig[GdCluster[it]];
			eachBead CoordFixW1   =   ThisClusterW.dump[k0-1].atomFix[GdCluster[it]];

			
			float thatX   =   CoordFixW1.rx + ShortDist(CoordFixW1.rx, CoordOrigW2.rx, ThisClusterW.dump[k0].BoxLength[x]),
			      thatY   =   CoordFixW1.ry + ShortDist(CoordFixW1.ry, CoordOrigW2.ry, ThisClusterW.dump[k0].BoxLength[y]),
			      thatZ   =   CoordFixW1.rz + ShortDist(CoordFixW1.rz, CoordOrigW2.rz, ThisClusterW.dump[k0].BoxLength[z]);

			
			AddFix (k0, GdCluster[it], {thatX, thatY, thatZ},  ThisClusterW);
			ThisLayer.writeTrj    += WriteTraj (k0, GdCluster[it], ThisLayer, ThisClusterW, {thatX, thatY, thatZ});
		}	
	}

	FileWriteTraj (ThisLayer);
}
// 
// 
// 
eachCluster GetData(eachLayer &ThisLayer, std::string FileBegin)
{
	eachCluster ThisCluster;


	ReadFileName =  ThisLayer.readFolderName + FileBegin + ThisLayer.readFileNameEnd + ".lammpstrj";
	ifstream ReadFile (ReadFileName.c_str());

	string line, 
	       UpdateLine,
	       NewDumpString = "ITEM: TIMESTEP";
	
	int countLine = 0,   
	    DumpTime  = 0,
	    checkLine = 0,   
	    checkBead = 0;

	eachDump ThisDump;
	eachBead ThisBead;


	while (std::getline(ReadFile, line))
	{
		countLine++;

		if (line.find(NewDumpString) < line.length())
		{
			ThisCluster.totalTimeDump++;
			ThisDump   =   eachDump();
			ThisBead   =   eachBead();
		}

		if (countLine == lammpsTotalBead)
		{
			ThisCluster.totalBead = stoi(line);
		}


		checkLine = countLine - (ThisCluster.totalBead + lammpsHeadRow) * (ThisCluster.totalTimeDump-1);
		checkBead = checkLine - lammpsHeadRow;

		if (checkLine == lammpsTimeDump)
		{
			DumpTime = stoi(line);
			ThisCluster.StepAtDump.push_back(stoi(line));
		}

		if (checkLine >= lammpsXBox and checkLine <= lammpsZBox)
		{
			istringstream iss(line);
			if (iss>>f1>>f2)
			{
				ThisDump.BoxLength.push_back(f2 - f1);
			}
		}

		if (checkLine <= lammpsHeadRow)
		{
			ThisDump.header.push_back(line + "\n");
			UpdateLine += line + "\n";
		}


		if (checkBead >= 1 and checkBead <= ThisCluster.totalBead)
		{
			istringstream iss(line);
			if (iss>>i1>>i2>>f1>>f2>>f3>>f4>>f5>>f6)
			{
				ThisBead.indx    =   i1;
				ThisBead.type    =   i2;
				ThisBead.rx      =   f1;
				ThisBead.ry      =   f2;
				ThisBead.rz      =   f3;

				ThisDump.atomOrig[i1]  =   ThisBead;
				ThisDump.atomFix[i1]   =   ThisBead;
				ThisBead               =   eachBead();

				if (ThisCluster.totalTimeDump == 1)
				{
					ThisCluster.VarSurfac.push_back(i1);
				}

				if (checkBead == ThisCluster.totalBead)
				{
					ThisCluster.dump.push_back(ThisDump);
					ThisCluster.Header.push_back(UpdateLine);
					UpdateLine = "";
				}
			}
		}
	}
	ReadFile.close();

	return ThisCluster;
}
// 
//
//
void GetEnergy(eachLayer &ThisLayer)
{
	ReadFileName =  ThisLayer.readFolderName + "MicelleUnWrap" + ThisLayer.readFileNameEnd + ".lammpstrj";
	ifstream ReadFile (ReadFileName.c_str());

	string line, 
	       UpdateLine,
	       NewDumpString = "ITEM: TIMESTEP";
	
	int countLine            =   0,   
	    DumpTime             =   0,
	    checkLine            =   0, 
	    thisTotalBead        =   0,   
	    thisTotalTimeDump    =   0,   
	    checkBead            =   0;


    float totalSum = 0,   
	      pairSum  = 0,
	      bondSum  = 0,   
	      angleSum = 0;


	while (std::getline(ReadFile, line))
	{
		countLine++;

		if (line.find(NewDumpString) < line.length())
		{
			thisTotalTimeDump++;
		}

		if (countLine == lammpsTotalBead)
		{
			thisTotalBead = stoi(line);
		}


		checkLine = countLine - (thisTotalBead + lammpsHeadRow) * (thisTotalTimeDump-1);
		checkBead = checkLine - lammpsHeadRow;

		if (checkLine == lammpsTimeDump)
		{
			DumpTime = stoi(line);
		}

		if (checkBead >= 1 and checkBead <= thisTotalBead)
		{
			istringstream iss(line);
			if (iss>>i1>>i2>>f1>>f2>>f3>>f4>>f5>>f6)
			{
				totalSum += f4+f5+f6,   
			    pairSum  += f4,
			    bondSum  += f5,   
			    angleSum += f6;

				ThisLayer.writeEnergy += WriteEnergy (DumpTime, {f1, f2, f3, f4+f5+f6, f4, f5, f6}, "Bd" + to_string(i2));

				if (checkBead == thisTotalBead)
				{
					ThisLayer.writeTotalEnergy += (itos(DumpTime) + "   " + ftos(totalSum) + "   " + ftos(pairSum) + "   " + ftos(bondSum) + "   " + ftos(angleSum) + "\n");

					totalSum = 0,   
					pairSum  = 0,
					bondSum  = 0,   
					angleSum = 0;
				}
			}
		}
	}
	ReadFile.close();
	FileWriteEnergy (ThisLayer);
}
// 
// 
// 
void FileWriteTraj (eachLayer &ThisLayer)
{
	WriteFileName = ThisLayer.writeFolderName1 + ThisLayer.writeFileNameEnd + "-" + ThisLayer.direction + ".lammpstrj";
	ofstream WriteFile1(WriteFileName.c_str());
	WriteFile1<<ThisLayer.writeTrj;
	WriteFile1.close();
}
// 
// 
// 
void FileWriteEnergy (eachLayer &ThisLayer)
{
	WriteFileName = ThisLayer.writeFolderName1 + "PerAtomEnergy" + ThisLayer.writeFileNameEnd + "-" + ThisLayer.direction + ".dat";
	ofstream WriteFile1(WriteFileName.c_str());
	WriteFile1<<ThisLayer.writeEnergy;
	WriteFile1.close();

	WriteFileName = ThisLayer.writeFolderName1 + "Energy" + ThisLayer.writeFileNameEnd + "-" + ThisLayer.direction + ".dat";
	ofstream WriteFile2(WriteFileName.c_str());
	WriteFile2<<ThisLayer.writeTotalEnergy;
	WriteFile2.close();
}


// void FindFixWriteMicelle(eachLayer &ThisLayer, eachCluster &ThisClusterW, eachCluster &ThisClusterU)
// {
// 	vector <int> GdCluster   =   {};
// 	ThisLayer.writeTrj      +=   ThisClusterW.Header[0];

// 	// Fix Trj In First Time Dump
// 	// for (int k0 = 0;  k0 < ThisCluster.VarSurfac.size();  k0++)
// 	while (ThisClusterW.VarSurfac.size() > 0)
// 	{
// 		int k0 = 0;
// 		GdCluster.push_back(ThisClusterW.VarSurfac[k0]);

// 		float thatX = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].rx,
// 		      thatY = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].ry,
// 		      thatZ = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].rz;

// 		AddFix  (0, ThisClusterW.VarSurfac[k0], {thatX, thatY, thatZ}, ThisClusterW);

// 		ThisLayer.writeTrj    += WriteTraj (0, ThisClusterW.VarSurfac[k0], ThisLayer, ThisClusterW, {thatX, thatY, thatZ});

// 		ThisClusterW.VarSurfac.erase(ThisClusterW.VarSurfac.begin()+k0);



		
// 		for (int k1 = 0;  k1 < GdCluster.size();  k1++)
// 		{	
// 			float x1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].rx,
// 			      y1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].ry,
// 			      z1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].rz;

// 			for (int k2 = ThisClusterW.VarSurfac.size()-1;  k2 >= 0;  k2--)
// 			{
// 				float x2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].rx,
// 				      y2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].ry,
// 				      z2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].rz,
// 				      xShort   =  ShortDist(x1, x2, ThisClusterW.dump[0].BoxLength[x]), 
// 				      yShort   =  ShortDist(y1, y2, ThisClusterW.dump[0].BoxLength[y]), 
// 				      zShort   =  ShortDist(z1, z2, ThisClusterW.dump[0].BoxLength[z]);

			
// 				if (xShort*xShort + yShort*yShort + zShort*zShort <= rCut)
// 				{
// 					GdCluster.push_back(ThisClusterW.VarSurfac[k2]);

// 					ThisLayer.writeTrj    += WriteTraj (0, ThisClusterW.VarSurfac[k2], ThisLayer, ThisClusterW, {x1+xShort, y1+yShort, z1+zShort});
					
// 					AddFix  (0, ThisClusterW.VarSurfac[k2], {x1+xShort, y1+yShort,  z1+zShort}, ThisClusterW);
// 					ThisClusterW.VarSurfac.erase(ThisClusterW.VarSurfac.begin()+k2);
// 				}
// 			}
// 		}

// 		cout<<"GdCluster.size(): " <<GdCluster.size()<<" "   <<ThisClusterW.VarSurfac.size()<<" " <<"\n";
// 		GdCluster.clear();
// 	}


// 	// Fix Trj In Other Time Dump
// 	for (int k0 = 1;  k0 < ThisClusterW.dump.size();  k0++)
// 	{
// 		ThisLayer.writeTrj += ThisClusterW.Header[k0];

// 		std::unordered_map <int, eachBead>::iterator it = ThisClusterW.dump[k0].atomOrig.begin();

		
		

// 		for (; it != ThisClusterW.dump[k0].atomOrig.end(); ++it)
// 		{
			
// 			eachBead CoordOrigW1  =   ThisClusterW.dump[k0-1].atomOrig[it->first];
// 			eachBead CoordOrigW2  =   ThisClusterW.dump[k0].atomOrig[it->first];
// 			eachBead CoordFixW1   =   ThisClusterW.dump[k0-1].atomFix[it->first];

// 			// eachBead CoordOrigU1  =   ThisClusterU.dump[k0-1].atomOrig[it->first];
// 			// eachBead CoordOrigU2  =   ThisClusterU.dump[k0].atomOrig[it->first];
// 			// eachBead CoordFixU1   =   ThisClusterU.dump[k0-1].atomFix[it->first];

			
// 			float thatX1   =   CoordFixW1.rx + ShortDist(CoordFixW1.rx, CoordOrigW2.rx, ThisClusterW.dump[k0].BoxLength[x]),
// 			      thatY1   =   CoordFixW1.ry + ShortDist(CoordFixW1.ry, CoordOrigW2.ry, ThisClusterW.dump[k0].BoxLength[y]),
// 			      thatZ1   =   CoordFixW1.rz + ShortDist(CoordFixW1.rz, CoordOrigW2.rz, ThisClusterW.dump[k0].BoxLength[z]);

// 			// float thatX2   =   CoordFixU1.rx + ShortDist(CoordFixU1.rx, CoordOrigU2.rx, ThisClusterU.dump[k0].BoxLength[x]),
// 			//       thatY2   =   CoordFixU1.ry + ShortDist(CoordFixU1.ry, CoordOrigU2.ry, ThisClusterU.dump[k0].BoxLength[y]),
// 			//       thatZ2   =   CoordFixU1.rz + ShortDist(CoordFixU1.rz, CoordOrigU2.rz, ThisClusterU.dump[k0].BoxLength[z]);



// 			float thatX    =   thatX1,
// 			      thatY    =   thatY1,
// 			      thatZ    =   thatZ1;

// 			// float thatX    =   std::min(thatX1, thatX2),
// 			//       thatY    =   std::min(thatY1, thatY2),
// 			//       thatZ    =   std::min(thatZ1, thatZ2);



// 			AddFix (k0, it->first, {thatX, thatY, thatZ},  ThisClusterW);
// 			ThisLayer.writeTrj    += WriteTraj (k0, it->first, ThisLayer, ThisClusterW, {thatX, thatY, thatZ});
// 		}	
// 	}

// 	FileWriteTraj (ThisLayer);
// }
// // 
// //
// //
// *****************************************************************************



// void FindFixWriteMicelle(eachLayer &ThisLayer, eachCluster &ThisClusterW, eachCluster &ThisClusterU)
// {
// 	vector <int> GdCluster   =   {};
// 	ThisLayer.writeTrj      +=   ThisClusterW.Header[0];

// 	// Fix Trj In First Time Dump
// 	// for (int k0 = 0;  k0 < ThisCluster.VarSurfac.size();  k0++)
// 	while (ThisClusterW.VarSurfac.size() > 0)
// 	{
// 		int k0 = 0;
// 		GdCluster.push_back(ThisClusterW.VarSurfac[k0]);

// 		float thatX = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].rx,
// 		      thatY = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].ry,
// 		      thatZ = ThisClusterW.dump[0].atomFix[ThisClusterW.VarSurfac[k0]].rz;

// 		AddFix  (0, ThisClusterW.VarSurfac[k0], {thatX, thatY, thatZ}, ThisClusterW);

// 		ThisLayer.writeTrj    += WriteTraj (0, ThisClusterW.VarSurfac[k0], ThisLayer, ThisClusterW, {thatX, thatY, thatZ});

// 		ThisClusterW.VarSurfac.erase(ThisClusterW.VarSurfac.begin()+k0);



		
// 		for (int k1 = 0;  k1 < GdCluster.size();  k1++)
// 		{	
// 			float x1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].rx,
// 			      y1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].ry,
// 			      z1 = ThisClusterW.dump[0].atomFix[GdCluster[k1]].rz;

// 			for (int k2 = ThisClusterW.VarSurfac.size()-1;  k2 >= 0;  k2--)
// 			{
// 				float x2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].rx,
// 				      y2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].ry,
// 				      z2       =  ThisClusterW.dump[0].atomOrig[ThisClusterW.VarSurfac[k2]].rz,
// 				      xShort   =  ShortDist(x1, x2, ThisClusterW.dump[0].BoxLength[x]), 
// 				      yShort   =  ShortDist(y1, y2, ThisClusterW.dump[0].BoxLength[y]), 
// 				      zShort   =  ShortDist(z1, z2, ThisClusterW.dump[0].BoxLength[z]);

			
// 				if (xShort*xShort + yShort*yShort + zShort*zShort <= rCut)
// 				{
// 					GdCluster.push_back(ThisClusterW.VarSurfac[k2]);

// 					ThisLayer.writeTrj    += WriteTraj (0, ThisClusterW.VarSurfac[k2], ThisLayer, ThisClusterW, {x1+xShort, y1+yShort, z1+zShort});
					
// 					AddFix  (0, ThisClusterW.VarSurfac[k2], {x1+xShort, y1+yShort,  z1+zShort}, ThisClusterW);
// 					ThisClusterW.VarSurfac.erase(ThisClusterW.VarSurfac.begin()+k2);
// 				}
// 			}
// 		}

// 		cout<<"GdCluster.size(): " <<GdCluster.size()<<" "   <<ThisClusterW.VarSurfac.size()<<" " <<"\n";
// 		GdCluster.clear();
// 	}


// 	// Fix Trj In Other Time Dump
// 	for (int k0 = 1;  k0 < ThisClusterW.dump.size();  k0++)
// 	{
// 		ThisLayer.writeTrj += ThisClusterW.Header[k0];

// 		std::unordered_map <int, eachBead>::iterator it = ThisClusterW.dump[k0].atomOrig.begin();

		
		

// 		for (; it != ThisClusterW.dump[k0].atomOrig.end(); ++it)
// 		{
			
// 			eachBead CoordOrigW1  =   ThisClusterW.dump[k0-1].atomOrig[it->first];
// 			eachBead CoordOrigW2  =   ThisClusterW.dump[k0].atomOrig[it->first];
// 			eachBead CoordFixW1   =   ThisClusterW.dump[k0-1].atomFix[it->first];

// 			// eachBead CoordOrigU1  =   ThisClusterU.dump[k0-1].atomOrig[it->first];
// 			// eachBead CoordOrigU2  =   ThisClusterU.dump[k0].atomOrig[it->first];
// 			// eachBead CoordFixU1   =   ThisClusterU.dump[k0-1].atomFix[it->first];

			
// 			float thatX1   =   CoordFixW1.rx + ShortDist(CoordFixW1.rx, CoordOrigW2.rx, ThisClusterW.dump[k0].BoxLength[x]),
// 			      thatY1   =   CoordFixW1.ry + ShortDist(CoordFixW1.ry, CoordOrigW2.ry, ThisClusterW.dump[k0].BoxLength[y]),
// 			      thatZ1   =   CoordFixW1.rz + ShortDist(CoordFixW1.rz, CoordOrigW2.rz, ThisClusterW.dump[k0].BoxLength[z]);

// 			// float thatX2   =   CoordFixU1.rx + ShortDist(CoordFixU1.rx, CoordOrigU2.rx, ThisClusterU.dump[k0].BoxLength[x]),
// 			//       thatY2   =   CoordFixU1.ry + ShortDist(CoordFixU1.ry, CoordOrigU2.ry, ThisClusterU.dump[k0].BoxLength[y]),
// 			//       thatZ2   =   CoordFixU1.rz + ShortDist(CoordFixU1.rz, CoordOrigU2.rz, ThisClusterU.dump[k0].BoxLength[z]);



// 			float thatX    =   thatX1,
// 			      thatY    =   thatY1,
// 			      thatZ    =   thatZ1;

// 			// float thatX    =   std::min(thatX1, thatX2),
// 			//       thatY    =   std::min(thatY1, thatY2),
// 			//       thatZ    =   std::min(thatZ1, thatZ2);



// 			AddFix (k0, it->first, {thatX, thatY, thatZ},  ThisClusterW);
// 			ThisLayer.writeTrj    += WriteTraj (k0, it->first, ThisLayer, ThisClusterW, {thatX, thatY, thatZ});
// 		}	
// 	}

// 	FileWriteTraj (ThisLayer);
// }
// // 
// //
// //
