#ifndef WRITINGLAMMPSFILES_H
#define WRITINGLAMMPSFILES_H


//struct waterInsystem
//{
	
//};



struct atom
{
	int id       = 0;
	int type     = 0;
	int chainId  = 0;
	int charge   = 0;
	
	float x = 0;
	float y = 0;
	float z = 0;
};

struct angle
{
	int id    = 0;
	int type  = 0;
	int i     = 0;   
	int j     = 0;
	int k     = 0;
};

struct bond
{
	int id     = 0;
	int type   = 0;
	int i      = 0;   
	int j      = 0;
};

struct singleTrj
{
	std::vector<atom> BeadInfo = {};
	
	
	
	float xBoxLength = 0;
	float yBoxLength = 0;
	float zBoxLength = 0;
	
	float xlo = 0;
	float ylo = 0;
	float zlo = 0;
	
	float xhi = 0;
	float yhi = 0;
	float zhi = 0;
	
	
	std::string timeDump        = "";
	std::string topInConfig     = "";
	std::string beadsInConfig   = "";
	std::string bondsInConfig   = "";
	std::string anglesInConfig  = "";
};

struct mysystem
{
	int mainBead         =  0;
	int mainChain        =  0;
	int soluteBead       =  0;
	int soluteChain      =  0; 
	int counterMain      =  0;   
	int counterSolute    =  0;            
	int mainChaB         =  0;         
	int soluteChaB       =  0;
	int chaB             =  0;            
	int chain            =  0;        
	int WA               =  0;  
	int AFW              =  0;  
	int size             =  0;    
	int WAf              =  0;
	int restartFreq      =  0;
	int trjDumpLong      =  0;
	int trjDumpShort     =  0;
	int logDump          =  0;
	int runStep          =  0;
	int solute_a_Num     =  0;
	int solute_b_Num     =  0;
	int main_b_Num       =  0;
	int main_a_Num       =  0;
	int a_Num            =  0;
	int b_Num            =  0;
	int massMartiniBead  =  72;
	int cumm0            =  0;
	int cumm1            =  0;
	int cumm2            =  0;
	int cumm3            =  0;
	int cumm4            =  0;
	int cumm5            =  0;
	int totalBeadType    =  0;
	int totalBondType    =  0;
	int totalAngleType   =  0; 
	
	
	double rate              = 0;
	double elongRate         = 0;
	double multiplierForVisc = 0;
	
	//float xBoxLength = 0;
	//float yBoxLength = 0;
	//float zBoxLength = 0;
	
	//float xlo = 0;
	//float ylo = 0;
	//float zlo = 0;
	
	//float xhi = 0;
	//float yhi = 0;
	//float zhi = 0;
	
	
	std::string direction          = "";
	std::string fileNameEnd        = "";
	std::string readFileNameEnd    = "";
	std::string mainTop            = "";
	std::string soluteTop          = "";
	std::string writeFolderName    = "";
	std::string readFolderName     = "";
	std::string runType            = "";
	//std::string topInConfigData  = "";
	//std::string beadsInConfigData  = "";
	//std::string bondsInConfigData  = "";
	//std::string anglesInConfigData  = "";
	
	//std::vector<atom> SingleGroTrj = {};
	
	std::vector<singleTrj> MultiTrj;
};




//********************Function Prototypes********************
std::string LogToPairModify (mysystem &CurrentSystem);

std::string VariableLxToScale (int deltaT);

std::string TimeStepToThermoModify (int deltaT);

std::string TensionRelax (mysystem &CurrentSystem, int deltaT);

void AssignConfig (mysystem &CurrentSystem);

void GetCoordConfig (mysystem &CurrentSystem);

void WriteBeadInConfig (mysystem &CurrentSystem, int timeDeump);

void WriteBondInConfig (mysystem &CurrentSystem, int timeDump);

void WriteAngleInConfig (mysystem &CurrentSystem, int timeDump);

void TopConfig (mysystem &CurrentSystem, int timeDump);

#endif

//****************************1*************************************//
//else if (CurrentSystem.runType == "Relax")
//{
	/////media/OkwuchisHDD/DifferentStructures/4/Load_x/MicelleWrapLoad-Rate10-Spn4-En1.lammpstrj
	//string ReadFileName1  =  CurrentSystem.readFolderName +  "MicelleWrap" + CurrentSystem.readFileNameEnd + ".lammpstrj";
	//ifstream ReadFile1 (ReadFileName1.c_str());
	
	//string ReadFileName2  =  CurrentSystem.readFolderName +  "WaterIonWrap" + CurrentSystem.readFileNameEnd + ".lammpstrj";
	//ifstream ReadFile2 (ReadFileName2.c_str());
	
	
	
	
	////vector <string> File1, File2;
	
	////singleTrj UnfixTrj,  FixTrj;
	
	
	//string line1,  line2,  boxLengthInfo = "";
	
	
	//int count = 0,        countTime = 0;
	//int skipDump = 25,    linePerDump = CurrentSystem.size+9;
	//int writeTime = 0;
	
	
	//int HeaderLines = 9,        countLineInFile = 0;
	//int countLineFile1 = 0,     countLineFile2 = 0;
	//int skipDumpFile1 = 25,     skipDumpFile2 = 1;
	//int index1 = 0,             index2 = 0;
	//int beadsFile1 = CurrentSystem.chaB;
	//int beadsFile2 = CurrentSystem.WAf + CurrentSystem.mainChain + CurrentSystem.soluteChain;
	
	//int linesFile1 = beadsFile1 + HeaderLines;
	//int linesFile2 = beadsFile2 + HeaderLines;
	
	
	////CurrentSystem.MultiTrj.resize(1);
	////cout<<"CurrentSystem.MultiTrj.size(): " <<CurrentSystem.MultiTrj.size()<<"\n";
	
	////CurrentSystem.MultiTrj[0].BeadInfo.resize(CurrentSystem.size);
	////cout<<"CurrentSystem.MultiTrj[0].BeadInfo: " <<CurrentSystem.MultiTrj[0].BeadInfo.size()<<"\n";
	
	
	//while (std::getline(ReadFile1, line1) or std::getline(ReadFile2, line2) )
	//{
		//float  x1, x2, x3, x4, x5, x6;
		//int    i1, i2;
		//string s1, s2;
		
		//countLineInFile++;
		
		//std::istringstream iss1(line1);
		//std::istringstream iss2(line2);
		//std::istringstream iss3(boxLengthInfo);
		
		
		//if ( (countLineInFile/linesFile1)%skipDumpFile1 == 0)
		//{
			//index1 = floor((countLineFile1/beadsFile1)/skipDumpFile1);
			
			////if (countLineFile1%linePerDump >= 5 and countLineFile1%linePerDump <= 7)
			//if (iss1>>x1>>x2)
			//{
				//boxLengthInfo += line1;
				
				//if (iss3>>x1>>x2>>x3>>x4>>x5>>x6)
				//{
					//CurrentSystem.MultiTrj[index1].xlo = x1;
					//CurrentSystem.MultiTrj[index1].xhi = x2;
					//CurrentSystem.MultiTrj[index1].xBoxLength = CurrentSystem.MultiTrj[index1].xhi - CurrentSystem.MultiTrj[index1].xlo;
					
					//CurrentSystem.MultiTrj[index1].ylo = x3;
					//CurrentSystem.MultiTrj[index1].yhi = x4;
					//CurrentSystem.MultiTrj[index1].yBoxLength = CurrentSystem.MultiTrj[index1].yhi - CurrentSystem.MultiTrj[index1].ylo;
					
					//CurrentSystem.MultiTrj[index1].zlo = x5;
					//CurrentSystem.MultiTrj[index1].zhi = x6;
					//CurrentSystem.MultiTrj[index1].zBoxLength = CurrentSystem.MultiTrj[index1].zhi - CurrentSystem.MultiTrj[index1].zlo;
					
					//boxLengthInfo = "";
				//}
			//}	
			
			//if (iss1>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6)
			//{
				//CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].x = x1;
				//CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].y = x2;
				//CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].z = x3;
				
				//countLineFile1++;
			//}
		//}
	////}
	
	
	////while (std::getline(ReadFile1, line1) or std::getline(ReadFile2, line2) )
	////{
		////float  x1, x2, x3, x4, x5, x6;
		////int    i1, i2;
		////string s1, s2;
		
		////countLineInFile++;
		
		////std::istringstream iss1(line1);
		////std::istringstream iss2(line2);
		////std::istringstream iss3(boxLengthInfo);
		
		
		
		//if ( (countLineFile2/beadsFile2)%skipDumpFile2 == 0)
		//{
			//index2 = floor((countLineFile2/beadsFile2)/skipDumpFile2);
			
			//if (iss2>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6)
			//{
				//CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].x = x1;
				//CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].y = x2;
				//CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].z = x3;
				
				//count++;
				
				//countLineFile2++;
				
				////cout<<x1<<";  ";
			//}
		//}
		
		//if ( (countLineFile1%beadsFile1) == beadsFile1-1 or (countLineFile2%beadsFile2) == beadsFile2-1)
		//{
			//CurrentSystem.MultiTrj.resize(index2+1);
			//CurrentSystem.MultiTrj[index2+1].BeadInfo.resize(CurrentSystem.size);
			
			//cout<<"Index1: " <<index1<<"   Index2: " <<index2 <<"\n";
		//}
	//}
	//ReadFile1.close();
	
	//cout<<"\n";
//}
//****************************1*************************************//






//****************************2*********************************//
////else if (CurrentSystem.runType == "Relax")
	////{
		///////media/OkwuchisHDD/DifferentStructures/4/Load_x/MicelleWrapLoad-Rate10-Spn4-En1.lammpstrj
		////string ReadFileName1  =  CurrentSystem.readFolderName +  "MicelleWrap" + CurrentSystem.readFileNameEnd + ".lammpstrj";
		////ifstream ReadFile1 (ReadFileName1.c_str());
		
		////string ReadFileName2  =  CurrentSystem.readFolderName +  "WaterIonWrap" + CurrentSystem.readFileNameEnd + ".lammpstrj";
		////ifstream ReadFile2 (ReadFileName2.c_str());
		
		
		
		
		//////vector <string> File1, File2;
		
		//////singleTrj UnfixTrj,  FixTrj;
		
		
		////string line1,  line2,  boxLengthInfo = "";
		
		
		//////int count = 0,        countTime = 0;
		//////int skipDump = 25,    linePerDump = CurrentSystem.size+9;
		//////int writeTime = 0;
		
		
		////int HeaderLines = 9,        countLineInFile = 0;
		////int skipDumpFile1 = 25,     skipDumpFile2 = 1;
		////int index1 = 0,             index2 = 0;
		
		////int beadsFile1 = CurrentSystem.chaB;
		////int beadsFile2 = CurrentSystem.WAf + CurrentSystem.mainChain + CurrentSystem.soluteChain;
		
		////int linesFile1 = beadsFile1 + HeaderLines;
		////int linesFile2 = beadsFile2 + HeaderLines;
		
		////int countLineBead1 = 0,   countLineBead2 = 0;
		////int countLineFile1 = 0,   countLineFile2 = 0;
		
		////CurrentSystem.MultiTrj.resize(1);
		////CurrentSystem.MultiTrj[0].BeadInfo.resize(CurrentSystem.size);
		
		
		//////cout<<"CurrentSystem.MultiTrj.size(): " <<CurrentSystem.MultiTrj.size()<<"\n";
		//////cout<<"CurrentSystem.MultiTrj[0].BeadInfo: " <<CurrentSystem.MultiTrj[0].BeadInfo.size()<<"\n";
		
		
		////cout<<"\n  Index1: ";
		////while (std::getline(ReadFile1, line1) )
		////{
			////float  x1, x2, x3, x4, x5, x6;
			////int    i1, i2;
			////string s1, s2;
			
			////countLineInFile++;
			
			////countLineFile1++;
			
			////std::istringstream iss1(line1);
			////std::istringstream iss3(boxLengthInfo);
			
			////index1 = floor((countLineFile1/linesFile1)/skipDumpFile1);
			
			////if ( (countLineInFile/linesFile1)%skipDumpFile1 == 0)
			////{
				////if (iss1>>x1>>x2)
				////{
					////boxLengthInfo += line1;
				////}
				
				////if (iss3>>x1>>x2>>x3>>x4>>x5>>x6)
				////{
					////CurrentSystem.MultiTrj[index1].xlo = x1;
					////CurrentSystem.MultiTrj[index1].xhi = x2;
					////CurrentSystem.MultiTrj[index1].xBoxLength = CurrentSystem.MultiTrj[index1].xhi - CurrentSystem.MultiTrj[index1].xlo;
					
					////CurrentSystem.MultiTrj[index1].ylo = x3;
					////CurrentSystem.MultiTrj[index1].yhi = x4;
					////CurrentSystem.MultiTrj[index1].yBoxLength = CurrentSystem.MultiTrj[index1].yhi - CurrentSystem.MultiTrj[index1].ylo;
					
					////CurrentSystem.MultiTrj[index1].zlo = x5;
					////CurrentSystem.MultiTrj[index1].zhi = x6;
					////CurrentSystem.MultiTrj[index1].zBoxLength = CurrentSystem.MultiTrj[index1].zhi - CurrentSystem.MultiTrj[index1].zlo;
					
					////boxLengthInfo = "";
				////}
				
				
				////if (iss1>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6)
				////{
					////CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].x = x1;
					////CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].y = x2;
					////CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].z = x3;
				////}
				
				////if ( (countLineFile1%linesFile1) == 0)
				////{
					////CurrentSystem.MultiTrj.resize(index1+1);
					////CurrentSystem.MultiTrj[index1].BeadInfo.resize(CurrentSystem.size);
					
					////cout<<index1<<";  ";
				////}
			////}
		////}
		////ReadFile1.close();
		
		
		////cout <<"\n  Index2: ";
		////while ( std::getline(ReadFile2, line2) )
		////{
			////float  x1, x2, x3, x4, x5, x6;
			////int    i1, i2;
			////string s1, s2;
			
			////countLineFile2++;
			
			////std::istringstream iss2(line2);
			
			////index2 = floor((countLineFile2/linesFile2)/skipDumpFile2);
			
			////if ( (countLineFile2/linesFile2)%skipDumpFile2 == 0 and (iss2>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) )
			////{
				////CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].x = x1;
				////CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].y = x2;
				////CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].z = x3;
				
				
				////////if ( (countLineFile2%linesFile2) == 0)
				////////{
					//////////CurrentSystem.MultiTrj.resize(index2+1);
					//////////CurrentSystem.MultiTrj[index2].BeadInfo.resize(CurrentSystem.size);
					
					
					////////cout<<index2<<";  ";
				////////}
				
			////}
			
			
		////}
		////ReadFile2.close();
		
		////cout<<"\n";
	////}
//****************************2*********************************//





//if ( (iss>>x1>>x2) and countLineInFile >= countTime*CurrentSystem.size+6 and countLineInFile == countTime*CurrentSystem.size+8)
			//{
				//UnfixTrj.xlo = x1;
				//UnfixTrj.xh1 = x2;
				//UnfixTrj.xBoxLength = x2-x1;
			//}
			
			//if ( (iss>>x1>>x2) and countLineInFile == countTime*CurrentSystem.size+7)
			//{
				//UnfixTrj.ylo = x1;
				//UnfixTrj.yh1 = x2;
				//UnfixTrj.yBoxLength = x2-x1;
			//}
			
			//if ( (iss>>x1>>x2) and countLineInFile == countTime*CurrentSystem.size+7)
			//{
				//CurrentSystem.zlo = x1;
				//CurrentSystem.zh1 = x2;
				//CurrentSystem.zBoxLength = x2-x1;
			//}
			

