#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>    
#include <cmath>  
#include <unordered_map>


#include "CommonFunctions.h"
#include "CalcHistogramFunctions.h"

using std::string;
using std::vector;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::pair;
using std::map;
using std::to_string;
using std::unordered_map;


#define _USE_MATH_DEFINES // for C++ 
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
#define longestMicelleLength    200
#define rCut                    9.0*9.0
#define initRate                0.3
// #define skipStrain              0.5
#define stopStrain              1.9
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
		if (shortestDistance*shortestDistance > Dist[a]*Dist[a])
		{
			shortestDistance = Dist [a];
		}
	}
	
	return shortestDistance;
}

// 
//
//
eachGroup GetTraj(eachLayer &ThisLayer,  std::string Mid)
{
	ReadFileName = ThisLayer.readFolderName + Mid + ThisLayer.readFileNameEnd  + ".lammpstrj";
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

    double checkTime         =   0;

    eachGroup ThisGroup;
   	eachDump  ThisDump;
   	eachBead  ThisBead;


	while (std::getline(ReadFile, line))
	{
		countLine++;

		if (line.find(NewDumpString) < line.length())
		{
			thisTotalTimeDump++;
			ThisBead   =   eachBead();
			ThisDump   =   eachDump();
		}

		if (countLine == lammpsTotalBead)
		{
			thisTotalBead        =  stoi(line);
			ThisGroup.totalBead  =  stoi(line);
		}
		

		checkLine    =   countLine - (thisTotalBead + lammpsHeadRow) * (thisTotalTimeDump-1);
		checkBead    =   checkLine - lammpsHeadRow;
		
		if (checkLine == lammpsTimeDump)
		{
			DumpTime     =   stoi(line);
			checkTime    =   fmod(DumpTime*timeStep*ThisLayer.rate*convDeformRate, ThisLayer.skipStrain)/ThisLayer.skipStrain;

			if (checkTime <= ThisLayer.skipCheckFactor or checkTime >= 1-ThisLayer.skipCheckFactor)
			// if (checkLine == lammpsTimeDump and (checkTime <= ThisLayer.skipCheckFactor or checkTime >= 1-ThisLayer.skipCheckFactor))
			{
				ThisLayer.StepAtDump.push_back(stoi(line));
				ThisDump.stepAtDump = stoi(line);
			}
		}


		// if ((iss>>i1>>i2>>f1>>f2>>f3>>f4>>f5>>f6 and Mid == "MicelleUnWrap") or  (iss>>i1>>i2>>f1>>f2>>f3 and Mid == "WaterIonUnWrap"))
		if (checkBead >= 1 and checkBead <= thisTotalBead and (checkTime <= ThisLayer.skipCheckFactor or checkTime >= 1-ThisLayer.skipCheckFactor))
		{
			istringstream iss(line);
		
			if (Mid == "MicelleUnWrap")
			{
				if ((iss>>i1>>i2>>f1>>f2>>f3>>f4>>f5>>f6) and i2 != 1) 
				{
					ThisBead.indx    =   i1;
					ThisBead.type    =   i2;
					ThisBead.rx      =   f1;
					ThisBead.ry      =   f2;
					ThisBead.rz      =   f3;
				}
			}
			else if (Mid == "WaterIonUnWrap")
			{
				if ((iss>>i1>>i2>>f1>>f2>>f3) and i2 != 1) 
				{
					ThisBead.indx    =   i1;
					ThisBead.type    =   i2;
					ThisBead.rx      =   f1;
					ThisBead.ry      =   f2;
					ThisBead.rz      =   f3;
				}
			}

			else if (Mid == "")
			{
				if ((iss>>i1>>s2>>f1>>f2>>f3>>f4>>f5>>f6) and i2 != 1) 
				{
					ThisBead.indx    =   i1;
					ThisBead.type    =   i2;
					ThisBead.rx      =   f1;
					ThisBead.ry      =   f2;
					ThisBead.rz      =   f3;
				}
			}

			ThisDump.atom.push_back(ThisBead);
			ThisBead = eachBead();
			

			if (checkBead == thisTotalBead)
			{	
				// cout<<checkTime <<" ";
				ThisGroup.dump.push_back(ThisDump);
				ThisBead   =   eachBead();
				ThisDump   =   eachDump();
			}
		}
	}
	ReadFile.close();

	return ThisGroup;
} 
// // 
// //
// //
void GetStrain(eachLayer &ThisLayer,  eachGroup Vec1)
{
	double firstLength = 0;
	for (int tDiff = 0;  Vec1.dump[tDiff].stepAtDump*timeStep*ThisLayer.rate*convDeformRate <= stopStrain;   tDiff++)
	{
		double currentMaxDist           =   0.1;
		int    ind1                     =   3,
		       quitSearch               =   20,
		       checkConv                =   7;

		vector <trackStrain> Info(quitSearch);
		Info[0].ix = Vec1.dump[tDiff].atom[0].rx,
		Info[0].iy = Vec1.dump[tDiff].atom[0].ry,
		Info[0].iz = Vec1.dump[tDiff].atom[0].rz;


		for (int rep = 0;  rep  < quitSearch;  rep++)
		{
			if (rep != 0)
			{
				Info[rep].ix = Info[rep-1].fx,
				Info[rep].iy = Info[rep-1].fy,
				Info[rep].iz = Info[rep-1].fz;
			}

			for (int ind2 = 0;  ind2  < Vec1.dump[tDiff].atom.size();  ind2++)
			{
				float diffX     =   Info[rep].ix - Vec1.dump[tDiff].atom[ind2].rx,
				      diffY     =   Info[rep].iy - Vec1.dump[tDiff].atom[ind2].ry,
				      diffZ     =   Info[rep].iz - Vec1.dump[tDiff].atom[ind2].rz,
				      diffSqR   =   diffX*diffX + diffY*diffY + diffZ*diffZ;

				if (diffSqR > Info[rep].dist)
				{
					Info[rep].dist  =  diffSqR,
					Info[rep].fx = Vec1.dump[tDiff].atom[ind2].rx,
					Info[rep].fy = Vec1.dump[tDiff].atom[ind2].ry,
					Info[rep].fz = Vec1.dump[tDiff].atom[ind2].rz;
				}
			}

			// cout<<Info[rep].ix<<","  <<Info[rep].iy<<","  <<Info[rep].iz<<";"  <<Info[rep].fx<<","  <<Info[rep].fy<<","  <<Info[rep].fz<<";"  <<Info[rep].dist<<" || ";
			// cout<<Info[rep].dist<<", "  <<pow(Info[rep].dist, 0.5)<<", ";

			if (rep > checkConv)
			{
				double checkDist = 0;
				for (int bla = quitSearch-checkConv;  bla < quitSearch;  bla++)
				{
					checkDist += Info[bla].dist;
				}
				if (checkDist == checkConv*Info[quitSearch-1].dist)
				{
					if (tDiff == 0)
					{
						firstLength = pow(Info[rep].dist, 0.5);
					}

					double currentLength = pow(Info[rep].dist, 0.5);

					ThisLayer.AllStrain += itos(Vec1.dump[tDiff].stepAtDump) + "  " + dtos(Vec1.dump[tDiff].stepAtDump*timeStep*ThisLayer.rate*convDeformRate) + "  " + dtos(currentLength) + "  " + dtos((currentLength-firstLength)/firstLength) + "\n";

					break;
				}
			}
		}
	}

	FileWriteStrain (ThisLayer);
}


void GetHistogram(eachLayer ThisLayer,  eachGroup Vec1,  eachGroup Vec2)
{
	// number_Of_Bins  = ceil((rangeMax - rangeMin)/binWidth) +1;
	// Calculated Histogram range =  rangeMin to rangeMin+(number_Of_Bins-1)*binWidth
	// Calculated Histogram range =  rangeMin to rangeMin+ceil((rangeMax - rangeMin)/binWidth)*binWidth

	// Given a value x, the bin number it should be is:
	// ceil((x - rangeMin)/binWidth) + 1

	// The actual upperlimit of the bin that value belongs in is
	// rangeMin + binWidth*ceil((x - rangeMin)/binWidth)

	// The radius of the spherical shell is the bin lowerLimit. The dr is the binWidth.


	double rangeMax       =   20,
	       rangeMin       =   0,
	       binWidth       =   0.1;
	int    typeNumber     =   10;

	

	// Order: timeDump | type | distance | f_1, f_2, f_3, f_4, f_5, f_6, f_7, f_8, f_9, f_10

	for (int tDiff = 0;  tDiff < (stopStrain/ThisLayer.skipStrain);   tDiff++)
	{
		// cout <<"Begin:" <<" S: " <<tDiff*ThisLayer.skipStrain <<" | ";
		// Order: type | distance | f_1, f_2, f_3, f_4, f_5, f_6, f_7, f_8, f_9, f_10

		eachHist   ThisHist;
		ThisHist.binWidth     =   0.2;
		ThisHist.startBin     =   0;
		ThisHist.stopBin      =   20;
		ThisHist.binNumber    =   ceil((ThisHist.stopBin - ThisHist.startBin)/ThisHist.binWidth ) + 1;

		ThisHist.type.resize(typeNumber);

		
		for (int ind1 = 0;  ind1  < Vec1.dump[tDiff].atom.size();  ind1++)
		{
			if (ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin.size() == 0)
		  	{
		  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bead = Vec1.dump[tDiff].atom[ind1].type;
		  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin.resize(ThisHist.binNumber);
		  	}

			for (int ind2 = ind1+1;  ind2  < Vec1.dump[tDiff].atom.size();  ind2++)
			{
				float diffX     =   Vec1.dump[tDiff].atom[ind1].rx - Vec1.dump[tDiff].atom[ind2].rx,
				      diffY     =   Vec1.dump[tDiff].atom[ind1].ry - Vec1.dump[tDiff].atom[ind2].ry,
				      diffZ     =   Vec1.dump[tDiff].atom[ind1].rz - Vec1.dump[tDiff].atom[ind2].rz,
				      diffSqR   =   diffX*diffX + diffY*diffY + diffZ*diffZ;

				if (diffSqR < rangeMax*rangeMax)
				{
					float diffR           =   pow(diffSqR, 0.5);
			    	int   thisBinNumber   =   ceil((diffR - ThisHist.startBin)/ThisHist.binWidth) + 1;
			    	float thisBinLimit    =   ThisHist.startBin + ThisHist.binWidth*(thisBinNumber-1);

				  	if (ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].freq.size() == 0)
				  	{
				  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].upperLimit = thisBinLimit;
				  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].lowerLimit = thisBinLimit - ThisHist.binWidth;
				  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].freq.resize(typeNumber, 0);
				  	}
				  	ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].freq[Vec1.dump[tDiff].atom[ind2].type-1] += 1;
				}
			}

			// cout <<"Mid:" <<" S: " <<tDiff*ThisLayer.skipStrain <<" | ";

			for (int ind2 = 0;  ind2  < Vec2.dump[tDiff].atom.size();  ind2++)
			{
				float diffX     =   Vec1.dump[tDiff].atom[ind1].rx - Vec2.dump[tDiff].atom[ind2].rx,
				      diffY     =   Vec1.dump[tDiff].atom[ind1].ry - Vec2.dump[tDiff].atom[ind2].ry,
				      diffZ     =   Vec1.dump[tDiff].atom[ind1].rz - Vec2.dump[tDiff].atom[ind2].rz,
				      diffSqR   =   diffX*diffX + diffY*diffY + diffZ*diffZ;

				if (diffSqR < rangeMax*rangeMax)
				{
					float diffR           =   pow(diffSqR, 0.5);
			    	int   thisBinNumber   =   ceil((diffR - ThisHist.startBin)/ThisHist.binWidth) + 1;
			    	float thisBinLimit    =   ThisHist.startBin + ThisHist.binWidth*(thisBinNumber-1);

				  	if (ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].freq.size() == 0)
				  	{
				  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].upperLimit = thisBinLimit;
				  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].lowerLimit = thisBinLimit - ThisHist.binWidth;
				  		ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].freq.resize(typeNumber, 0);
				  	}
				  	ThisHist.type[Vec1.dump[tDiff].atom[ind1].type-1].bin[thisBinNumber-1].freq[Vec2.dump[tDiff].atom[ind2].type-1] += 1;
				}
			}
		}

		ReadHistogram(tDiff,  ThisLayer,  ThisHist);
		ThisHist    =   eachHist();
	}
}
// // 
// //
// //
void ReadHistogram(int tDiff,  eachLayer ThisLayer,  eachHist MiniHistogram)
{
	// Order: timeDump | type | distance | f_1, f_2, f_3, f_4, f_5, f_6, f_7, f_8, f_9, f_10
	for (int ind1 = 0;  ind1  < MiniHistogram.type.size();  ind1++)
	{//For Ref Bead Type

		string ThisFileContent = "";
		string ThatFileContent = "";
		
		
		for (int ind2 = 0;  ind2  < MiniHistogram.type[ind1].bin.size();  ind2++)
		{//For Distance between bead
			
			float binUpperLimit = MiniHistogram.type[ind1].bin[ind2].upperLimit,
			      binLowerLimit = MiniHistogram.type[ind1].bin[ind2].lowerLimit;
			if (MiniHistogram.type[ind1].bin[ind2].upperLimit == 0)
			{
				ThisFileContent += ftos(MiniHistogram.startBin + MiniHistogram.binWidth*ind2) + "  ";
				ThatFileContent += ftos(MiniHistogram.startBin + MiniHistogram.binWidth*ind2) + "  ";
			}
			else
			{
				ThisFileContent += ftos(binUpperLimit) + "  ";
				ThatFileContent += ftos(binUpperLimit) + "  ";
			}


			if (MiniHistogram.type[ind1].bin[ind2].freq.size() != 0)
			{
				for (auto thisThat : MiniHistogram.type[ind1].bin[ind2].freq)
				{//for Freq of Beads Within distance
					ThisFileContent += ftos(thisThat/(2*binLowerLimit*binLowerLimit*M_PI*MiniHistogram.binWidth)) + "  ";
					ThatFileContent += ftos(thisThat) + "  ";
				}
			}
			else
			{
				for (int thisThat = 0;  thisThat < MiniHistogram.type.size(); thisThat++)
				{//for Freq of Beads Within distance
					ThisFileContent += ftos(0) + "  ";
					ThatFileContent += ftos(0) + "  ";
				}
			}

			ThisFileContent += "  \n";
			ThatFileContent += "  \n";
		}

		if (MiniHistogram.type[ind1].bin.size() != 0)
		{
			string WriteFileName = ThisLayer.writeFolderName1 + "Gofr-Bd" + itos(MiniHistogram.type[ind1].bead) + "-"  + ThisLayer.readFileNameEnd + "-S" + dtos(tDiff*ThisLayer.skipStrain)  + "-" + ThisLayer.direction + ".dat";
			ofstream WriteFile1(WriteFileName.c_str());
			WriteFile1<<ThisFileContent;
			WriteFile1.close();

			WriteFileName = ThisLayer.writeFolderName1 + "Hist-Bd" + itos(MiniHistogram.type[ind1].bead) + "-"  + ThisLayer.readFileNameEnd + "-S" + dtos(tDiff*ThisLayer.skipStrain)  + "-" + ThisLayer.direction + ".dat";
			ofstream WriteFile2(WriteFileName.c_str());
			WriteFile2<<ThatFileContent;
			WriteFile2.close();
		}
	}
}
// 
//
//
eachOrderedGroup GetOrderedTraj(eachLayer &ThisLayer,  std::string Mid)
{
	ReadFileName = ThisLayer.readFolderName + Mid + ThisLayer.readFileNameEnd  + ".lammpstrj";
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
    double checkTime         =   0;

    eachOrderedGroup ThisGroup;
   	eachOrderedDump  ThisDump;
   	eachBead         ThisBead;


	while (std::getline(ReadFile, line))
	{
		countLine++;

		if (line.find(NewDumpString) < line.length())
		{
			thisTotalTimeDump++;
			ThisBead   =   eachBead();
			ThisDump   =   eachOrderedDump();
		}

		if (countLine == lammpsTotalBead)
		{
			thisTotalBead        =  stoi(line);
			ThisGroup.totalBead  =  stoi(line);
		}

		if (checkLine == lammpsTimeDump)
		{
			DumpTime     =   stoi(line);
		}


		checkLine    =   countLine - (thisTotalBead + lammpsHeadRow) * (thisTotalTimeDump-1);
		checkBead    =   checkLine - lammpsHeadRow;
		
		if (checkLine == lammpsTimeDump)
		{
			DumpTime     =   stoi(line);
			checkTime    =   fmod(DumpTime*timeStep*ThisLayer.rate*convDeformRate, ThisLayer.skipStrain)/ThisLayer.skipStrain;

			if (checkTime <= ThisLayer.skipCheckFactor or checkTime >= 1-ThisLayer.skipCheckFactor)
			// if (checkLine == lammpsTimeDump and (checkTime <= ThisLayer.skipCheckFactor or checkTime >= 1-ThisLayer.skipCheckFactor))
			{
				ThisLayer.StepAtDump.push_back(stoi(line));
				ThisDump.stepAtDump = stoi(line);
			}
		}
		

		// if ((iss>>i1>>i2>>f1>>f2>>f3>>f4>>f5>>f6 and Mid == "MicelleUnWrap") or  (iss>>i1>>i2>>f1>>f2>>f3 and Mid == "WaterIonUnWrap"))
		if (checkBead >= 1 and checkBead <= thisTotalBead and (checkTime <= ThisLayer.skipCheckFactor or checkTime >= 1-ThisLayer.skipCheckFactor))
		{
			istringstream iss(line);
		
			if (Mid == "MicelleUnWrap" or Mid == "")
			{
				if ((iss>>i1>>i2>>f1>>f2>>f3>>f4>>f5>>f6) and i2 != 1) 
				{
					ThisBead.indx    =   i1;
					ThisBead.type    =   i2;
					ThisBead.rx      =   f1;
					ThisBead.ry      =   f2;
					ThisBead.rz      =   f3;
				}
			}
			else
			{
				if ((iss>>i1>>i2>>f1>>f2>>f3) and i2 != 1) 
				{
					ThisBead.indx    =   i1;
					ThisBead.type    =   i2;
					ThisBead.rx      =   f1;
					ThisBead.ry      =   f2;
					ThisBead.rz      =   f3;
				}
			}

			ThisDump.atom[i1]   =   ThisBead;
			ThisBead            =   eachBead();
			

			if (checkBead == thisTotalBead)
			{	
				// cout<<checkTime <<" ";
				ThisGroup.dump.push_back(ThisDump);
				ThisBead   =   eachBead();
				ThisDump   =   eachOrderedDump();
			}
		}
	}
	ReadFile.close();

	return ThisGroup;
}
// // 
// // 
// // 
void CalcTsmd(eachLayer &ThisLayer,  eachOrderedGroup Vec1)
{
	float thisTmsd     =   0;
	int count          =   0;

	for (int tDiff = 0; tDiff < ThisLayer.StepAtDump.size();   tDiff++)
	{
		ThisLayer.AllTmsd         +=   (itos(ThisLayer.StepAtDump[tDiff]) + "   ");
		ThisLayer.BeadTypeForTmsd +=   (itos(ThisLayer.StepAtDump[tDiff]) + "   ");
		count                      =   0;
		thisTmsd                   =   0;

		std::unordered_map<int, eachBead>::iterator it = Vec1.dump[tDiff].atom.begin();
		for (;  it  != Vec1.dump[tDiff].atom.end();  it++)
		{
			int ind1     =   it->first;
			thisTmsd     =   0;
			count        =   0;

			for (int tInit = 0; tInit+tDiff < ThisLayer.StepAtDump.size();  tInit++)
			{
				float r1Sq = pow(Vec1.dump[tInit+tDiff].atom[ind1].rx, 2.0)+pow(Vec1.dump[tInit+tDiff].atom[ind1].ry, 2.0)+pow(Vec1.dump[tInit+tDiff].atom[ind1].rz, 2.0);
				float r0Sq = pow(Vec1.dump[tInit].atom[ind1].rx, 2.0)+pow(Vec1.dump[tInit].atom[ind1].ry, 2.0)+pow(Vec1.dump[tInit].atom[ind1].rz, 2.0);

				thisTmsd += (r1Sq + r0Sq - pow(4*r0Sq*r1Sq, 0.5) );
				count++;

				if (Vec1.dump[tInit+tDiff].atom[ind1].indx !=  Vec1.dump[tInit].atom[ind1].indx)
				{
					cout<<"Not same bead! " <<tDiff <<"  "   <<tInit <<"  ";
				}
			}
			// cout<<thisTmsd <<"  ";
			ThisLayer.AllTmsd         += (ftos(thisTmsd/count) + "   ");
			ThisLayer.BeadTypeForTmsd += (itos(Vec1.dump[tDiff].atom[ind1].type) + "   ");
			thisTmsd                  =  0;
			count                     =  0;
		}

		ThisLayer.AllTmsd         += ("\n");
		ThisLayer.BeadTypeForTmsd += ("\n");
	}

	FileWriteTsmd (ThisLayer);
}
// 
// 
// 
void FileWriteTsmd (eachLayer ThisLayer)
{
	WriteFileName = ThisLayer.writeFolderName1 + "Bead" + ThisLayer.writeFileNameEnd + "-" + ThisLayer.direction + ".dat";
	ofstream WriteFile1(WriteFileName.c_str());
	WriteFile1<<ThisLayer.BeadTypeForTmsd;
	WriteFile1.close();


	WriteFileName = ThisLayer.writeFolderName1 + "Tamsd" + ThisLayer.writeFileNameEnd + "-" + ThisLayer.direction + ".dat";
	ofstream WriteFile2(WriteFileName.c_str());
	WriteFile2<<ThisLayer.AllTmsd;
	WriteFile2.close();
}
// 
// 
// 
void FileWriteStrain (eachLayer ThisLayer)
{
	WriteFileName = ThisLayer.writeFolderName1 + "Strain" + ThisLayer.writeFileNameEnd + "-" + ThisLayer.direction + ".dat";
	ofstream WriteFile1(WriteFileName.c_str());
	WriteFile1<<ThisLayer.AllStrain;
	WriteFile1.close();
}