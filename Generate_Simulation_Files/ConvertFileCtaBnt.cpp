#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <sstream>
#include<cmath>
#include <iomanip>
#include <algorithm>
using std::vector;
using namespace std;
using std::string;
using std::cout;
using std:: swap;



string itos(int i)
{
	stringstream s;
	s << i;
	return s.str();
}


string ftos(float i)
{
	stringstream s;
	s << i;
	return s.str();
}


#define BntBead          4  
#define ClBead           1  
#define BtnBead          2  
#define CcnBead          3  
#define LcnBead          4  
#define PcnBead          5  
#define McnBead          7  
#define HcnBead          10  
#define OtaBead          3  
#define CtaBead          5  
#define BtaBead          7  

string Line;


int main()
{
	string SoluteType,       ReadWriteFolderName,    SharedGromacsFiles;
	string ReadFolderName1,  ReadFolderName2,    ReadFolderName,   WriteFolderName,        ReadWriteFolderName2;
	
	string ReadFileName1,   ReadFileName2,   ReadFileName3,   ReadFileName4,   ReadFileName5,   ReadFileName6,   ReadFileName7,   ReadFileName8,   ReadFileName9,   ReadFileName10;
	string ReadFileName11,  ReadFileName12,  ReadFileName13,  ReadFileName14,  ReadFileName15,  ReadFileName16,  ReadFileName17,  ReadFileName18,  ReadFileName19,  ReadFileName20;
	
	string WriteFileName1,   WriteFileName2,   WriteFileName3,   WriteFileName4,   WriteFileName5,   WriteFileName6,   WriteFileName7,   WriteFileName8,   WriteFileName9,   WriteFileName10;
	string WriteFileName11,  WriteFileName12,  WriteFileName13,  WriteFileName14,  WriteFileName15,  WriteFileName16,  WriteFileName17,  WriteFileName18,  WriteFileName19,  WriteFileName20;
	
	
	string  LocatePC = "/media/KaceHDD1/RatioFatty/";
	//string  LocatePC = "/media/OkwuchisHDD/";
	
	
	int RunCores;
	
	if (LocatePC == "/media/KaceHDD1/RatioFatty/")
	{
		//RunCores = 6;
		RunCores = 3;
		SharedGromacsFiles = "/home/kelechi/SharedGromacsFiles/";
	}
	if (LocatePC == "/media/OkwuchisHDD/RatioFatty/")
	{
		RunCores = 4;
		SharedGromacsFiles = "/home/okwuchi/SharedGromacsFiles/";
	}
	
	
	string FolderName =  "/media/KaceHDD1/DifferentStructures/SortStructures/";
	
	vector <string>   FilesForSorting = {"MCN-Ota-7_M4",
					"PCN-Mcn-1_M2",
					"PCN-Mcn-7_M2",
					"CTA-NaBnt-0.3_M4",
					};
	
	
	for (int chosen = 0;  chosen < FilesForSorting.size();  chosen++)
	{
		string Micelle = FilesForSorting[chosen];
		
		ReadFolderName   =  FolderName;
		WriteFolderName  =  FolderName;
		
		
		clock_t begin1 = clock();
		
		
		
		//vector <float> Xu(Size),  Yu(Size),  Zu(Size);
		//vector <int> BeadID(Size);
		
		vector <float> Xu,   Yu,   Zu;
		vector <float> CXu,  CYu,  CZu;
		vector <float> BXu,  BYu,  BZu;
		
		vector <int>   BeadID,  InitialID;
		
		
		
		
		ReadFileName1 = ReadFolderName + "Sorted-" + Micelle + "-T0.xyz";
		ifstream ReadInput1(ReadFileName1.c_str());
		
		int countAllIndex = 0,  countCta  =  0,    countBnt  =  0;
		
		if(!ReadInput1){ }
		else
		{
			while (std::getline(ReadInput1, Line))
			{
				std::istringstream iss(Line);
				
				int    i1,  i2,  i3,  i4;
				string s1,  s2,  s3,  s4;
				float  f1,  f2,  f3,  f4;
				
				if ((iss>>s2>>f1>>f2>>f3) and s2[0] == 'B' and s2[1] == 'e')
				{
					Xu.push_back(f1);
					Yu.push_back(f2);
					Zu.push_back(f3);
					//InitialID.push_back(i1);
				}
			}
		}
		ReadInput1.close();
		
		
		
		//ReadFileName1 = ReadFolderName2 + "CounterMainNear1-" + MainPhase + "-"+ SoluteType +"-"+ rValue +"_M" + itos(Micelle[chosen]) +".xyz";
		//ifstream ReadInput2(ReadFileName1.c_str());
		
		//if(!ReadInput2){ }
		//else
		//{
			//while (std::getline(ReadInput2, Line))
			//{
				//std::istringstream iss(Line);
				
				//int    i1,  i2,  i3,  i4;
				//string s1,  s2,  s3,  s4;
				//float  f1,  f2,  f3,  f4;
				
				//if (iss>>s2>>f1>>f2>>f3)
				//{
					//BXu.push_back(f1);
					//BYu.push_back(f2);
					//BZu.push_back(f3);
				//}
			//}
		//}
		//ReadInput2.close();
		
		
		//ReadFileName1 = ReadFolderName2 + "CounterSoluteNear1-" + MainPhase + "-"+ SoluteType +"-"+ rValue +"_M" + itos(Micelle[chosen]) +".xyz";
		//ifstream ReadInput3(ReadFileName1.c_str());
		
		//if(!ReadInput3){ }
		//else
		//{
			//while (std::getline(ReadInput3, Line))
			//{
				//std::istringstream iss(Line);
				
				//int    i1,  i2,  i3,  i4;
				//string s1,  s2,  s3,  s4;
				//float  f1,  f2,  f3,  f4;
				
				//if (iss>>s2>>f1>>f2>>f3)
				//{
					//CXu.push_back(f1);
					//CYu.push_back(f2);
					//CZu.push_back(f3);
				//}
			//}
		//}
		//ReadInput3.close();
		
		
		
		// ************************* BEGIN WRITING TO SINGLE XYZ ************************* \\
		
		//string WriteFileName1a =  WriteFolderName + "MicelleIons-" + MainPhase + "-" + SoluteType +"-"+ rValue +"_M" + itos(Micelle[chosen]) +".xyz";
		//ofstream WriteFile1a(WriteFileName1a .c_str());
		
		//WriteFile1a<<Xu.size()<<"\n";
		//WriteFile1a<<"Random comment. I do not know if it is required. \n";
		
		//for(int a = 0;  a < Xu.size(); a++)
		//{
			//WriteFile1a<<setw(8)<<"Bead"
			//<<setw(10)<<fixed<<setprecision(3)<<Xu[a]<<""
			//<<setw(10)<<fixed<<setprecision(3)<<Yu[a]<<""
			//<<setw(10)<<fixed<<setprecision(3)<<Zu[a]<<"\n";
		//}
		
		//WriteFile1a.close();
		
		// ************************* END WRITING TO SINGLE XYZ ************************* \\
		
		
		
		
		
		// *************************BEGIN WRITING TO SINGLE GRO************************* \\
		
		int countID = 0;
		for (int a = 0;  a < ceil(Xu.size()/1e5) + 1;  a++)
		//for (int a = 0;  a < ceil(Size/1e5) + 1;  a++)
		{
			int changeBead = 0;
			for (int ab = 0;  ab < 1e5 - 1;  ab++)
			{
				if (countID < Xu.size()+BXu.size()+CXu.size())
				//if (countID < Size)
				{
					//BeadID[countID++]  =  ++changeBead;
					
					BeadID.push_back(++changeBead);
				}
			}
		}
		
		
		WriteFileName2 =  WriteFolderName + "Sorted-" + Micelle +".gro";
		ofstream WriteFile2(WriteFileName2 .c_str());
		
		WriteFile2<<WriteFileName2<<"  \n";
		WriteFile2<<Xu.size()<<"\n";
		
		
		//WriteFileName3 =  WriteFolderName + "MicelleIons-" + MainPhase + "-" + SoluteType +"-"+ rValue +"_M" + itos(Micelle[chosen]) +".gro";
		//ofstream WriteFile3(WriteFileName3.c_str());
		
		//WriteFile3<<WriteFileName3<<"  \n";
		//WriteFile3<<Xu.size()+BXu.size()+CXu.size()<<"\n";
		
		int countBead = 0;
		for (int a = 0;  a < ceil((Xu.size()+BXu.size()+CXu.size())/10000) + 1;  a++)
		//for (int a = 0;  a < ceil(WA/10000) + 1;  a++)
		{
			for(int b = 0;  b < 9999;  b++)
			{
				if (countBead < Xu.size() )
				//if (countBead < WA)
				{
					WriteFile2<<" "<<setw(4)<<setfill(' ')<<b+1<<""  <<setw(5)<<setfill(' ')<<"W    "<<""
					<<setw(5)<<setfill(' ')<<"W"<<""           <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
					<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
					<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
					<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
					
					//WriteFile3<<" "<<setw(4)<<setfill(' ')<<b+1<<""  <<setw(5)<<setfill(' ')<<"W    "<<""
					//<<setw(5)<<setfill(' ')<<"W"<<""           <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
					//<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
					//<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
					//<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
				}
				if (countBead >= Xu.size() and countBead < Xu.size()+BXu.size()+CXu.size())
				//if (countBead < WA)
				{
					//WriteFile3<<" "<<setw(4)<<setfill(' ')<<b+1<<""  <<setw(5)<<setfill(' ')<<"W    "<<""
					//<<setw(5)<<setfill(' ')<<"W"<<""           <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
					//<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
					//<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
					//<<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
				}
				countBead++;
			}
		}
		
		
		WriteFile2<<setw(10)<<fixed<<setprecision(5)<<setfill(' ')<<30.0<<""
		<<setw(10)<<fixed<<setprecision(5)<<setfill(' ')<<30.0<<""
		<<setw(10)<<fixed<<setprecision(5)<<setfill(' ')<<30.0<<"\n";
		
		
		
		// *************************END WRITING TO GRO************************* \\
		
		BeadID.clear(),  Xu.clear(),  Yu.clear(),  Zu.clear();
		WriteFile2.close();
	}
	
	
	return 0;
}

// *************************BEGIN HELPFUL HINTS************************* \\

/*
 // std::setprecision(3); on its own will make sure there are only 3 significant figures in my floating number
 //std::fixed  When used with setpprecision, tells the program that you want the precision that was set to be fixed number format (ie decimal points)
 
 //witea<<setw(6)<<"ATOM  "   <<setw(5)<<a+1   <<setw(1)<<""   <<setw(1)<<"Z" <<setfill(    "0")<<setw(3)<<a+1    <<setfill(' ')<<setw(1)<<""   <<setw(3)<<"PEO"   <<setw(1)<<""   <<setw(1)<<""         <<setw(4)<<"  1"   <<setw(1)<<""   <<setw(3)<<""   <<fixed<<setw(8)<<setprecision(3)<<BNx[a]   <<    ""   <<fixed<<setw(8)<<setprecision(3)<<BNy[a]    <<""   <<fixed<<setw(8)<<setprecision(3)<<BNz[a]       <<fixed<<setprecision(2)<<setw(6)<<" 1.0"   <<fixed<<setprecision(2)<<setw(6)<<" 0.0"<<"\n";}
 
 //for(int a = 0;  a < BeadNum;  a++)
 {
 cout <<"Coords"<<""<<fixed<<setw(8)<<setprecision(3)<<BNx[a]<<""
 <<fixed<<setw(8)<<setprecision(3)<<BNy[a]<<""
 <<fixed<<setw(8)<<setprecision(3)<<BNz[a]<<"\n";
 }
 
 
 
 
 //In Brookhaven PDB format, the ATOM should begin at the very first space. There should not be spaces before the word ATOM.
 //The apostrophes are used create space in between the entries in the array.
 //witea is a string name that C++ recongnises and because of the ofstream command and then it writes the data into the file with the file and << command
 //<<set(w) is the way to set the width of that entry.
 //<<setprecision(2) this sets the number of significant figures in the array to 2
 //<<fixed<<setprecision(2) sets the number of decimal places to 2.
 //<<setfil("0") sets the fill in all subsequent columns to zero. If this is not wanted use <<setfill(' ') to change the fill back to space rather than 0. Anything can be used as a fill.
 
 
 
 
 
 for(int a = 0;  a < WA;  a++)
 {
 WAx[a] = 0.00 + a*2.80;
 WAy[a] = 0.00 + a*0.00;
 WAz[a] = 6.000 +a*0.00;
 }
 
 ofstream witeb("Water.pdb");
 for(int a = 0;  a < WA;  a++)
 {
 witeb<<setw(6)<<"ATOM  "<<setw(5)<<a+1 <<setw(1)<<""<<setw(4)<<"W "<<setw(1)<<""<<setw(3)<<"W"
 <<setw(1)<<" "<<setw(1)<<" "<<setw(4)<<1 <<setw(1)<<" "<<setw(3)<<"   "<<fixed<<setw(8)<<setprecision(3)<<WAx[a]<<""<<fixed<<setw(8)<<setprecision(3)<<WAy[a]<<""
 <<fixed<<setw(8)<<setprecision(3)<<WAz[a]<<fixed<<setprecision(2)<<setw(6)<<1.0<<fixed<<setprecision(2)<<setw(6)<<0.0<<"\n";
 }
 //Water molecule name must be called W because that is what is written in the martini itp file.
 
 
 
 for(int a = 0;  a < AFWA;  a++)
 {
 AFWAx[a] = 1.00 + a*2.80;
 AFWAy[a] = 6.00 + a*0.280;
 AFWAz[a] = 5.000 +a*0.280;
 }
 
 ofstream witec("AntifreezeWater.pdb");
 for(int a = 0;  a < AFWA;  a++)
 {
 witec<<setw(6)<<"ATOM  "<<setw(5)<<a+1 <<setw(1)<<""<<setw(4)<<"WF"
 <<setw(1)<<""<<setw(3)<<"WF"<<setw(1)<<" "<<setw(1)<<" "<<setw(4)<<1
 <<setw(1)<<" "<<setw(3)<<" "<<fixed<<setw(8)<<setprecision(3)<<AFWAx[a]<<""
 <<fixed<<setw(8)<<setprecision(3)<<AFWAy[a]<<""<<fixed<<setw(8)<<setprecision(3)<<AFWAz[a]
 <<fixed<<setprecision(2)<<setw(6)<<1.0<<fixed<<setprecision(2)<<setw(6)<<0.0<<"\n";
 }
 //Antifreeze Water molecule name must be called WF because that is what is written in the martini itp file.
 
 */

// *************************END HELPFUL HINTS************************* \\









/*
 // ************** For Cta
 for(int a = 0;  a < ceil(CtaChain/10000) + 1; a++)
 {
 for(int b = 0;  b < 9999; b++)
 {
 if (countBead >= WAf && countBead < WAf + CtaChaB)
 {
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Cta  "<<""
 <<setw(5)<<setfill(' ')<<"B01"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 
 
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Cta  "<<""
 <<setw(5)<<setfill(' ')<<"B02"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 
 
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Cta  "<<""
 <<setw(5)<<setfill(' ')<<"C03"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 
 
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Cta  "<<""
 <<setw(5)<<setfill(' ')<<"C04"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Cta  "<<""
 <<setw(5)<<setfill(' ')<<"D05"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 }
 }
 }
 
 
 
 
 
 // ************** For Bnt
 for(int a = 0;  a < ceil(BntChain/10000) + 1; a++)
 {
 for(int b = 0;  b < 9999; b++)
 {
 if ((countBead >= WAf + SESChaB + CtaChaB + CL) && (countBead < WAf + SESChaB + CtaChaB + CL + BntChaB))
 {
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Bnt  "<<""
 <<setw(5)<<setfill(' ')<<"E06"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 
 
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Bnt  "<<""
 <<setw(5)<<setfill(' ')<<"E07"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 
 
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Bnt  "<<""
 <<setw(5)<<setfill(' ')<<"E08"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 
 
 witea<<" "<<setw(4)<<setfill(' ')<<b+1<<"" <<setw(5)<<setfill(' ')<<"Bnt  "<<""
 <<setw(5)<<setfill(' ')<<"F09"<<""    <<setfill(' ')<<setw(5)<<BeadID[countBead]<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Xu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Yu[countBead]/10<<""
 <<setw(8)<<fixed<<setprecision(3)<<setfill(' ')<<Zu[countBead]/10<<"\n";
 countBead++;
 }
 }
 }

 
*/
