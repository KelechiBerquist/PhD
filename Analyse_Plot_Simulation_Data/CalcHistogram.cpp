#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>   
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
					eachGroup WaterIon;
					eachGroup Micelle;
					eachLayer ThisLayer;


					ThisLayer.chosen            =  chosen;
					ThisLayer.rate              =  rate;
					ThisLayer.skipStrain        =  0.5;
					ThisLayer.skipCheckFactor   =  0.02;
					ThisLayer.forRate           =  log10(rate/Rate[0]);
					ThisLayer.direction         =  direct;
					ThisLayer.readFolderName    =  rootDir + itos(chosen) + "/" + direct + "/"  + simType + "/";
					ThisLayer.writeFolderName1  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Hist/";
					ThisLayer.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-En1";
					if (simType != "Load")
					{
						ThisLayer.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-S3";
					}
					
					ThisLayer.writeFileNameEnd  =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen);
					
					Micelle  = GetTraj(ThisLayer, "MicelleUnWrap");
					WaterIon = GetTraj(ThisLayer, "WaterIonUnWrap");

					
					cout<<" For: Spn"<<chosen<<" "  <<direct<<" R"  <<rate<<" Micelle:"  <<Micelle.dump.size()<<" Water:"  <<WaterIon.dump.size()<<" \n";  

					GetHistogram(ThisLayer, Micelle,  WaterIon);
				}
			}
		}
	}
	return 0;
}
// ThisLayer.writeFolderName1  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Struct/";
// ThisLayer.writeFolderName2  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Energy/";
// ThisLayer.writeFolderName3  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Tasmd/";
// ThisLayer.writeFolderName4  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Hist/";