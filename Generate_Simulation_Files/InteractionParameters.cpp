
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <set>
#include <list>
#include <ctime>
#include <deque>
#include <tuple>

#include "../CommonFunctions.h"
#include "InteractionParameters.h"



//For Solute
#define BondForceConstSC4_Qa             dtosPW(0.5*1250*0.01/4.187, 5, 15)      
#define EquilibriumBondLengthSC4_Qa      dtosPW(4.7, 2, 15)
#define BondForceConstSC4_SC4            dtosPW(50.00, 0, 15)                
#define ConstraintSC4_SC4                dtosPW(2.7, 2, 15)
#define EquilibriumAngleSC4_SC4_SC4      dtosPW(60, 1, 15)
#define EquilibriumAngleQa_SC41_SC42     dtosPW(150, 1, 15)
#define EquilibriumAngleQa_SC41_SC43     dtosPW(210, 1, 15)

//For Cta
#define BondForceConstCta                dtosPW(0.5*1250*0.01/4.187, 5, 15)      
#define EquilibriumBondLengthCta         dtosPW(4.7, 2, 15)
#define AngleForceConstCtaSolute         dtosPW(0.5*25/4.187, 5, 15)             
#define EquilibriumAngleCta              dtosPW(180, 1, 15)

#define Eps_supra_attractive             dtosPW(5.6/4.187, 5, 15)                
#define Eps_attractive                   dtosPW(5.0/4.187, 5, 15)                 
#define Eps_almost_attractive            dtosPW(4.5/4.187, 5, 15)                 
#define Eps_semi_attractive              dtosPW(4.0/4.187, 5, 15)                
#define Eps_intermediate                 dtosPW(3.5/4.187, 5, 15)                 
#define Eps_almost_intermediate          dtosPW(3.1/4.187, 5, 15)                 
#define Eps_semi_repulsive               dtosPW(2.7/4.187, 5, 15)                 
#define Eps_almost_repulsive             dtosPW(2.3/4.187, 5, 15)                
#define Eps_repulsive                    dtosPW(2.0/4.187, 5, 15)                 
#define Eps_super_repulsive              dtosPW(2.0/4.187, 5, 15)               
#define Eps_SC4_SC4                      dtosPW(2.625/4.187, 5, 15)               
#define Eps_P4_BP4                       dtosPW(5.60/4.187, 5, 15)               

#define Sig_supra_attractive             dtosPW(4.7, 2, 10)
#define Sig_attractive                   dtosPW(4.7, 2, 10)
#define Sig_almost_attractive            dtosPW(4.7, 2, 10)
#define Sig_semi_attractive              dtosPW(4.7, 2, 10)
#define Sig_intermediate                 dtosPW(4.7, 2, 10)
#define Sig_almost_intermediate          dtosPW(4.7, 2, 10)
#define Sig_semi_repulsive               dtosPW(4.7, 2, 10)
#define Sig_almost_repulsive             dtosPW(4.7, 2, 10)
#define Sig_repulsive                    dtosPW(4.7, 2, 10)
#define Sig_super_repulsive              dtosPW(6.2, 2, 10)
#define Sig_SC4_SC4                      dtosPW(4.3, 2, 10)
#define Sig_P4_BP4                       dtosPW(5.7, 2, 10)

#define Supra_attractive                 Eps_supra_attractive    + Sig_supra_attractive
#define Attractive                       Eps_attractive          + Sig_attractive
#define Almost_attractive                Eps_almost_attractive   + Sig_almost_attractive
#define Semi_attractive                  Eps_semi_attractive     + Sig_semi_attractive
#define Intermediate                     Eps_intermediate        + Sig_intermediate
#define Almost_intermediate              Eps_almost_intermediate + Sig_almost_intermediate
#define Semi_repulsive                   Eps_semi_repulsive      + Sig_semi_repulsive
#define Almost_repulsive                 Eps_almost_repulsive    + Sig_almost_repulsive
#define Repulsive                        Eps_repulsive           + Sig_repulsive
#define Super_repulsive                  Eps_super_repulsive     + Sig_super_repulsive
#define SC4_SC4                          Eps_SC4_SC4             + Sig_SC4_SC4
#define P4_BP4                           Eps_P4_BP4              + Sig_P4_BP4




std::string InteractionNormalLipidAshishCta (std::string MainTop, std::string SoluteTop)
{
	std::string ReturnValue = "";
	
	ReturnValue += "\n";
	ReturnValue += "\n";
	
	ReturnValue += ( FormatString("bond_coeff", "left", 20)  +  FormatString("1", "left", 5) + BondForceConstCta + EquilibriumBondLengthCta  +"\n");
	if (SoluteTop == "NaSal") 
	{
		ReturnValue += ( FormatString("bond_coeff", "left", 20)  +  FormatString("2", "left", 5) + BondForceConstSC4_SC4 + ConstraintSC4_SC4  +"\n");
	}
	ReturnValue += "\n";
	
	
	ReturnValue += ( FormatString("#Parameter", "left", 20)  +  FormatString("AngleType", "", 20) + FormatString("AngleForceConstant", "", 20) + FormatString("EquilibriumAngle", "", 20)  +"\n");
	ReturnValue += ( FormatString("angle_coeff", "left", 20) +  FormatString("1", "", 5) + AngleForceConstCtaSolute + EquilibriumAngleCta  +"\n");
	
	
	if (SoluteTop == "NaSal") 
	{
		ReturnValue += ( FormatString("angle_coeff", "left", 20)  +  FormatString("2", "", 5) + AngleForceConstCtaSolute + EquilibriumAngleQa_SC41_SC42  +"\n");
		ReturnValue += ( FormatString("angle_coeff", "left", 20)  +  FormatString("3", "", 5) + AngleForceConstCtaSolute + EquilibriumAngleSC4_SC4_SC4  +"\n");
	}
	ReturnValue += "\n";
	
	
	
	ReturnValue += ( FormatString("#Parameter", "left", 20)  +  "Eps  " + "Sig  "  +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    1" + Attractive  +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    2" + P4_BP4  +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    3" + Supra_attractive  +"\n");
	
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    4" + Repulsive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    5" + Supra_attractive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    6" + Supra_attractive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    8" + Supra_attractive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    2" + Attractive +"\n"); //P4-P4
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    3" + Supra_attractive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    4" + Repulsive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    5" + Supra_attractive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    6" + Supra_attractive +"\n");
	ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    8" + Supra_attractive +"\n");
	
	if (MainTop == "CTA" and SoluteTop == "NaSal")
	{
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    7" + Semi_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    9" + Almost_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    7" + Semi_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    9" + Almost_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    3" + Intermediate +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    4" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    5" + Almost_attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    6" + Almost_attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    7" + Semi_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    8" + Almost_attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    9" + Super_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    4" + Intermediate +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    5" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    6" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    7" + Almost_intermediate +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    8" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    9" + Intermediate +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    5" + Attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    6" + Attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    7" + Semi_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    8" + Supra_attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    9" + Super_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "6    6" + Attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "6    7" + Semi_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "6    9" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "6    8" + Supra_attractive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "7    7" + Intermediate +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "7    8" + Semi_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "7    9" + Almost_intermediate +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "8    8" + Attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "8    9" + Super_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "9    9" + Intermediate +"\n");
	}
	
	else if (MainTop != "CTA" or SoluteTop == "Cta")
	{
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "1    7" + Repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "2    7" + Repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    3" + Attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    4" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    5" + Supra_attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    7" + Super_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    4" + Intermediate +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    5" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    6" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    7" + Intermediate +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "4    8" + Super_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    5" + Attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    7" + Super_repulsive +"\n");
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "6    6" + Attractive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "6    7" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "6    8" + Supra_attractive +"\n");
		
		if ( (MainTop[2] == 'a'  and SoluteTop[2] == 'n') or (MainTop[2] == 'n'  and SoluteTop[2] == 'a') )
		{
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    6" + Supra_attractive +"\n");
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    8" + Attractive +"\n");
			
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    6" + Attractive +"\n");
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    8" + Supra_attractive +"\n");
			
		}
		
		if ( (MainTop[2] == 'a'  and SoluteTop[2] == 'a') or (MainTop[2] == 'n'  and SoluteTop[2] == 'n') )
		{
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    6" + Attractive +"\n");
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "3    8" + Supra_attractive +"\n");
			
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    6" + Supra_attractive +"\n");
			ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "5    8" + Attractive +"\n");
		}
		
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "7    7" + Intermediate +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "7    8" + Super_repulsive +"\n");
		ReturnValue += ( FormatString("pair_coeff", "left", 20)  +  "8    8" + Attractive +"\n");
	}
	ReturnValue += "\n";
	ReturnValue += "\n";
	
	
	ReturnValue += ( FormatString("group ", "left", 20) + FormatString("water", "left", 20)       + "type  1  2\n");
	ReturnValue += ( FormatString("group ", "left", 20) + FormatString("ions", "left", 20)        + "type  5  8\n");
	ReturnValue += ( FormatString("group ", "left", 20) + FormatString("notmicelle", "left", 20)  + "type  1  2  5  8\n");
	
	if (MainTop == "CTA" or SoluteTop == "Cta")
	{
		ReturnValue += (FormatString( "group ", "left", 20) + FormatString("micelle", "left", 20)   + "type  3  4  6  7  9\n");
		ReturnValue += ( FormatString("group ", "left", 20) + FormatString("notwater", "left", 20)  + "type  3  4  6  7  9  5  8\n");
	}
	else if (SoluteTop == "NaSal")                
	{
		ReturnValue += ( FormatString("group ", "left", 20) + FormatString("micelle", "left", 20) + "type  3  4  6  7  9  10\n");
		ReturnValue += ( FormatString("group ", "left", 20) + FormatString("notwater", "left", 20) + "type  3  4  6  7  9  10  5  8\n");
	}
	else                                           
	{
		ReturnValue += ( FormatString("group ", "left", 20) + FormatString("micelle", "left", 20)   + "type  3  4  6  7\n");
		ReturnValue += ( FormatString("group ", "left", 20) + FormatString("notwater", "left", 20)  + "type  3  4  6  7  5  8\n");
	}
	
	return ReturnValue;
}
//// ******************** For Hysteresis
					
					//string FileNameEnd  = "UnLoad-Rate" + dtos(Rate) + "-Spn" + itosPW(chosen+1) + "-En" + ftosPW(encore);
					
					//WriteFile1+left+"#################################################################################################### \n");
					//WriteFile1+left+"#For Hysteresis \n");
					//WriteFile1+left+"#################################################################################################### \n");
					
					//ReturnValue += "\n";
					//ReturnValue += ( "log " +" Log" + FileNameEnd+".dat \n");
					//ReturnValue += "\n";
					
					//ReturnValue += ( "variable " + std::setw(10)+"temp1 "   +std::setw(8)+"equal" + " lx \n");
					//ReturnValue += ( "variable " + std::setw(10)+"Lx0 "     +std::setw(8)+"equal" + " ${temp1} \n");
					//ReturnValue += ( "variable " + std::setw(10)+"strain1 " +std::setw(8)+"equal" + " (lx-${Lx0})/${Lx0} \n");
					//ReturnValue += ( "variable " + std::setw(10)+"temp2 "   +std::setw(8)+"equal" + " ly \n");
					//ReturnValue += ( "variable " + std::setw(10)+"Ly0 "     +std::setw(8)+"equal" + " ${temp2} \n");
					//ReturnValue += ( "variable " + std::setw(10)+"strain2 " +std::setw(8)+"equal" + " (${Ly0}-ly)/${Ly0} \n");
					//ReturnValue += ( "variable " + std::setw(10)+"temp3 "   +std::setw(8)+"equal" + " lz \n");
					//ReturnValue += ( "variable " + std::setw(10)+"Lz0 "     +std::setw(8)+"equal" + " ${temp3} \n");
					//ReturnValue += ( "variable " + std::setw(10)+"strain3 " +std::setw(8)+"equal" + " (${Lz0}-lz)/${Lz0} \n");
					
					
					//ReturnValue += "\n";
					//ReturnValue += ( "fix " + std::setw(10)+"ensemble " + std::setw(5)+"all " + " npt    temp   $T  $T  " + itosPW(tdamp)+"  iso   1.0  1.0  " + itosPW(pdamp)+"  drag 2 \n");
					//ReturnValue += ( "fix " + std::setw(12)+"avtym1 "   +std::setw(5)+"all" + " ave/time  1  " + fixed+setprecision(0)+LogHystDump+"  "
					//+fixed+setprecision(0)+LogHystDump+"  v_unpxx1  v_unpyy1  v_unpzz1  v_stress1  file SystemAveFile" + FileNameEnd+".dat       \n");
					
					//ReturnValue += ( "fix " + std::setw(12)+"avtym2 "   +std::setw(5)+"all" + " ave/time  1  " + fixed+setprecision(0)+LogHystDump+"  "
					//+fixed+setprecision(0)+LogHystDump+"  v_unpxx2  v_unpyy2  v_unpzz2  v_stress2  file MicelleAveFile" + FileNameEnd+".dat       \n");
					
					
					//ReturnValue += "\n";
					//ReturnValue += ( "variable " + std::setw(12)+"avePxx1 "      +std::setw(8)+"equal" + " f_avtym1[1] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"avePyy1 "      +std::setw(8)+"equal" + " f_avtym1[2] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"avePzz1 "      +std::setw(8)+"equal" + " f_avtym1[3] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"aveStress1 "   +std::setw(8)+"equal" + " f_avtym1[4] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"tempTime "     +std::setw(8)+"equal" + " step \n");
					//ReturnValue += ( "variable " + std::setw(12)+"initialTime "  +std::setw(8)+"equal" + " ${tempTime} \n");
					//ReturnValue += ( "variable " + std::setw(12)+"time "         +std::setw(8)+"equal" + " step-${initialTime} \n");
					//ReturnValue += ( "variable " + std::setw(12)+"revTime "      +std::setw(8)+"equal" + " " + RunStep +"-(step-${initialTime}) \n");
					//ReturnValue += ( "variable " + std::setw(12)+"visc1 "        +std::setw(8)+"equal" + " ${aveStress1}*${viscMultiplier} \n");
					//ReturnValue += "\n";
					
					
					//ReturnValue += ( "variable " + std::setw(12)+"avePxx2 "      +std::setw(8)+"equal" + " f_avtym2[1] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"avePyy2 "      +std::setw(8)+"equal" + " f_avtym2[2] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"avePzz2 "      +std::setw(8)+"equal" + " f_avtym2[3] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"aveStress2 "   +std::setw(8)+"equal" + " f_avtym2[4] \n");
					//ReturnValue += ( "variable " + std::setw(12)+"visc2 "        +std::setw(8)+"equal" + " ${aveStress2}*${viscMultiplier} \n");
					
					
					//ReturnValue += ( "fix " + std::setw(12)+"def1 " + std::setw(5)+"all" + " print  " + fixed+setprecision(0)+LogHystDump+" \"${revTime}   ${time}   ${strain1}   ${strain2}   ${strain3}"
					//+"   ${avePxx1}   ${avePyy1}   ${avePzz1}   ${aveStress1}   ${visc1} \" file SystemFixFile" + FileNameEnd+".dat screen no \n");
					
					//ReturnValue += ( "fix " + std::setw(12)+"def2 " + std::setw(5)+"all" + " print  " + fixed+setprecision(0)+LogHystDump+" \"${revTime}   ${time}   ${strain1}   ${strain2}   ${strain3}"
					//+"   ${avePxx2}   ${avePyy2}   ${avePzz2}   ${aveStress2}   ${visc2} \" file MicelleFixFile" + FileNameEnd+".dat screen no \n");
					
					
					//WriteFile1+"#\n");
					//ReturnValue += ( "print " + "\"Initial Length, Lx0: ${Lx0}\" \n");
					//WriteFile1+"#\n");
					//ReturnValue += ( "thermo " + fixed+setprecision(0)+LogHystDump+" \n");
					//ReturnValue += ( "thermo_style " +"custom  step  temp press  lx  ly  lz  pxx  pyy  pzz pxy  pxz  pyz  ke pe \n");
					//ReturnValue += ( "thermo_modify " + "flush   yes \n");
					//ReturnValue += ( "thermo_modify " + "norm    yes \n");
					
					
					//ReturnValue += "\n";
					
					//ReturnValue += ( "dump " + std::setw(15) +"micelleDump " + std::setw(8)+"micelle " + " custom  " + fixed+setprecision(0)+TrjDumpShort
					//+"   Micelle" + FileNameEnd+".lammpstrj  id   type   x   y   z   xu   yu   zu   c_atomprop1   c_atomprop2   c_atomprop4   vx   vy   vz \n");
					
					//ReturnValue += ( "dump " + std::setw(15) +"waterDump " + std::setw(8)+"water " + " custom  " + fixed+setprecision(0)+TrjDumpLong
					//+"  Water" + FileNameEnd+".lammpstrj    id   type   x   y   z   xu   yu   zu   c_atomprop1   c_atomprop2   c_atomprop4   vx   vy   vz \n");
					
					
					//ReturnValue += "\n";
					
					//ReturnValue += ( "run_style " + "verlet \n");
					
					
					//ReturnValue += ( "run " + fixed+setprecision(0)+RunStep+" \n");
					//ReturnValue += ( "write_data " +"WrittenData" + FileNameEnd+"-*.dat \n");
					
					
					//ReturnValue += ( "undump " + "micelleDump \n");
					//ReturnValue += ( "undump " + "waterDump \n");
					
					//ReturnValue += ( "unfix " +"ensemble \n");
					//ReturnValue += ( "unfix " +"avtym1 \n");
					//ReturnValue += ( "unfix " +"avtym2 \n");
					//ReturnValue += ( "unfix " +"def1 \n");
					//ReturnValue += ( "unfix " +"def2 \n");
					
