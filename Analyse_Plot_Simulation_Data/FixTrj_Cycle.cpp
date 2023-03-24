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



#define  Encore         3
// string rootDir = "/mnt/e/Struct/";
// string rootDir = "/media/KaceHDD1/DifferentStructures/";
string rootDir     =   "/media/KaceHDD1/Struct/";
string RunType     =   "Cycle";

int main ()
{
	vector <int> Chosen    =  {1};
	// vector <int> Chosen    =  {1, 2, 3, 4};
	vector <double> Rate   =  {1, 3, 10, 30};
	vector <string> Direct =  {"X"};
	vector <string> SimType =  {"Load", "UnLoad"};



	for (auto chosen : Chosen)
	{
		for (auto direct : Direct)
		{
			for (auto rate : Rate)
			{
				eachCluster ThisCluster;
				string  AllTrjData = "";
				
				for (int encore = 0;  encore < Encore;  encore++)
				{
					for (auto simType : SimType)
					{
						ThisCluster.chosen            =  chosen;
						ThisCluster.rate              =  rate;
						ThisCluster.forRate           =  1.5*log10(rate/Rate[0]);
						ThisCluster.direction         =  direct;
						ThisCluster.readFolderName    =  rootDir + itos(chosen) + "/" + direct + "/"  + RunType + "/";
						ThisCluster.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-En" + itos(encore+1);
						
						
						cout <<ThisCluster.readFolderName<<"   "<<ThisCluster.writeFolderName1<<"\n";

						GetData(ThisCluster);
						FindFixWriteMicelle(ThisCluster);

						AllTrjData += ThisCluster.writeTrj;
						ThisCluster = eachCluster();
					} //ForEncore
				}//For SimType

				ThisCluster.writeFileNameEnd   =    RunType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen);
				ThisCluster.writeFolderName1   =    "/media/KaceHDD1/Struct/ImageVideos/" + RunType + "_Struct/";
				ThisCluster.writeTrj           =    AllTrjData;
				FileWriteTraj (ThisCluster);
			}//ForRate
		}//For Direct
	}//For Chosen
	return 0;
}

// ThisLayer.writeFolderName1    =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Struct/";
// ThisLayer.writeFolderName2    =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Energy/";
// ThisLayer.writeFolderName3    =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Tasmd/";
// ThisLayer.writeFolderName4    =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Hist/";
// ThisLayer.writeFolderName4    =  "/media/KaceHDD1/Struct/ImageVideos/" + RunType + "_Struct/";