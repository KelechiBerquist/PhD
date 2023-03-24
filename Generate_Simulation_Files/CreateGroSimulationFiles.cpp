#include<iostream>
#include<fstream>
#include<string>
#include <sstream>
#include<cmath>
#include <vector>
#include <iomanip>
#include <list>
using std::vector;
using namespace std;
using std::string;
using std::cout;



//This is an updated lammps script that puts Water first, Antifreeze water next, Cta and CounterMainChain 3rd and 4th, Bnt and CounterSoluteChain 5th and 6th.

string itos(int number, int precision, int width)
{
	stringstream s;
	s <<setw(width)<<fixed <<setprecision (precision) << number;
	return s.str();
}

string ftos(float number, int precision, int width)
{
	stringstream s;
	s <<setw(width)<<fixed <<setprecision (precision) << number;
	return s.str();
}

string dtos(double number, int precision, int width)
{
	stringstream s;
	s <<setw(width)<<fixed <<setprecision (precision) << number;
	return s.str();
}

int stroi (string s)
{
	int numb;
	istringstream iss (s);
	iss >> numb;
	
	return numb;
}

class systemContent {
	public:
	string Main; 
	string Solute;
};
 
#define AFW     40e3                              
#define WA      400e3
#define NATop   "NA+"     
#define CLTop   "CL-"
#define B0ZTop  "B0Z"



string ReadWriteFolderName,     SharedGromacsFiles;

int main ()
{
	
	string  LocatePC = "/media/KaceHDD1/";
	//string  LocatePC = "/media/OkwuchisHDD/";
	
	
	int RunCores = 2;
	
	if (LocatePC == "/media/KaceHDD1/")   {SharedGromacsFiles = "/home/kelechi/SharedGromacsFiles/";}
	if (LocatePC == "/media/OkwuchisHDD/"){SharedGromacsFiles = "/home/okwuchi/SharedGromacsFiles/";}
	
	LocatePC += "Lipids/";
	
	
	vector <int>  MainChainVector = {2};
	vector <double> R = {0.3, 1.0, 3.0};
	vector <string> MdpType    = {"rf.mdp", "npt.mdp"};
	
	
	
	for (int forChain = 0;  forChain < MainChainVector.size();  forChain++)
	{
		int MainChain        =   MainChainVector[forChain]*1000; 
		int CounterMainChain =   MainChain; 
		
		vector <int>    MainVector  = {2, 4, 6};
		vector <string> SystemIndex = {"PN", "ZZ"};
		
		
		
		for (int forIndex = 0;  forIndex < SystemIndex.size();  forIndex++)
		{
			
			for (int forMain = 0;  forMain < MainVector.size();  forMain++)
			{
			 	string  MainTop  =  "B" + itos(MainVector[forMain],0,1) + SystemIndex[forIndex][0];
				string  MainPdb  =  SharedGromacsFiles +  "peo"  + itos(MainVector[forMain]+1,0,1) + ".pdb";
				string  RunPC    =  LocatePC  +  itos(MainChainVector[forChain],0,1) + "/" + MainTop + "/";
				
				
				string CounterMainTop,    CounterMainPdb;
				string CounterSoluteTop,  CounterSolutePdb;
				
				if (MainTop[2] == 'P')
				{
					CounterMainPdb  =  SharedGromacsFiles + "peo1.pdb";
					CounterMainTop  =  CLTop;
				}
				if (MainTop[2] == 'Z')
				{
					CounterMainPdb  =  SharedGromacsFiles + "peo1.pdb";
					CounterMainTop  =  B0ZTop;
				}
				
				
				string WriteFileName8, WriteFileName10;
				
				
				WriteFileName8   =    RunPC + "run_" + MainTop + "_Micelle_Equilibrium";
				ofstream WriteFile8 (WriteFileName8.c_str());
				WriteFile8<<"#!/bin/bash \n";
				WriteFile8<<" \n \n";
				WriteFile8<<"source /usr/local/gromacs/bin/GMXRC\n";
				WriteFile8<<" \n \n";
				
				
				WriteFileName10   =    RunPC + "run_" + MainTop + "_Micelle_PreRun_Equilibrium";
				ofstream WriteFile10 (WriteFileName10.c_str());
				WriteFile10<<"#!/bin/bash \n";
				WriteFile10<<" \n \n";
				WriteFile10<<"source /usr/local/gromacs/bin/GMXRC\n";
				WriteFile10<<" \n \n";
				
				
				
				vector <int> SoluteVector = {1,2,3,4,6};
				
				
				for (int forSoluteType = 0;  forSoluteType < SoluteVector.size();  forSoluteType++)
				{
					string SoluteTop =  "B" + itos(SoluteVector[forSoluteType],0,1) + SystemIndex[forIndex][1];
					string SolutePdb = SharedGromacsFiles +  "peo"  + itos(SoluteVector[forSoluteType]+1,0,1) + ".pdb";
					
					
					if (SoluteTop[2] == 'N')
					{
						CounterSolutePdb  =  SharedGromacsFiles + "peo1.pdb";
						CounterSoluteTop  =  NATop;
					}
					if (SoluteTop[2] == 'Z')
					{
						CounterSolutePdb  =  SharedGromacsFiles + "peo1.pdb";
						CounterSoluteTop  =  B0ZTop;
					}
					
					
					for (int r = 0;  r < R.size();  r++)
					{
						ReadWriteFolderName  =  RunPC + SoluteTop + "/" + dtos(R[r],1,3) + "/" + dtos(R[r],1,3) + "-EquilibriumGromacs/";
						
						
						double BoxLength     =   450;
						double xhi =  0.5*BoxLength,    yhi =  0.5*BoxLength,   zhi =  0.5*BoxLength;
						double xlo = -0.5*BoxLength,    ylo = -0.5*BoxLength,   zlo = -0.5*BoxLength;
						
						
						int SoluteChain        =   int(R[r]*MainChain); 
						int CounterSoluteChain =   SoluteChain; 
						
						
						string WriteFileName1   =    ReadWriteFolderName + "sys.top";
						ofstream WriteFile1 (WriteFileName1.c_str());
						
						WriteFile1<<"#include \""<<SharedGromacsFiles <<"martini_v2.0.itp\" " <<"\n";
						WriteFile1<<"#include \""<<SharedGromacsFiles <<"martini_MyMicelle.itp\" " <<"\n";
						
						WriteFile1 <<"\n \n";
						
						WriteFile1<<"[ system ] " <<"\n";
						WriteFile1 <<MainChain<<"="<<MainTop <<"   " <<CounterMainChain<<"="<<CounterMainTop<<"   "  <<SoluteChain<<"="<<SoluteTop<<"   "  <<CounterSoluteChain<<"="<<CounterSoluteTop<<"   "<<"R="<<dtos(R[r],1,3)<<"  In  P4-BP4  Solution  " <<"\n";
						
						
						//cout<<MainTop <<"_" <<SoluteTop<<"_\n";
						
						//cout<<ReadWriteFolderName<<"_"<<MainChain<<"="<<MainTop <<"   " <<CounterMainChain<<"="<<CounterMainTop<<"   "  <<SoluteChain<<"="<<SoluteTop<<"   "  <<CounterSoluteChain<<"="<<CounterSoluteTop<<"   "<<"R="<<dtos(R[r],1,3)<<"  In  P4-BP4  Solution  " <<"\n";
						
						
						
						WriteFile1 <<"\n \n";
						WriteFile1<<"[ molecules ] " <<"\n";
						
						WriteFile1<<left<<setw(9)<<"W"            << WA <<"  \n";
						WriteFile1<<left<<setw(9)<<"WF"           << AFW <<"  \n";
						WriteFile1<<left<<setw(9)<<MainTop        << MainChain <<"  \n";
						WriteFile1<<left<<setw(9)<<CounterMainTop << CounterMainChain <<"  \n";
						
						if (SoluteChain > 0)
						{
							WriteFile1<<left<<setw(9)<<SoluteTop         << SoluteChain <<"  \n";
							WriteFile1<<left<<setw(9)<<CounterSoluteTop  << CounterSoluteChain <<"  \n";
						}
						WriteFile1.close();
						
						
						string WriteFileName2   =    ReadWriteFolderName  +    "pack.Micelle";
						ofstream WriteFile2 (WriteFileName2.c_str());
						
						WriteFile2<<"tolerance 2.0 " <<"\n";
						WriteFile2<<"filetype  pdb" <<"\n";
						WriteFile2<<"output MicelleWa.pdb" <<"\n";
						
						WriteFile2 <<"\n \n";
						WriteFile2<<"structure  "<<SharedGromacsFiles <<"water.pdb" <<"\n";
						WriteFile2<<"  number  " <<WA <<"  \n";
						WriteFile2<<"  inside  box  "<<xlo<<" "<<ylo<<" "<<zlo <<" "<<xhi<<" "<<yhi<<" "<<zhi<<"" <<"\n";
						WriteFile2<<"end  structure" <<"\n";
						
						WriteFile2 <<"\n \n";
						WriteFile2<<"structure  "<<SharedGromacsFiles <<"antifreezewater.pdb" <<"\n";
						WriteFile2<<"  number  " <<AFW <<"  \n";
						WriteFile2<<"  inside  box  "<<xlo<<" "<<ylo<<" "<<zlo <<" "<<xhi<<" "<<yhi<<" "<<zhi<<"" <<"\n";
						WriteFile2<<"end  structure" <<"\n";
						
						WriteFile2 <<"\n \n";
						WriteFile2<<"structure  " <<MainPdb <<"\n";
						WriteFile2<<"  number  "  <<MainChain <<"  \n";
						WriteFile2<<"  inside  box  "<<xlo<<" "<<ylo<<" "<<zlo <<" "<<xhi<<" "<<yhi<<" "<<zhi<<"" <<"\n";
						WriteFile2<<"end  structure" <<"\n";
						
						
						WriteFile2 <<"\n \n";
						WriteFile2<<"structure  "<< CounterMainPdb <<"\n";
						WriteFile2<<"  number  " << CounterMainChain <<"  \n";
						WriteFile2<<"  inside  box  "<<xlo<<" "<<ylo<<" "<<zlo <<" "<<xhi<<" "<<yhi<<" "<<zhi<<"" <<"\n";
						WriteFile2<<"end  structure" <<"\n";
						
						if (SoluteChain > 0)
						{
							WriteFile2 <<"\n \n";
							WriteFile2<<"structure  "<<SolutePdb <<"\n";
							WriteFile2<<"  number  "<< SoluteChain <<"  \n";
							WriteFile2<<"  inside  box  "<<xlo<<" "<<ylo<<" "<<zlo <<" "<<xhi<<" "<<yhi<<" "<<zhi<<"" <<"\n";
							WriteFile2<<"end  structure" <<"\n";
							
							WriteFile2 <<"\n \n";
							WriteFile2<<"structure  "<< CounterSolutePdb <<"\n";
							WriteFile2<<"  number  " << CounterSoluteChain <<"  \n";
							WriteFile2<<"  inside  box  "<<xlo<<" "<<ylo<<" "<<zlo <<" "<<xhi<<" "<<yhi<<" "<<zhi<<"" <<"\n";
							WriteFile2<<"end  structure" <<"\n";
						}
						
						WriteFile2.close(); 
						
						
						
						string   tauTValue = "0.5",    refTempValue  = "300";
						string   TempCoupleGroups,     RefTemp,    TauT;
						
						if (SoluteChain > 0)
						{
							if (CounterMainTop == SoluteTop)
							{
								TempCoupleGroups = "W_WF  "     + MainTop + "   " + CounterMainTop + "   "  + CounterSoluteTop;
								RefTemp          = refTempValue + "   " + refTempValue    + "   " + refTempValue              + "   "  + refTempValue;
								TauT             = tauTValue    + "   " + tauTValue       + "   " + tauTValue                 + "   "  + tauTValue;
							}
							
							else if (CounterMainTop == CounterSoluteTop and MainTop != SoluteTop)
							{
								TempCoupleGroups = "W_WF  "     + MainTop + "   " + CounterMainTop + "   "  + SoluteTop;
								RefTemp          = refTempValue + "   " + refTempValue    + "   " + refTempValue              + "   "  + refTempValue;
								TauT             = tauTValue    + "   " + tauTValue       + "   " + tauTValue                 + "   "  + tauTValue;
							}
							
							else if (MainTop == SoluteTop)
							{
								TempCoupleGroups = "W_WF  "     + MainTop + "   " + CounterMainTop;
								RefTemp          = refTempValue + "   " + refTempValue    + "   " + refTempValue ;
								TauT             = tauTValue    + "   " + tauTValue       + "   " + tauTValue    ;
							}
							
							else
							{
								TempCoupleGroups = "W_WF  "     + MainTop + "   " + CounterMainTop + "   " + SoluteTop  + "   " + CounterSoluteTop ;
								RefTemp          = refTempValue + "   " + refTempValue    + "   " + refTempValue              + "   " + refTempValue           + "   " + refTempValue;
								TauT             = tauTValue    + "   " + tauTValue       + "   " + tauTValue                 + "   " + tauTValue              + "   " + tauTValue;
							}
						}
						
						if (SoluteChain == 0)
						{
							TempCoupleGroups = "W_WF  "     + MainTop + "   " + CounterMainTop;
							RefTemp          = refTempValue + "   " + refTempValue    + "   " + refTempValue             ;
							TauT             = tauTValue    + "   " + tauTValue       + "   " + tauTValue                ;
						}
						
						
						for (int a0 = 0;  a0 < MdpType.size();  a0++ )
						{
							string WriteFileName3   =    ReadWriteFolderName  +  MdpType[a0];
							
							ofstream WriteFile3 (WriteFileName3.c_str());
							
							WriteFile3<<left <<setw(30)<<setfill(' ')<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;  INTRO  ;;;;;;;;;;;;;;;;;;;;;;;;;;;; " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"integrator "<<setw(5)<<"="<<setw(45)<<"md " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"pbc "       <<setw(5)<<"="<<setw(45)<<"xyz " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"tinit "     <<setw(5)<<"="<<setw(45)<<"0 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"init-step " <<setw(5)<<"="<<setw(45)<<"0 " <<"\n";
							
							if (MdpType[a0] == "npt.mdp" )
							{
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"dt "        <<setw(5)<<"="<<setw(45)<<"0.01 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"nsteps "    <<setw(5)<<"="<<setw(45)<<"2000 " <<"\n";
							}
							
							else if (MdpType[a0] == "rf.mdp" || MdpType[a0] == "nvt.mdp" )
							{
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"dt "        <<setw(5)<<"="<<setw(45)<<"0.04 " <<"\n";
								//WriteFile3<<left <<setw(30)<<setfill(' ')<<"nsteps "    <<setw(5)<<"="<<setw(45)<<"-1 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"nsteps "    <<setw(5)<<"="<<setw(45)<<"25000000 " <<"\n";
							}
							
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"comm-mode " <<setw(5)<<"="<<setw(45)<<"" <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstcomm "   <<setw(5)<<"="<<setw(45)<<"100 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"comm-grps " <<setw(5)<<"="<<setw(45)<<TempCoupleGroups <<"\n";       
							
							
							WriteFile3<<"\n \n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;  WRITING TO FILES  ;;;;;;;;;;;;;;;;;;;;;;;;;;;; " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"energygrps "   <<setw(5)<<"="<<setw(45)<<TempCoupleGroups <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstlog "     <<setw(5)<<"="<<setw(45)<<"20000 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstxout "    <<setw(5)<<"="<<setw(45)<<"100000 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstvout "    <<setw(5)<<"="<<setw(45)<<"100000 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstfout "    <<setw(5)<<"="<<setw(45)<<"0 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstenergy "  <<setw(5)<<"="<<setw(45)<<"2000 " <<"\n";
							
							
							WriteFile3<<"\n \n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;  NEIGHBOUR LIST  ;;;;;;;;;;;;;;;;;;;;;;;;;;;; " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstlist "            <<setw(5)<<"="<<setw(45)<<"25 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"ns_type "            <<setw(5)<<"="<<setw(45)<<"grid " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"cutoff-scheme "      <<setw(5)<<"="<<setw(45)<<"Verlet " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"rlist "              <<setw(5)<<"="<<setw(45)<<"1.1 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"coulomb-modifier "   <<setw(5)<<"="<<setw(45)<<"potential-shift-verlet " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"vdw-modifier "       <<setw(5)<<"="<<setw(45)<<"potential-shift-verlet " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"verlet-buffer-drift "<<setw(5)<<"="<<setw(45)<<"0.005 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"; DispCorr "         <<setw(5)<<"="<<setw(45)<<"EnerPres " <<"\n";
							
							
							WriteFile3<<"\n \n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;  INTERACTION PARAMETERS  ;;;;;;;;;;;;;;;;;;;;;;;;;;;; " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"vdwtype "         <<setw(5)<<"="<<setw(45)<<"cutoff " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"rvdw "            <<setw(5)<<"="<<setw(45)<<"1.1 " <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"rvdw_switch "     <<setw(5)<<"="<<setw(45)<<"0.9 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"coulombtype "     <<setw(5)<<"="<<setw(45)<<"reaction-field " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"rcoulomb_switch " <<setw(5)<<"="<<setw(45)<<"0.0 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"rcoulomb "        <<setw(5)<<"="<<setw(45)<<"1.1 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"epsilon_r "       <<setw(5)<<"="<<setw(45)<<"15 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"epsilon_rf "      <<setw(5)<<"="<<setw(45)<<"0 " <<"\n";
							//WriteFile3<<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; rlist "         <<setw(5)<<"="<<setw(45)<<"1.2 " <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; coulombtype "   <<setw(5)<<"="<<setw(45)<<"PME " <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; fourierspacing "<<setw(5)<<"="<<setw(45)<<"0.32 " <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; pme-order  "    <<setw(5)<<"="<<setw(45)<<"4 "  <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; optimize_fft "  <<setw(5)<<"="<<setw(45)<<"yes " <<"\n";
							
							
							WriteFile3<<"\n \n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;  TEMPERATURE AND PRESSURE COUPLING  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;  " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"tcoupl "          <<setw(5)<<"="<<setw(45)<<"v-rescale " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"tc-grps "         <<setw(5)<<"="<<setw(45)<<TempCoupleGroups <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"tau-t "           <<setw(5)<<"="<<setw(45)<<TauT <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"ref-t "           <<setw(5)<<"="<<setw(45)<<RefTemp <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"nsttcouple "      <<setw(5)<<"="<<setw(45)<<"20 " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"refcoord-scaling "<<setw(5)<<"="<<setw(45)<<"all " <<"\n";
							
							if (MdpType[a0] == "npt.mdp" )
							{
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"Pcoupl "          <<setw(5)<<"="<<setw(45)<<"Berendsen " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"Pcoupltype "      <<setw(5)<<"="<<setw(45)<<"isotropic " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"tau-p "           <<setw(5)<<"="<<setw(45)<<"2.0 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"ref-p "           <<setw(5)<<"="<<setw(45)<<"1.0 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"compressibility " <<setw(5)<<"="<<setw(45)<<"4.5895e-5 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstpcouple "      <<setw(5)<<"="<<setw(45)<<"20 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"gen-vel "         <<setw(5)<<"="<<setw(45)<<"yes " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"gen-temp "        <<setw(5)<<"="<<setw(45)<<"300 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"gen-seed "        <<setw(5)<<"="<<setw(45)<<"1807253 " <<"\n";
								
							}
							
							if (MdpType[a0] == "rf.mdp")
							{
								//WriteFile3<<left <<setw(30)<<setfill(' ')<<"Pcoupl "          <<setw(5)<<"="<<setw(45)<<"Parrinello-Rahman " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"Pcoupl "          <<setw(5)<<"="<<setw(45)<<"Berendsen " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"Pcoupltype "      <<setw(5)<<"="<<setw(45)<<"isotropic " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"tau-p "           <<setw(5)<<"="<<setw(45)<<"12.0 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"ref-p "           <<setw(5)<<"="<<setw(45)<<"1.0 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"compressibility " <<setw(5)<<"="<<setw(45)<<"4.5895e-5 " <<"\n";
								WriteFile3<<left <<setw(30)<<setfill(' ')<<"nstpcouple "      <<setw(5)<<"="<<setw(45)<<"25 " <<"\n";
							}
							
							
							WriteFile3<<"\n \n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;  BONDS PARAMETERS  ;;;;;;;;;;;;;;;;;;;;;;;;;;;; " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"constraints "         <<setw(5)<<"="<<setw(45)<<"none " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"constraint-algorithm "<<setw(5)<<"="<<setw(45)<<"LINCS " <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"lincs-order "         <<setw(5)<<"="<<setw(45)<<"4" <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"lincs-warnangle "     <<setw(5)<<"="<<setw(45)<<"90" <<"\n";
							WriteFile3<<left <<setw(30)<<setfill(' ')<<"shake_tol "           <<setw(5)<<"="<<setw(45)<<"0.0001" <<"\n";
							
							
							//WriteFile3<<"\n \n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;;  Simulated annealing  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;" <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; annealing "         <<setw(5)<<"="<<setw(45)<<"single    single     single  ; single sequence of points for each T-coupling group" <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; annealing_npoints  "<<setw(5)<<"="<<setw(45)<<"2         2          2       ; two points - start and end temperatures" <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; annealing_time "    <<setw(5)<<"="<<setw(45)<<"0   1000  0   1000   0   1000; time frame of heating - heat over period of 500 ps" <<"\n";
							//WriteFile3<<left <<setw(30)<<setfill(' ')<<"; annealing_temp "    <<setw(5)<<"="<<setw(45)<<"500 310   500 310    500 310 ; start and end temperatures" <<"\n";
							
							
							WriteFile3.close();
							
						} //For MdpType
						
						
					} //For R
					 
					 
					
					string  ReadWriteFolderName2   =  RunPC + SoluteTop + "/BashScripts/"; 
					
					string WriteFileName5,  WriteFileName6,   WriteFileName7,   WriteFileName9;
					
					
					
					WriteFileName6   =    ReadWriteFolderName2 + "runEquilGro";
					ofstream WriteFile6 (WriteFileName6.c_str());
					
					WriteFile6<<"#!/bin/bash \n";
					WriteFile6<<" \n \n";
					WriteFile6<<"source /usr/local/gromacs/bin/GMXRC\n";
					WriteFile6<<" \n \n";
					
					WriteFileName9   =    ReadWriteFolderName2 + "preRunEquilGro";
					ofstream WriteFile9 (WriteFileName9.c_str());
					
					WriteFile9<<"#!/bin/bash \n";
					WriteFile9<<" \n \n";
					//WriteFile9<<"source /usr/local/gromacs/bin/GMXRC\n";
					WriteFile9<<" \n \n";
					
					for (int r = 0;  r < R.size();  r++ )
					{
						string  WriteFolderName   =  RunPC + SoluteTop + "/" + dtos(R[r],1,3) + "/" + dtos(R[r],1,3) + "-EquilibriumGromacs/";
						string  ReadFolderName    =  RunPC + SoluteTop + "/" + dtos(R[r],1,3) + "/" + dtos(R[r],1,3) + "-EquilibriumGromacs/";
						
						WriteFile6<<"cd    "<<WriteFolderName<<" \n";
						WriteFile6<<"gmx  grompp  -f  rf.mdp   -c NPT1.gro   -p sys.top   -o  NPT2.tpr  -n index.ndx   -maxwarn  -1 \n";
						WriteFile6<<"gmx  mdrun   -v  -deffnm  NPT2  -nt  "<<RunCores <<" -nb gpu -tunepme\n";
						WriteFile6<<" \n \n";
						
						
						//WriteFile9<<"cd    "<<WriteFolderName<<" \n";
						//WriteFile9<<"packmol < pack.Micelle \n \n";
						//WriteFile9<<"gmx editconf -f  MicelleWa.pdb  -o  MicelleWa.gro  -c -d 0.00 \n";
						//WriteFile9<<"gmx  grompp  -f  "<<SharedGromacsFiles <<"em.mdp   -c MicelleWa.gro   -p sys.top   -o EM.tpr   -maxwarn  -1 \n";
						//WriteFile9<<"gmx  mdrun -v -deffnm EM -nt "<<RunCores <<"  -nb gpu -tunepme \n";
						WriteFile9<<"gmx make_ndx  -f EM.gro   -o index.ndx << EOF\n";
						WriteFile9<<"2|3 \n";
						WriteFile9<<"q \n";
						WriteFile9<<"EOF\n \n";
						
						WriteFile9<<"cd    "<<WriteFolderName<<" \n";
						WriteFile9<<"gmx  grompp  -f npt.mdp   -c EM.gro   -p sys.top   -o NPT1.tpr  -n index.ndx   -maxwarn  -1 \n";
						WriteFile9<<"gmx  mdrun -v -deffnm NPT1 -nt "<<RunCores <<"  -nb gpu -tunepme  \n";
						WriteFile9<<"gmx editconf -f  NPT1.gro  -o   NPT1.pdb   \n";
						
						WriteFile9<<" \n \n \n";
					}
					
					WriteFile6.close();
					WriteFile9.close();
					
					WriteFile8 <<"sh  " <<WriteFileName6 <<" \n \n";
					WriteFile10<<"sh  " <<WriteFileName9 <<" \n \n";
					
					
				} //For SoluteTop
			
			} //For Main Phase
		} //For PN or ZZ
	} // For Number of Main
	
	return 0;
	
}
