#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h> 
#include <unordered_map>    


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



// string rootDir     =   "/media/KaceHDD1/Struct/";
string rootDir     =   "/media/KaceHDD1/DifferentStructures/";

int main ()
{
	// vector <int> Chosen    =  {4};
	vector <int> Chosen      =  {1, 2, 3, 4};
	vector <double> Rate     =  {};
	vector <string> Direct   =  {"X", "Y"};
	vector <string> SimType  =  {"Load"};

	// vector <string> Direct =  {"X"};
	// vector <string> SimType =  {"UnLoad"};

	// vector <string> Direct =  {"X"};
	// vector <string> SimType =  {"Load", "UnLoad"};


	for (auto chosen : Chosen)
	{
		for (auto direct : Direct)
		{
			for (auto simType : SimType)
			{
				if (simType == "Load"){Rate   =  {0.1, 0.3, 1, 3, 10, 30};}
				else                  {Rate   =  {0.3, 3, 30};}
				

				for (auto rate : Rate)
				{
					// cout<<"\n";
					eachLayer ThisLayer;

					ThisLayer.chosen            =  chosen;
					ThisLayer.rate              =  rate;
					ThisLayer.forRate           =  log10(rate/Rate[0]);
					ThisLayer.direction         =  direct;
					ThisLayer.readFolderName    =  rootDir + itos(chosen) + "/" + direct + "/"  + simType + "/";
					// ThisLayer.writeFolderName1  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Struct/";
					ThisLayer.writeFolderName1  =  "/media/KaceHDD1/Struct/ImageVideos/Previous/" + simType + "_DiffStruct_Struct/";
					
					ThisLayer.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-En1";
					if (simType != "Load")
					{
						ThisLayer.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-S3";
					}
					
					ThisLayer.writeFileNameEnd  =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen);
					
					cout <<ThisLayer.readFolderName<<"   "<<ThisLayer.writeFolderName1<<"\n";


					eachCluster ThisClusterW = GetData(ThisLayer, "MicelleWrap");
					FindEachMicelle(ThisLayer, ThisClusterW);
					

					// eachCluster ThisCluster1 = GetData(ThisLayer, "MicelleWrap"),
					//             ThisCluster2 = GetData(ThisLayer, "MicelleUnWrap");
					// FindFixWriteMicelle(ThisLayer, ThisCluster1, ThisCluster2);
					// FileWriteEnergy (ThisLayer);
				}
			}
		}
	}
	return 0;
}