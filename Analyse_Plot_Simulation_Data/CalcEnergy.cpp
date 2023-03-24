#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>   


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




// string rootDir = "/mnt/e/Struct/";
string rootDir = "/media/KaceHDD1/Struct/";



int main ()
{
	vector <int> Chosen    =  {1, 2, 3, 4};
	vector <double> Rate   =  {0.1, 0.3, 1, 3.0, 10, 30};
	vector <string> Direct =  {"X", "Y"};
	vector <string> SimType =  {"Load"};


	for (auto chosen : Chosen)
	{
		for (auto direct : Direct)
		{
			for (auto simType : SimType)
			{
				for (auto rate : Rate)
				{
					eachLayer ThisLayer;

					ThisLayer.chosen            =  chosen;
					ThisLayer.rate              =  rate;
					ThisLayer.forRate           =  log10(rate/Rate[0]);
					ThisLayer.direction         =  direct;
					ThisLayer.readFolderName    =  rootDir + itos(chosen) + "/" + direct + "/"  + simType + "/";
					ThisLayer.writeFolderName1  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Energy/";
					ThisLayer.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-En1";
					if (simType != "Load")
					{
						ThisLayer.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-S3";
					}
					
					ThisLayer.writeFileNameEnd  =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen);
					
					cout <<ThisLayer.readFolderName<<"   "<<ThisLayer.writeFolderName1<<"\n";

					GetEnergy(ThisLayer);
				}
			}
		}
	}
	return 0;
}