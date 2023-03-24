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




string rootDir = "/media/KaceHDD1/Struct/";



int main ()
{
	// vector <int> Chosen    =  {2};
	// vector <double> Rate   =  {10};
	// vector <string> Direct =  {"X"};
	// vector <string> SimType =  {"Load"};
	
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
					eachGroup Micelle;
					eachLayer ThisLayer;

					ThisLayer.chosen            =  chosen;
					ThisLayer.rate              =  rate;
					ThisLayer.skipStrain        =  0.02;
					ThisLayer.skipCheckFactor   =  1;
					ThisLayer.forRate           =  log10(rate/Rate[0]);
					ThisLayer.direction         =  direct;
					ThisLayer.readFolderName    =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Struct/";
					ThisLayer.writeFolderName1  =  "/media/KaceHDD1/Struct/ImageVideos/" + simType + "_Struct/";
					ThisLayer.readFileNameEnd   =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen) + "-" + ThisLayer.direction;
					ThisLayer.writeFileNameEnd  =  simType + "-Rate" + dtos(rate) + "-Spn" + itos(chosen);
					
					Micelle  = GetTraj(ThisLayer, "");

					// cout <<ThisLayer.readFolderName<<"   "<<ThisLayer.writeFolderName1<<"\n";
					cout<<" For: Spn"<<chosen<<" "  <<direct<<" R"  <<rate<<" Micelle:"  <<Micelle.dump.size()<<" "  <<" \n";  

					GetStrain(ThisLayer, Micelle);
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