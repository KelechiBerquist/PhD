#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cmath>

#include "../CommonFunctions.h"
#include "WritingLammpsFiles.h"

using std::string;
using std::cout;
using std::ofstream;
using std::ifstream;
using std::vector;


//
string LogToPairModify (mysystem &CurrentSystem)
{
	string ReturnValue = "";
	
	
	ReturnValue += ("\n" + 
	FormatString("log ", "left", 20)+"Log"+CurrentSystem.fileNameEnd+".dat \n" +
	FormatString("units ", "left", 20)+"real \n" +
	FormatString("boundary ", "left", 20)+"p   p   p \n" +
	
	FormatString("atom_style ", "left", 20)+"full \n" +
	FormatString("dielectric ", "left", 20)+"15.000 \n" +
	FormatString("special_bonds ", "left", 20)+"lj   0.0   1.0  1.0  coul  0.0  1.0  1.0 \n" +
	FormatString("bond_style ", "left", 20)+"harmonic  \n" +
	FormatString("angle_style ", "left", 20)+"harmonic  \n" +
	FormatString("pair_style ", "left", 20)+"lj/cut/coul/cut   11.0   11.0 \n" +
	FormatString("pair_modify ", "left", 20)+"shift  yes \n");
	
	return ReturnValue;
}



//
string VariableLxToScale (int deltaT)
{
	string ReturnValue = "";
	
	
	ReturnValue += ("\n" +
	FormatString("variable ", "left", 20) + FormatString("lx ", "left", 20)       + FormatString("equal", "left", 10) + "   lx \n" + 
	FormatString("variable ", "left", 20) + FormatString("ly ", "left", 20)       + FormatString("equal", "left", 10) + "   ly \n" + 
	FormatString("variable ", "left", 20) + FormatString("lz ", "left", 20)       + FormatString("equal", "left", 10) + "   lz \n" + 
	FormatString("variable ", "left", 20) + FormatString("pxy ", "left", 20)      + FormatString("equal", "left", 10) + "   pxy \n" + 
	FormatString("variable ", "left", 20) + FormatString("pxz ", "left", 20)      + FormatString("equal", "left", 10) + "   pxz \n" + 
	FormatString("variable ", "left", 20) + FormatString("pyz ", "left", 20)      + FormatString("equal", "left", 10) + "   pyz \n" + 
	FormatString("variable ", "left", 20) + FormatString("pxx ", "left", 20)      + FormatString("equal", "left", 10) + "   pxx \n" + 
	FormatString("variable ", "left", 20) + FormatString("pzz ", "left", 20)      + FormatString("equal", "left", 10) + "   pzz \n" + 
	FormatString("variable ", "left", 20) + FormatString("pyy ", "left", 20)      + FormatString("equal", "left", 10) + "   pyy \n" + 
	FormatString("variable ", "left", 20) + FormatString("dim ", "left", 20)      + FormatString("equal", "left", 10) + "   3 \n" + 
	FormatString("variable ", "left", 20) + FormatString("a ", "left", 20)        + FormatString("equal", "left", 10) + "   5 \n" + 
	FormatString("variable ", "left", 20) + FormatString("b ", "left", 20)        + FormatString("equal", "left", 10) + "   400 \n" + 
	FormatString("variable ", "left", 20) + FormatString("c ", "left", 20)        + FormatString("equal", "left", 10) + "   $a*$b \n" + 
	FormatString("variable ", "left", 20) + FormatString("delta_t ", "left", 20)  + FormatString("equal", "left", 10) + "   " + itos(deltaT)  + "\n" + 
	FormatString("variable ", "left", 20) + FormatString("V ", "left", 20)        + FormatString("equal", "left", 10) + "   vol \n" + 
	FormatString("variable ", "left", 20) + FormatString("T ", "left", 20)        + FormatString("equal", "left", 10) + "   300 \n" + 
	FormatString("variable ", "left", 20) + FormatString("kB ", "left", 20)       + FormatString("equal", "left", 10) + "   1.3806504e-23 \n" + 
	FormatString("variable ", "left", 20) + FormatString("atm2Pa ", "left", 20)   + FormatString("equal", "left", 10) + "   101325 \n" + 
	FormatString("variable ", "left", 20) + FormatString("A2m ", "left", 20)      + FormatString("equal", "left", 10) + "   1e-10 \n" + 
	FormatString("variable ", "left", 20) + FormatString("fs2s ", "left", 20)     + FormatString("equal", "left", 10) + "   1e-15 \n" + 
	FormatString("variable ", "left", 20) + FormatString("convert ", "left", 20)  + FormatString("equal", "left", 10) + "   ${atm2Pa}*${atm2Pa}*${fs2s}*${A2m}*${A2m}*${A2m} \n" + 
	FormatString("variable ", "left", 20) + FormatString("scale ", "left", 20)    + FormatString("equal", "left", 10) + "   ${convert}/(${kB}*$T)*$V*$a*${delta_t} \n");
	
	return ReturnValue;
}



//
string TimeStepToThermoModify (int deltaT)
{
	string ReturnValue = "";
	

	ReturnValue += ( 
		FormatString("\ntimestep ", "left", 20)     + "${delta_t} \n" + 
		FormatString("velocity ", "left", 20)     + "all    create   $T     4928459  rot     yes   dist   gaussian \n" + 
		FormatString("neighbor " , "left", 20)    + "21.0   bin \n" + 
		FormatString("neigh_modify ", "left", 20) + "delay  0        every  1        check   yes \n\n");
	

	int tdamp = deltaT*10;
	int pdamp = deltaT*10;

	ReturnValue += ( 
		FormatString("compute ", "left", 20) + FormatString("atomprop1 ", "left", 12) + FormatString("notwater ", "left", 8) + " pe/atom   pair \n" +
		FormatString("compute ", "left", 20) + FormatString("atomprop2 ", "left", 12) + FormatString("notwater ", "left", 8) + " pe/atom   bond \n" +
		FormatString("compute ", "left", 20) + FormatString("atomprop3 ", "left", 12) + FormatString("notwater ", "left", 8) + " pe/atom   kspace \n" +
		FormatString("compute ", "left", 20) + FormatString("atomprop4 ", "left", 12) + FormatString("notwater ", "left", 8) + " pe/atom   angle \n" +
		FormatString("compute ", "left", 20) + FormatString("atomprop5 ", "left", 12) + FormatString("notwater ", "left", 8) + " pe/atom \n\n" +

		FormatString("compute ", "left", 20) + FormatString("compstress1 ", "left", 12) + FormatString("micelle", "left", 10) + " stress/atom  NULL \n" + 

		FormatString("compute ", "left", 20) + FormatString("dunno1 ", "left", 12) + FormatString("micelle", "left", 10) + " reduce  sum  c_compstress1[1]  c_compstress1[2]  " + "c_compstress1[3]  c_compstress1[4]  c_compstress1[5]  c_compstress1[6] \n" + 
	
		FormatString("compute ", "left", 20) + FormatString("compstress2 ", "left", 12) + FormatString("notwater", "left", 10) + " stress/atom  NULL \n" + FormatString("compute ", "left", 20) + FormatString("dunno2 ", "left", 12)  + FormatString("notwater", "left", 10) + " reduce  sum  c_compstress2[1]  c_compstress2[2]  " + "c_compstress2[3]  c_compstress2[4]  c_compstress2[5]  c_compstress2[6] \n \n" +
		
		FormatString("thermo ", "left", 20)        + "1000 #$c \n" +
		FormatString("thermo_style ", "left", 20)  + "custom   step  density temp  press  pxx  lx  ly  lz  #v_sigmaxx1  v_sigmaxx2  v_netsigmaxx   \n" + 
		FormatString("thermo_modify ", "left", 20) + "flush    yes \n" + 
		FormatString("thermo_modify ", "left", 20) + "norm     yes \n\n");

	return ReturnValue;
}






// 
string TensionRelax (mysystem &CurrentSystem, int deltaT)
{
	int tdamp = deltaT*10;
	int pdamp = deltaT*10;
	
	
	
	string ReturnValue = "";
	
	
	if (CurrentSystem.runType == "Tension")
	{
		ReturnValue += ("\n\n####################################################################################################\n#Uniaxial Tensile Deformation\n####################################################################################################\n \n" +
		
		
		FormatString("fix ", "left", 20) + FormatString("ensemble ", "left", 12) + FormatString("all", "left", 10) + " npt       temp   $T  $T  " + itos(tdamp)  + "   " + CurrentSystem.direction[1] + "  0.0  0.0  " + itos(pdamp)  + "   " + CurrentSystem.direction[2] + "  0.0  0.0  " + itos(pdamp) + "  drag 2 \n" +
		
		FormatString("fix ", "left", 20) + FormatString("def4m ", "left", 12)  + FormatString("all", "left", 10) + " deform    1  " + CurrentSystem.direction[0] + "  erate  " + dtosPW(CurrentSystem.elongRate,9,11) + "  units   box  remap  x \n");
	}

	else
	{
		ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("ensemble ", "left", 10) + FormatString("all ", "left", 5) + " npt    temp   $T  $T  " + itos(tdamp) + "  iso   1.0  1.0  " + itos(pdamp) + "  drag 2 \n");;
	}
	
	
	ReturnValue += ( 
		FormatString("fix ", "left", 20) + FormatString("bals ", "left", 12) + FormatString("all", "left", 10) + " balance   10   1.01  shift  " + CurrentSystem.direction[0] + "  10  1.00 \n \n" + 
	
		

		FormatString("variable ", "left", 20) + FormatString("sigxx1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[1]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigyy1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[2]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigzz1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[3]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigxy1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[4]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigxz1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[5]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigyz1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[6]/vol \n\n" + 
	
		FormatString("variable ", "left", 20) + FormatString("sigxx2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[1]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigyy2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[2]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigzz2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[3]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigxy2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[4]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigxz2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[5]/vol \n" + 
		FormatString("variable ", "left", 20) + FormatString("sigyz2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[6]/vol \n\n" +
	
	
		FormatString("variable ", "left", 20) + FormatString("temp1 ", "left", 12) + FormatString("equal", "left", 10)  + " l" + CurrentSystem.direction[0] + " \n" + 

		FormatString("variable ", "left", 20) + "L" + CurrentSystem.direction[0] + FormatString("0 ", "left", 10)     + FormatString("equal", "left", 10)  + " ${temp1} \n" + 

		FormatString("variable ", "left", 20) + FormatString("strain1 ", "left", 12) + FormatString("equal", "left", 10)  + " (l" + CurrentSystem.direction[0] + "-${L" + CurrentSystem.direction[0] + "0})/${L" + CurrentSystem.direction[0] + "0} \n" + 

		FormatString("variable ", "left", 20) + FormatString("temp2 ", "left", 12) + FormatString("equal", "left", 10)  + " l" + CurrentSystem.direction[1] + " \n" + 

		FormatString("variable ", "left", 20) + "L" + CurrentSystem.direction[1] + FormatString("0 ", "left", 10) + FormatString("equal", "left", 10)  + " ${temp2} \n" + 

		FormatString("variable ", "left", 20) + FormatString("strain2 ", "left", 12) + FormatString("equal", "left", 10)  + " (${L" + CurrentSystem.direction[1] + "0}-l" + CurrentSystem.direction[1] + ")/${L" + CurrentSystem.direction[1] + "0} \n" + 

		FormatString("variable ", "left", 20) + FormatString("temp3 ", "left", 12) + FormatString("equal", "left", 10)  + " l" + CurrentSystem.direction[2] + " \n" + 

		FormatString("variable ", "left", 20) + "L" + CurrentSystem.direction[2] + FormatString("0 ", "left", 10) + FormatString("equal", "left", 10)  + " ${temp3} \n" + 

		FormatString("variable ", "left", 20) + FormatString("strain3 ", "left", 12) + FormatString("equal", "left", 10)  + " (${L" + CurrentSystem.direction[2] + "0}-l" + CurrentSystem.direction[2] + ")/${L" + CurrentSystem.direction[2] + "0} \n \n" +


		FormatString("variable ", "left", 20) + FormatString("unpxx1 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigxx1*0.101325  \n" + 
		FormatString("variable ", "left", 20) + FormatString("unpyy1 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigyy1*0.101325  \n" + 
		FormatString("variable ", "left", 20) + FormatString("unpzz1 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigzz1*0.101325  \n" + 
		FormatString("variable ", "left", 20) + FormatString("stress1 ", "left", 12)  + FormatString("equal", "left", 10)  + " v_unp" + CurrentSystem.direction[0] + CurrentSystem.direction[0] + "1-0.5*(v_unp" + CurrentSystem.direction[1] + CurrentSystem.direction[1] + "1+v_unp" + CurrentSystem.direction[2] + CurrentSystem.direction[2] + "1)  \n \n" +
		

		FormatString("variable ", "left", 20) + FormatString("unpxx2 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigxx2*0.101325  \n" + 
		FormatString("variable ", "left", 20) + FormatString("unpyy2 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigyy2*0.101325  \n" + 
		FormatString("variable ", "left", 20) + FormatString("unpzz2 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigzz2*0.101325  \n" + 
		FormatString("variable ", "left", 20) + FormatString("stress2 ", "left", 12)  + FormatString("equal", "left", 10)  + " v_unp" + CurrentSystem.direction[0] + CurrentSystem.direction[0] + "2-0.5*(v_unp" + CurrentSystem.direction[1] + CurrentSystem.direction[1] + "2+v_unp" + CurrentSystem.direction[2] + CurrentSystem.direction[2] + "2)   \n \n" +
		
		
		FormatString("variable ", "left", 20) + FormatString("unpxx3 ", "left", 12)  + FormatString("equal", "left", 10)  + " -1*v_pxx*0.101325  \n" +
		FormatString("variable ", "left", 20) + FormatString("unpyy3 ", "left", 12)  + FormatString("equal", "left", 10)  + " -1*v_pyy*0.101325  \n" +
		FormatString("variable ", "left", 20) + FormatString("unpzz3 ", "left", 12)  + FormatString("equal", "left", 10)  + " -1*v_pzz*0.101325  \n" +
		FormatString("variable ", "left", 20) + FormatString("stress3 ", "left", 12) + FormatString("equal", "left", 10)  + " v_unp" + CurrentSystem.direction[0] + CurrentSystem.direction[0] + "3-0.5*(v_unp" + CurrentSystem.direction[1] + CurrentSystem.direction[1] + "3+v_unp" + CurrentSystem.direction[2] + CurrentSystem.direction[2] + "3)   \n \n" +
	
	
	
		FormatString("fix ", "left", 20) + FormatString("avtym1 ", "left", 12)    + FormatString("micelle", "left", 15) + " ave/time  1  "  + itos(CurrentSystem.logDump) + "  "  + itos(CurrentSystem.logDump) + "  v_unpxx1  v_unpyy1  v_unpzz1  v_stress1  #file MicelleAve" + CurrentSystem.fileNameEnd + ".dat       \n" +
	
		FormatString("fix ", "left", 20) + FormatString("avtym2 ", "left", 12)    + FormatString("notwater", "left", 15) + " ave/time  1  "  + itos(CurrentSystem.logDump) + "  " + itos(CurrentSystem.logDump) + "  v_unpxx2  v_unpyy2  v_unpzz2  v_stress2  #file NotWaterAve" + CurrentSystem.fileNameEnd + ".dat       \n" +
	
		FormatString("fix ", "left", 20) + FormatString("avtym3 ", "left", 12)    + FormatString("all", "left", 15) + " ave/time  1  "  + itos(CurrentSystem.logDump) + "  "  + itos(CurrentSystem.logDump) + "  v_unpxx3  v_unpyy3  v_unpzz3  v_stress3  #file SystemAve" + CurrentSystem.fileNameEnd + ".dat  \n \n" +
	
	
		FormatString("variable ", "left", 20) + FormatString("tempTime ", "left", 12)   + FormatString("equal", "left", 10)  + " step \n" + 
		FormatString("variable ", "left", 20) + FormatString("initialTime ", "left", 12)+ FormatString("equal", "left", 10)  + " ${tempTime} \n" + 
		FormatString("variable ", "left", 20) + FormatString("time ", "left", 12)       + FormatString("equal", "left", 10)  + " step-${initialTime} \n" + 
		FormatString("variable ", "left", 20) + FormatString("avePxx1 ", "left", 12)    + FormatString("equal", "left", 10)  + " f_avtym1[1] \n" + 
		FormatString("variable ", "left", 20) + FormatString("avePyy1 ", "left", 12)    + FormatString("equal", "left", 10)  + " f_avtym1[2] \n" + 
		FormatString("variable ", "left", 20) + FormatString("avePzz1 ", "left", 12)    + FormatString("equal", "left", 10)  + " f_avtym1[3] \n" + 
		FormatString("variable ", "left", 20) + FormatString("aveStress1 ", "left", 12) + FormatString("equal", "left", 10)  + " f_avtym1[4] \n" + 
		FormatString("variable ", "left", 20) + FormatString("visc1 ", "left", 12)      + FormatString("equal", "left", 10)  + " v_aveStress1*v_viscMultiplier  \n \n" +
	
	
		FormatString("variable ", "left", 20) + FormatString("avePxx2 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym2[1] \n" + 
		FormatString("variable ", "left", 20) + FormatString("avePyy2 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym2[2] \n" + 
		FormatString("variable ", "left", 20) + FormatString("avePzz2 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym2[3] \n" + 
		FormatString("variable ", "left", 20) + FormatString("aveStress2 ", "left", 12)+ FormatString("equal", "left", 10)  + " f_avtym2[4] \n" + 
		FormatString("variable ", "left", 20) + FormatString("visc2 ", "left", 12)     + FormatString("equal", "left", 10)  + " v_aveStress2*v_viscMultiplier \n \n" +

	
		FormatString("variable ", "left", 20) + FormatString("avePxx3 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym3[1] \n" + 
		FormatString("variable ", "left", 20) + FormatString("avePyy3 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym3[2] \n" + 
		FormatString("variable ", "left", 20) + FormatString("avePzz3 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym3[3] \n" + 
		FormatString("variable ", "left", 20) + FormatString("aveStress3 ", "left", 12)+ FormatString("equal", "left", 10)  + " f_avtym3[4] \n" + 
		FormatString("variable ", "left", 20) + FormatString("visc3 ", "left", 12)     + FormatString("equal", "left", 10)  + " v_aveStress3*v_viscMultiplier \n \n" +
	
	
		FormatString("fix ", "left", 20) + FormatString("def1 ", "left", 12) + FormatString("micelle", "left", 15) + " print  "  + itos(CurrentSystem.logDump) + " \"${time}   ${strain1}   ${strain2}   ${strain3}" + "   ${avePxx1}   ${avePyy1}   ${avePzz1}   ${aveStress1}   ${visc1} \" file MicelleFix" + CurrentSystem.fileNameEnd + ".dat screen no \n" + 
	
		FormatString("fix ", "left", 20) + FormatString("def2 ", "left", 12) + FormatString("notwater", "left", 15) + " print  "  + itos(CurrentSystem.logDump) + " \"${time}   ${strain1}   ${strain2}   ${strain3}" + "   ${avePxx2}   ${avePyy2}   ${avePzz2}   ${aveStress2}   ${visc2} \" file NotWaterFix" + CurrentSystem.fileNameEnd + ".dat screen no \n" + 
	
		FormatString("fix ", "left", 20) + FormatString("def3 ", "left", 12) + FormatString("all", "left", 15) + " print  "  + itos(CurrentSystem.logDump) + " \"${time}   ${strain1}   ${strain2}   ${strain3}"  + "   ${avePxx3}   ${avePyy3}   ${avePzz3}   ${aveStress3}   ${visc3} \" file SystemFix" + CurrentSystem.fileNameEnd + ".dat screen no \n \n" +
	
	
		FormatString("print ", "left", 20) + "\"Initial Length, Lx0: ${Lx0}\" \n \n" +
		FormatString("thermo ", "left", 20) + itos(CurrentSystem.logDump) + " \n" + 
		FormatString("thermo_style " , "left", 20) + "custom  step  v_strain1 temp press  lx  ly  lz  pxx  pyy  pzz pxy  pxz  pyz  ke pe \n" + 
		FormatString("thermo_modify ", "left", 20) + "flush   yes \n" + 
		FormatString("thermo_modify ", "left", 20) + "norm    yes \n \n" +
	
		
		FormatString("dump ", "left", 20) + FormatString("m1 ", "left", 10) + FormatString("micelle ", "left", 15)  + FormatString(" custom", "left", 10)   + FormatString(itos(CurrentSystem.trjDumpShort),"left",10) + FormatString("MicelleWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   x   y   z   vx   vy   vz \n" + 
	
		FormatString("dump ", "left", 20) + FormatString("m2 ", "left", 10) + FormatString("micelle ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpShort),"left",10) + FormatString("MicelleUnWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   xu   yu   zu   c_atomprop1   c_atomprop2   c_atomprop4 \n" + 
	
	
		FormatString("dump ", "left", 20) + FormatString("w1 ", "left", 10) + FormatString("notmicelle ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpLong),"left",10) + FormatString("WaterIonWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   x   y   z   vx   vy   vz \n" + 
		
		FormatString("dump ", "left", 20) + FormatString("s1 ", "left", 10) + FormatString("all ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpLong),"left",10) + FormatString("SystemWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   x   y   z   vx   vy   vz \n" + 
	

		FormatString("dump ", "left", 20) + FormatString("w2 ", "left", 10) + FormatString("notmicelle ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpLong),"left",10)  + FormatString("WaterIonUnWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   xu   yu   zu \n \n" +
		
		FormatString("dump ", "left", 20) + FormatString("s2 ", "left", 10) + FormatString("all ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpLong),"left",10)  + FormatString("SystemUnWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   xu   yu   zu \n \n" +
	
	
	
		FormatString("restart ", "left", 20)  + itos(CurrentSystem.restartFreq)   + "   Restart" + CurrentSystem.fileNameEnd + "-*.equil \n" + 
	
		FormatString("run_style ", "left", 20) + "verlet \n" + 
		FormatString("run ", "left", 20) + itos(CurrentSystem.runStep) + " \n \n");


	if (CurrentSystem.runType == "Tension")
	{
		ReturnValue += ( FormatString("unfix ", "left", 20) +"def4m \n");
	}


	ReturnValue += ( 
		FormatString("undump ", "left", 20)+"m1 \n" + 
		FormatString("undump ", "left", 20)+"m2 \n" + 
		FormatString("undump ", "left", 20)+"w1 \n" + 
		FormatString("undump ", "left", 20)+"w2 \n" + 
		FormatString("undump ", "left", 20)+"s1 \n" + 
		FormatString("undump ", "left", 20)+"s2 \n" + 
		FormatString("unfix ", "left", 20) +"avtym1 \n" + 
		FormatString("unfix ", "left", 20) +"avtym2 \n" + 
		FormatString("unfix ", "left", 20) +"avtym3 \n" + 
		FormatString("unfix ", "left", 20) +"def1 \n" + 
		FormatString("unfix ", "left", 20) +"def2 \n" + 
		FormatString("unfix ", "left", 20) +"def3 \n" + 
		FormatString("unfix ", "left", 20) +"ensemble \n" +
		FormatString("write_data ", "left", 20)  + "WrittenData" + CurrentSystem.fileNameEnd + "-*.dat \n \n\n \n");

	return ReturnValue;
}



//
void AssignConfig (mysystem &CurrentSystem)
{
	CurrentSystem.counterMain    =   CurrentSystem.mainChain;  
	CurrentSystem.counterSolute  =   CurrentSystem.soluteChain;            
	CurrentSystem.mainChaB       =   CurrentSystem.mainChain   *  CurrentSystem.mainBead;         
	CurrentSystem.soluteChaB     =   CurrentSystem.soluteChain *  CurrentSystem.soluteBead;
	CurrentSystem.chaB   =   CurrentSystem.mainChaB   +  CurrentSystem.soluteChaB;            
	CurrentSystem.size   =   CurrentSystem.chaB + CurrentSystem.counterSolute +  CurrentSystem.counterMain  +  CurrentSystem.WA  +  CurrentSystem.AFW;    
	
	
	CurrentSystem.cumm0 = CurrentSystem.WA;    
	CurrentSystem.cumm1 = CurrentSystem.cumm0+CurrentSystem.AFW;    
	CurrentSystem.cumm2 = CurrentSystem.cumm1+CurrentSystem.mainChaB;    
	CurrentSystem.cumm3 = CurrentSystem.cumm2+CurrentSystem.counterMain;
	CurrentSystem.cumm4 = CurrentSystem.cumm3+CurrentSystem.soluteChaB;                                 
	CurrentSystem.cumm5 = CurrentSystem.cumm4+CurrentSystem.counterSolute;
	

	
	if (CurrentSystem.soluteTop == "NaCl")
	{
		CurrentSystem.solute_b_Num = CurrentSystem.solute_a_Num  = 0;
	}
	else if (CurrentSystem.soluteTop == "NaSal")
	{
		CurrentSystem.solute_b_Num = CurrentSystem.soluteBead * CurrentSystem.soluteChain;   
		CurrentSystem.solute_a_Num = (CurrentSystem.soluteBead + 1) * CurrentSystem.soluteChain;
	}
	else
	{
		CurrentSystem.solute_b_Num   =  (CurrentSystem.soluteBead-1) * CurrentSystem.soluteChain;
		CurrentSystem.solute_a_Num   =  (CurrentSystem.soluteBead-2) * CurrentSystem.soluteChain;
	}
	
	CurrentSystem.main_b_Num  =  (CurrentSystem.mainBead - 1) * CurrentSystem.mainChain;
	CurrentSystem.main_a_Num  =  (CurrentSystem.mainBead - 2) * CurrentSystem.mainChain;
	CurrentSystem.a_Num  =   CurrentSystem.main_a_Num    + CurrentSystem.solute_a_Num;
	CurrentSystem.b_Num  =   CurrentSystem.main_b_Num    + CurrentSystem.solute_b_Num;
	
	std::cout << "Size = "  << CurrentSystem.size  << "\n";
	
	CurrentSystem.massMartiniBead   =   72;
}



//
void GetCoordConfig (mysystem &CurrentSystem)
{
	if (CurrentSystem.runType == "Equilibrium")
	{
		singleTrj UnfixTrj;
		singleTrj FixTrj;
		
		UnfixTrj.BeadInfo.resize(CurrentSystem.size);
		
		
		string ReadFileName1  =  CurrentSystem.readFolderName +  "MainNPT.gro";
		ifstream ReadFile1 (ReadFileName1.c_str());
		
		
		string line;
		int count = 0,     countLineInFile = 0;
		while (std::getline(ReadFile1, line))
		{
			float  x1, x2, x3, x4, x5, x6;
			int    i1;
			string s1, s2;
			
			std::istringstream iss(line);
			
			countLineInFile++;
			if(countLineInFile > 2 and countLineInFile <= 2+CurrentSystem.size)
			{
				std::string TruncLine = line.substr(20,24);
				std::istringstream iss(TruncLine);
				
				if(iss>>x1>>x2>>x3)
				{
					UnfixTrj.BeadInfo[count].x = x1*10;
					UnfixTrj.BeadInfo[count].y = x2*10;
					UnfixTrj.BeadInfo[count].z = x3*10;
					
					count++;
				}
			}
			
			else if (countLineInFile > 2+CurrentSystem.size)
			{
				std::istringstream iss(line);
				
				if(iss>>x1>>x2>>x3)
				{
					UnfixTrj.xBoxLength = x1*10;
					UnfixTrj.yBoxLength = x2*10;
					UnfixTrj.zBoxLength = x3*10;
					
					FixTrj.xBoxLength = x1*10;
					FixTrj.yBoxLength = x2*10;
					FixTrj.zBoxLength = x3*10;
				}
			}
			
		}
		ReadFile1.close();
		
		UnfixTrj.xhi = 0.5*UnfixTrj.xBoxLength,     UnfixTrj.yhi = 0.5*UnfixTrj.yBoxLength,       UnfixTrj.zhi = 0.5*UnfixTrj.zBoxLength;
		UnfixTrj.xlo = -0.5*UnfixTrj.xBoxLength,    UnfixTrj.ylo = -0.5*UnfixTrj.yBoxLength,      UnfixTrj.zlo = -0.5*UnfixTrj.zBoxLength;
		
		FixTrj.xhi = 0.5*FixTrj.xBoxLength,     FixTrj.yhi = 0.5*FixTrj.yBoxLength,       FixTrj.zhi = 0.5*FixTrj.zBoxLength;
		FixTrj.xlo = -0.5*FixTrj.xBoxLength,    FixTrj.ylo = -0.5*FixTrj.yBoxLength,      FixTrj.zlo = -0.5*FixTrj.zBoxLength;
		
		
		
		FixTrj.BeadInfo.insert(FixTrj.BeadInfo.begin(), UnfixTrj.BeadInfo.begin()+CurrentSystem.chaB+CurrentSystem.counterMain+CurrentSystem.counterSolute,   UnfixTrj.BeadInfo.begin()+CurrentSystem.size);
		
		
		FixTrj.BeadInfo.insert(FixTrj.BeadInfo.end(),   UnfixTrj.BeadInfo.begin(),   UnfixTrj.BeadInfo.begin()+CurrentSystem.mainChaB);
		
		
		FixTrj.BeadInfo.insert(FixTrj.BeadInfo.end(),   UnfixTrj.BeadInfo.begin()+CurrentSystem.chaB,   UnfixTrj.BeadInfo.begin()+CurrentSystem.chaB+CurrentSystem.counterMain);
		
		
		FixTrj.BeadInfo.insert(FixTrj.BeadInfo.end(),   UnfixTrj.BeadInfo.begin()+CurrentSystem.mainChaB,   UnfixTrj.BeadInfo.begin()+CurrentSystem.chaB);
		
		
		FixTrj.BeadInfo.insert(FixTrj.BeadInfo.end(),   UnfixTrj.BeadInfo.begin()+CurrentSystem.chaB+CurrentSystem.counterMain,   UnfixTrj.BeadInfo.begin()+CurrentSystem.chaB+CurrentSystem.counterMain+CurrentSystem.counterSolute);
		
		
		CurrentSystem.MultiTrj.push_back(FixTrj);
		
		UnfixTrj.BeadInfo.clear();
		FixTrj.BeadInfo.clear();
	}
	
	
	else if (CurrentSystem.runType == "Relax")
	{
		///media/OkwuchisHDD/DifferentStructures/4/Load_x/MicelleWrapLoad-Rate10-Spn4-En1.lammpstrj
		string ReadFileName1  =  CurrentSystem.readFolderName +  "MicelleWrap" + CurrentSystem.readFileNameEnd + ".lammpstrj";
		ifstream ReadFile1 (ReadFileName1.c_str());
		
		string ReadFileName2  =  CurrentSystem.readFolderName +  "WaterIonWrap" + CurrentSystem.readFileNameEnd + ".lammpstrj";
		ifstream ReadFile2 (ReadFileName2.c_str());
		
		
		string line1,  line2,  boxLengthInfo = "";
		
		string file1TimeStep = "", file2TimeStep = "";
		
		
		int HeaderLines = 9;
		//int skipDumpFile1 = 25,     skipDumpFile2 = 1;
		int skipDumpFile1 = 1,     skipDumpFile2 = 1;
		int index1 = 0,             index2 = 0;
		
		int beadsFile1 = CurrentSystem.chaB;
		int beadsFile2 = CurrentSystem.WAf + CurrentSystem.mainChain + CurrentSystem.soluteChain;
		
		int linesFile1 = beadsFile1 + HeaderLines;
		int linesFile2 = beadsFile2 + HeaderLines;
		
		int countLineBead1 = 0,   countLineBead2 = 0;
		int countLineFile1 = 0,   countLineFile2 = 0;
		
		CurrentSystem.MultiTrj.resize(1);
		CurrentSystem.MultiTrj[0].BeadInfo.resize(CurrentSystem.size);
		
		int tempTime1 = 1,   tempTime2 = 1;
		
		
		cout <<"\n  Index2: "<<tempTime2<<";  ";
		while ( std::getline(ReadFile2, line2) )
		{
			float  x1, x2, x3, x4, x5, x6;
			int    i1, i2;
			string s1, s2;
			
			countLineFile2++;
			
			//index2 = floor((countLineFile2/linesFile2)/skipDumpFile2);
			
			
			
			if ( (countLineFile2/linesFile2)%skipDumpFile2 == 0 and countLineFile2%linesFile2 == 2 )
			{
				//CurrentSystem.MultiTrj[index2].timeDump = line2;
				//cout<<index2<<"|  ";
				//cout<<tempTime2<<"|  ";
				
				CurrentSystem.MultiTrj[tempTime2-1].timeDump = line2;
				file2TimeStep += (line2 + "     ");
			}
			
			
			if ( (countLineFile2/linesFile2)%skipDumpFile2 == 0 and (countLineFile2-1)%linesFile2 >= HeaderLines and (countLineFile2-1)%linesFile2 < linesFile2)
			{
				std::istringstream iss2(line2);
				if (iss2>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6)
				{
					//CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].x = x1;
					//CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].y = x2;
					//CurrentSystem.MultiTrj[index2].BeadInfo[i1-1].z = x3;
					
					//cout<<i1<<";  ";
					
					CurrentSystem.MultiTrj[tempTime2-1].BeadInfo[i1-1].x = x1;
					CurrentSystem.MultiTrj[tempTime2-1].BeadInfo[i1-1].y = x2;
					CurrentSystem.MultiTrj[tempTime2-1].BeadInfo[i1-1].z = x3;
					
					
					if (countLineFile2%linesFile2 == 0)
					{
						//CurrentSystem.MultiTrj.resize(index2+1);
						//CurrentSystem.MultiTrj[index2].BeadInfo.resize(CurrentSystem.size);
						//index2++;
						
						//cout<<index2<<";  ";
						
						
						CurrentSystem.MultiTrj.resize(tempTime2+1);
						CurrentSystem.MultiTrj[tempTime2].BeadInfo.resize(CurrentSystem.size);
						tempTime2++;
						
						cout<<tempTime2<<";  ";
					}
				}
			}
		}
		ReadFile2.close();
		
		
		
		
		cout<<"\n  Index1: "<<tempTime1<<";  ";
		string timeStep = "";
		while (std::getline(ReadFile1, line1) )
		{
			float  x1, x2, x3, x4, x5, x6;
			int    i1, i2;
			string s1, s2;
			
			
			countLineFile1++;
			
			index1 = floor((countLineFile1/linesFile1)/skipDumpFile1);
			
			
			
			
		
			if ( (countLineFile1/linesFile1)%skipDumpFile1 == 0)
			//if ( timeStep == CurrentSystem.MultiTrj[tempTime1-1].timeDump)
			{
				if (countLineFile1%linesFile1 == 2 )
				{
					timeStep = line1;
					
					file1TimeStep += (line1 + "     ");
				}
				
				
				if(countLineFile1%linesFile1 >= 6 and countLineFile1%linesFile1 <= 8)
				{
					boxLengthInfo += line1;
					
					std::istringstream iss3(boxLengthInfo);
					
					if (iss3>>x1>>x2>>x3>>x4>>x5>>x6)
					{
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
						
						CurrentSystem.MultiTrj[tempTime1-1].xlo = x1;
						CurrentSystem.MultiTrj[tempTime1-1].xhi = x2;
						CurrentSystem.MultiTrj[tempTime1-1].xBoxLength = CurrentSystem.MultiTrj[tempTime1-1].xhi - CurrentSystem.MultiTrj[tempTime1-1].xlo;
						
						CurrentSystem.MultiTrj[tempTime1-1].ylo = x3;
						CurrentSystem.MultiTrj[tempTime1-1].yhi = x4;
						CurrentSystem.MultiTrj[tempTime1-1].yBoxLength = CurrentSystem.MultiTrj[tempTime1-1].yhi - CurrentSystem.MultiTrj[tempTime1-1].ylo;
						
						CurrentSystem.MultiTrj[tempTime1-1].zlo = x5;
						CurrentSystem.MultiTrj[tempTime1-1].zhi = x6;
						CurrentSystem.MultiTrj[tempTime1-1].zBoxLength = CurrentSystem.MultiTrj[tempTime1-1].zhi - CurrentSystem.MultiTrj[tempTime1-1].zlo;
						
						boxLengthInfo = "";
					}
				}
				
				
				if ((countLineFile1-1)%linesFile1 >= HeaderLines and (countLineFile1-1)%linesFile1 < linesFile1)
				{
					std::istringstream iss1(line1);
					
					if (iss1>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6)
					{
						//CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].x = x1;
						//CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].y = x2;
						//CurrentSystem.MultiTrj[index1].BeadInfo[i1-1].z = x3;
						
						CurrentSystem.MultiTrj[tempTime1-1].BeadInfo[i1-1].x = x1;
						CurrentSystem.MultiTrj[tempTime1-1].BeadInfo[i1-1].y = x2;
						CurrentSystem.MultiTrj[tempTime1-1].BeadInfo[i1-1].z = x3;
						
						
						if (countLineFile1%linesFile1 == 0)
						{
							CurrentSystem.MultiTrj.resize(tempTime1+1);
							CurrentSystem.MultiTrj[tempTime1].BeadInfo.resize(CurrentSystem.size);
							
							tempTime1++;
							
							//cout<<tempTime1<<";  ";
							
							cout<<"\n  Compare: "<<tempTime1<<";  "<<timeStep<<";  "<<CurrentSystem.MultiTrj[tempTime1-1].timeDump<<";  "<<"\n";
						}
					}
				}
				
			}
			
		}
		ReadFile1.close();
		
		string WriteFileName1  =  CurrentSystem.writeFolderName +  "FileHeader" + CurrentSystem.readFileNameEnd + ".dat";
		ofstream WriteFile1 (WriteFileName1.c_str());
		
		WriteFile1<<file1TimeStep <<"\n";
		WriteFile1<<file2TimeStep <<"\n";
		WriteFile1.close();
		
		cout<<"\n";
	}
}



//
void WriteBeadInConfig (mysystem &CurrentSystem, int timeDump)
{
	//Naming all Bead Information
	int countForChain = 0;
	int countBeadType = 1;
	int countBeadId = 0;
	
	CurrentSystem.MultiTrj[timeDump].beadsInConfig += ("Atoms  #AtomID   ChainID   BeadType   Charge   x   y   z\n \n");
	for (int a = 0;  a < CurrentSystem.cumm0;   a++)  
	{
		CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(++countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
		countBeadId++;
	}
	
	
	countBeadType++;
	for (int a = CurrentSystem.cumm0;  a < CurrentSystem.cumm1;   a++)  
	{
		CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(++countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
		countBeadId++;
	}
	
	
	countBeadType++;
	for (int a = 0;  a < CurrentSystem.mainChain;   a++)  
	{
		++countForChain;
		for (int b = 0;  b < CurrentSystem.mainBead;   b++)  
		{
			if (CurrentSystem.mainTop[2] == 'A' and b == 0)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if (CurrentSystem.mainTop[2] == 'N' and b == 0)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(-1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if (CurrentSystem.mainTop != "CTA" and b !=0)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType+1,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if (CurrentSystem.mainTop == "CTA" and b > 0 and b <= 2)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(9,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if (CurrentSystem.mainTop == "CTA" and b > 2 and b < CurrentSystem.mainBead)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType+1,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			countBeadId++;
		}
	}
	
	
	countBeadType+=2;
	for (int a = CurrentSystem.cumm2;  a < CurrentSystem.cumm3;   a++)  
	{
		if (CurrentSystem.mainTop[2] == 'N')
		{
			CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(++countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
		}
		
		else if (CurrentSystem.mainTop[2] == 'A')
		{
			CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(++countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(-1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
		}
		countBeadId++;
	}
	
	
	countBeadType++;
	for (int a = 0;  a < CurrentSystem.soluteChain;   a++)  
	{
		++countForChain;
		for (int b = 0;  b < CurrentSystem.soluteBead;   b++)  
		{
			if (CurrentSystem.soluteTop[2] == 'a' and b == 0)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if ( (CurrentSystem.soluteTop[2] == 'n' or CurrentSystem.soluteTop == "NaSal") and b == 0)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(-1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if (CurrentSystem.soluteTop != "Cta" and b !=0)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType+1,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if (CurrentSystem.soluteTop == "Cta" and b > 0 and b <= 2)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(9,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			
			else if (CurrentSystem.soluteTop == "Cta" and b > 2 and b < CurrentSystem.soluteBead)
			{
				CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(countForChain,0,12) + itosPW(countBeadType+1,0,7) + itosPW(0,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
			}
			countBeadId++;
		}
	}
	
	
	countBeadType+=2;
	for (int a = CurrentSystem.cumm4;  a < CurrentSystem.cumm5;   a++)  
	{
		if (CurrentSystem.soluteTop[2] == 'n' or CurrentSystem.soluteTop == "NaSal")
		{
			CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(++countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
		}
		
		else if (CurrentSystem.soluteTop[2] == 'a')
		{
			CurrentSystem.MultiTrj[timeDump].beadsInConfig += (itosPW(countBeadId+1, 0,12) + itosPW(++countForChain,0,12) + itosPW(countBeadType,0,7) + itosPW(-1,0,5) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].x,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].y,3,15) + ftosPW(CurrentSystem.MultiTrj[timeDump].BeadInfo[countBeadId].z,3,15)  + "\n");
		}
		countBeadId++;
	}
	
	if (CurrentSystem.soluteTop == "Cta" or CurrentSystem.mainTop == "CTA")
	{
		CurrentSystem.totalBeadType   =  countBeadType+1;
	}
	else
	{
		CurrentSystem.totalBeadType   =  countBeadType;
	}
}



//
void WriteBondInConfig (mysystem &CurrentSystem, int timeDump)
{
	// ********************** Naming Bonds *******************
	CurrentSystem.MultiTrj[timeDump].bondsInConfig += ("\n \nBonds  #BondID  BondType Bond_i  Bond_j\n \n");
	
	// ***** For MainPhase
	int forBondij  =  0;
	int forBondType = 1;
	
	for (int ai = 0;  ai < CurrentSystem.mainChain;  ai++)
	{
		for (int aj = 0;  aj+1 < CurrentSystem.mainBead;  aj++)
		{
			CurrentSystem.MultiTrj[timeDump].bondsInConfig += (itosPW(++forBondij,0,12) + itosPW(forBondType,0,5) + itosPW(CurrentSystem.cumm1+CurrentSystem.mainBead*ai+aj+1, 0, 12) + itosPW(CurrentSystem.cumm1+CurrentSystem.mainBead*ai+aj+2, 0, 12) + "\n");
		}
	}
	
	// ***** For Solute
	if (CurrentSystem.soluteTop != "NaSal")
	{
		for (int ai = 0;  ai < CurrentSystem.soluteChain;  ai++)
		{
			for (int aj = 0;  aj+1 < CurrentSystem.soluteBead;  aj++)
			{
				CurrentSystem.MultiTrj[timeDump].bondsInConfig += (itosPW(++forBondij,0,12) + itosPW(forBondType,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+aj+1, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+aj+2, 0, 12) + "\n");
			}
		}
	}
	
	else
	{
		for (int ai = 0;  ai < CurrentSystem.soluteChain;  ai++)
		{
			CurrentSystem.MultiTrj[timeDump].bondsInConfig += (itosPW(++forBondij,0,12) + itosPW(forBondType,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+1, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + "\n");
			
			CurrentSystem.MultiTrj[timeDump].bondsInConfig += (itosPW(++forBondij,0,12) + itosPW(forBondType+1,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+3, 0, 12) + "\n");
			
			CurrentSystem.MultiTrj[timeDump].bondsInConfig += (itosPW(++forBondij,0,12) + itosPW(forBondType+1,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+3, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+4, 0, 12) + "\n");
			
			CurrentSystem.MultiTrj[timeDump].bondsInConfig += (itosPW(++forBondij,0,12) + itosPW(forBondType+1,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+4, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + "\n");
		}
		
		forBondType++;
	}
	
	CurrentSystem.totalBondType   =  forBondType;
}



void WriteAngleInConfig (mysystem &CurrentSystem, int timeDump)
{
	// ********************** Naming Angles *******************
	CurrentSystem.MultiTrj[timeDump].anglesInConfig += ("\n \nAngles #AngleID   AngleType   Angle_i   Angle_j   Angle_k\n \n");
	// ***** For MainPhase
	int forAngleijk   =   0;
	int forAngleType  =   1;
	for (int ai = 0;  ai < CurrentSystem.mainChain;  ai++)
	{
		for (int aj = 0;  aj+2 < CurrentSystem.mainBead;  aj++)
		{
			CurrentSystem.MultiTrj[timeDump].anglesInConfig += (itosPW(++forAngleijk,0,12) + itosPW(forAngleType,0,5) + itosPW(CurrentSystem.cumm1+CurrentSystem.mainBead*ai+aj+1, 0, 12) + itosPW(CurrentSystem.cumm1+CurrentSystem.mainBead*ai+aj+2, 0, 12) + itosPW(CurrentSystem.cumm1+CurrentSystem.mainBead*ai+aj+3, 0, 12) + "\n");
		}
	}
	
	// For Solute
	if (CurrentSystem.soluteTop != "NaSal")
	{
		for (int ai = 0;  ai < CurrentSystem.soluteChain;  ai++)
		{
			for (int aj = 0;  aj+2 < CurrentSystem.soluteBead;  aj++)
			{
				CurrentSystem.MultiTrj[timeDump].anglesInConfig += (itosPW(++forAngleijk,0,12) + itosPW(forAngleType,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+aj+1, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+aj+2, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+aj+3, 0, 12) + "\n");
			}
		}
	}
	
	else
	{
		for (int ai = 0;  ai < CurrentSystem.soluteChain;  ai++)
		{
			CurrentSystem.MultiTrj[timeDump].anglesInConfig += (itosPW(++forAngleijk,0,12) + itosPW(forAngleType+1,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+1, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+3, 0, 12) + "\n");
			
			CurrentSystem.MultiTrj[timeDump].anglesInConfig += (itosPW(++forAngleijk,0,12) + itosPW(forAngleType+1,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+1, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+4, 0, 12) + "\n");
			
			
			CurrentSystem.MultiTrj[timeDump].anglesInConfig += (itosPW(++forAngleijk,0,12) + itosPW(forAngleType+2,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+3, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+4, 0, 12) + "\n");
			
			CurrentSystem.MultiTrj[timeDump].anglesInConfig += (itosPW(++forAngleijk,0,12) + itosPW(forAngleType+2,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+3, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+4, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + "\n");
			
			CurrentSystem.MultiTrj[timeDump].anglesInConfig += (itosPW(++forAngleijk,0,12) + itosPW(forAngleType+2,0,5) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+4, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+2, 0, 12) + itosPW(CurrentSystem.cumm3+CurrentSystem.soluteBead*ai+3, 0, 12) + "\n");
		}
		
		forAngleType+=2;
	}
	
	CurrentSystem.totalAngleType  =  forAngleType;
}



//
void TopConfig (mysystem &CurrentSystem, int timeDump)
{
	CurrentSystem.MultiTrj[timeDump].topInConfig += ("\n \n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( itos(CurrentSystem.size) + " atoms"  + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( itos(CurrentSystem.b_Num) + " bonds"  + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( itos(CurrentSystem.a_Num) + " angles"  + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( itos(CurrentSystem.totalBeadType) + " atom types"  + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( itos(CurrentSystem.totalBondType) + " bond types"  + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( itos(CurrentSystem.totalAngleType) + " angle types"  + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( ftos(CurrentSystem.MultiTrj[timeDump].xlo) + " " + ftos(CurrentSystem.MultiTrj[timeDump].xhi) + " " + "xlo xhi" + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( ftos(CurrentSystem.MultiTrj[timeDump].ylo) + " " + ftos(CurrentSystem.MultiTrj[timeDump].yhi) + " " + "ylo yhi" + "\n");
	CurrentSystem.MultiTrj[timeDump].topInConfig += ( ftos(CurrentSystem.MultiTrj[timeDump].zlo) + " " + ftos(CurrentSystem.MultiTrj[timeDump].zhi) + " " + "zlo zhi" + "\n \n");
	
	CurrentSystem.MultiTrj[timeDump].topInConfig += ("Masses \n \n");
	for(int a = 0;  a < CurrentSystem.totalBeadType;  a++)
	{
		CurrentSystem.MultiTrj[timeDump].topInConfig += ( itos(a+1) + "    " + itos(CurrentSystem.massMartiniBead) + "\n");
	}
	
	CurrentSystem.MultiTrj[timeDump].topInConfig += ("\n \n");
}






// //
// string TensionRelax (mysystem &CurrentSystem, int deltaT)
// {
// 	int tdamp = deltaT*10;
// 	int pdamp = deltaT*10;
	
	
	
// 	string ReturnValue = "";
	
	
// 	if (CurrentSystem.runType == "Tension")
// 	{
// 		ReturnValue +="\n";
// 		ReturnValue +="\n";
// 		ReturnValue +=( "#################################################################################################### \n");
// 		ReturnValue +=( "#Uniaxial Tensile Deformation \n");
// 		ReturnValue +=( "#################################################################################################### \n");
		
		
// 		ReturnValue +="\n";
// 		ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("ensemble ", "left", 12) + FormatString("all", "left", 10) + " npt       temp   $T  $T  " + itos(tdamp)  + "   " + CurrentSystem.direction[1] + "  0.0  0.0  " + itos(pdamp)  + "   " + CurrentSystem.direction[2] + "  0.0  0.0  " + itos(pdamp) + "  drag 2 \n");
		
// 		ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("def4m ", "left", 12)    + FormatString("all", "left", 10) + " deform    1  " + CurrentSystem.direction[0] + "  erate  " + dtosPW(CurrentSystem.elongRate,9,11) + "  units   box  remap  x \n");
// 	}
	
	
// 	ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("bals ", "left", 12)    + FormatString("all", "left", 10) + " balance   10   1.01  shift  " + CurrentSystem.direction[0] + "  10  1.00 \n");
// 	ReturnValue +="\n";
	
// 	ReturnValue += ( FormatString("compute ", "left", 20) + FormatString("compstress1 ", "left", 12) + FormatString("micelle", "left", 10) + " stress/atom  NULL \n");
// 	ReturnValue += ( FormatString("compute ", "left", 20) + FormatString("dunno1 ", "left", 12)      + FormatString("micelle", "left", 10) + " reduce  sum  c_compstress1[1]  c_compstress1[2]  " + "c_compstress1[3]  c_compstress1[4]  c_compstress1[5]  c_compstress1[6] \n");
	
// 	ReturnValue += ( FormatString("compute ", "left", 20) + FormatString("compstress2 ", "left", 12) + FormatString("notwater", "left", 10) + " stress/atom  NULL \n");
// 	ReturnValue += ( FormatString("compute ", "left", 20) + FormatString("dunno2 ", "left", 12)      + FormatString("notwater", "left", 10) + " reduce  sum  c_compstress2[1]  c_compstress2[2]  " + "c_compstress2[3]  c_compstress2[4]  c_compstress2[5]  c_compstress2[6] \n");
	
	
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigxx1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[1]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigyy1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[2]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigzz1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[3]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigxy1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[4]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigxz1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[5]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigyz1 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno1[6]/vol \n");
// 	ReturnValue +="\n";
	
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigxx2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[1]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigyy2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[2]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigzz2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[3]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigxy2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[4]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigxz2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[5]/vol \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("sigyz2 ", "left", 12)+ FormatString("equal", "left", 10)  + "  c_dunno2[6]/vol \n");
// 	ReturnValue +="\n";
	
	
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("temp1 ", "left", 12)   + FormatString("equal", "left", 10)  + " l" + CurrentSystem.direction[0] + " \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + "L" + CurrentSystem.direction[0] + FormatString("0 ", "left", 10)     + FormatString("equal", "left", 10)  + " ${temp1} \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("strain1 ", "left", 12) + FormatString("equal", "left", 10)  + " (l" + CurrentSystem.direction[0] + "-${L" + CurrentSystem.direction[0] + "0})/${L" + CurrentSystem.direction[0] + "0} \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("temp2 ", "left", 12)   + FormatString("equal", "left", 10)  + " l" + CurrentSystem.direction[1] + " \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + "L" + CurrentSystem.direction[1] + FormatString("0 ", "left", 10)     + FormatString("equal", "left", 10)  + " ${temp2} \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("strain2 ", "left", 12) + FormatString("equal", "left", 10)  + " (${L" + CurrentSystem.direction[1] + "0}-l" + CurrentSystem.direction[1] + ")/${L" + CurrentSystem.direction[1] + "0} \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("temp3 ", "left", 12)   + FormatString("equal", "left", 10)  + " l" + CurrentSystem.direction[2] + " \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + "L" + CurrentSystem.direction[2] + FormatString("0 ", "left", 10)     + FormatString("equal", "left", 10)  + " ${temp3} \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("strain3 ", "left", 12) + FormatString("equal", "left", 10)  + " (${L" + CurrentSystem.direction[2] + "0}-l" + CurrentSystem.direction[2] + ")/${L" + CurrentSystem.direction[2] + "0} \n");
	
	
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpxx1 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigxx1*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpyy1 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigyy1*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpzz1 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigzz1*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("stress1 ", "left", 12)  + FormatString("equal", "left", 10)  + " v_unp" + CurrentSystem.direction[0] + CurrentSystem.direction[0] + "1-0.5*(v_unp" + CurrentSystem.direction[1] + CurrentSystem.direction[1] + "1+v_unp" + CurrentSystem.direction[2] + CurrentSystem.direction[2] + "1)  \n");
	
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpxx2 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigxx2*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpyy2 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigyy2*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpzz2 ", "left", 12)   + FormatString("equal", "left", 10)  + " v_sigzz2*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("stress2 ", "left", 12)  + FormatString("equal", "left", 10)  + " v_unp" + CurrentSystem.direction[0] + CurrentSystem.direction[0] + "2-0.5*(v_unp" + CurrentSystem.direction[1] + CurrentSystem.direction[1] + "2+v_unp" + CurrentSystem.direction[2] + CurrentSystem.direction[2] + "2)  \n");
	
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpxx3 ", "left", 12)  + FormatString("equal", "left", 10)  + " -1*v_pxx*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpyy3 ", "left", 12)  + FormatString("equal", "left", 10)  + " -1*v_pyy*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("unpzz3 ", "left", 12)  + FormatString("equal", "left", 10)  + " -1*v_pzz*0.101325  \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("stress3 ", "left", 12) + FormatString("equal", "left", 10)  + " v_unp" + CurrentSystem.direction[0] + CurrentSystem.direction[0] + "3-0.5*(v_unp" + CurrentSystem.direction[1] + CurrentSystem.direction[1] + "3+v_unp" + CurrentSystem.direction[2] + CurrentSystem.direction[2] + "3)  \n");
	
	
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("avtym1 ", "left", 12)    + FormatString("micelle", "left", 15) + " ave/time  1  "  + itos(CurrentSystem.logDump) + "  "  + itos(CurrentSystem.logDump) + "  v_unpxx1  v_unpyy1  v_unpzz1  v_stress1  #file MicelleAve" + CurrentSystem.fileNameEnd + ".dat       \n");
	
// 	ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("avtym2 ", "left", 12)    + FormatString("notwater", "left", 15) + " ave/time  1  "  + itos(CurrentSystem.logDump) + "  " + itos(CurrentSystem.logDump) + "  v_unpxx2  v_unpyy2  v_unpzz2  v_stress2  #file NotWaterAve" + CurrentSystem.fileNameEnd + ".dat       \n");
	
// 	ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("avtym3 ", "left", 12)    + FormatString("all", "left", 15) + " ave/time  1  "  + itos(CurrentSystem.logDump) + "  "  + itos(CurrentSystem.logDump) + "  v_unpxx3  v_unpyy3  v_unpzz3  v_stress3  #file SystemAve" + CurrentSystem.fileNameEnd + ".dat       \n");
	
	
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("tempTime ", "left", 12)     + FormatString("equal", "left", 10)  + " step \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("initialTime ", "left", 12)  + FormatString("equal", "left", 10)  + " ${tempTime} \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("time ", "left", 12)         + FormatString("equal", "left", 10)  + " step-${initialTime} \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePxx1 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym1[1] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePyy1 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym1[2] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePzz1 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym1[3] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("aveStress1 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym1[4] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("visc1 ", "left", 12)        + FormatString("equal", "left", 10)  + " v_aveStress1*v_viscMultiplier \n");
// 	ReturnValue +="\n";
	
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePxx2 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym2[1] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePyy2 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym2[2] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePzz2 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym2[3] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("aveStress2 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym2[4] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("visc2 ", "left", 12)        + FormatString("equal", "left", 10)  + " v_aveStress2*v_viscMultiplier \n");
// 	ReturnValue +="\n";
	
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePxx3 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym3[1] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePyy3 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym3[2] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("avePzz3 ", "left", 12)      + FormatString("equal", "left", 10)  + " f_avtym3[3] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("aveStress3 ", "left", 12)   + FormatString("equal", "left", 10)  + " f_avtym3[4] \n");
// 	ReturnValue += ( FormatString("variable ", "left", 20) + FormatString("visc3 ", "left", 12)        + FormatString("equal", "left", 10)  + " v_aveStress3*v_viscMultiplier \n");
// 	ReturnValue +="\n";
	
	
// 	ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("def1 ", "left", 12) + FormatString("micelle", "left", 15) + " print  "  + itos(CurrentSystem.logDump) + " \"${time}   ${strain1}   ${strain2}   ${strain3}" + "   ${avePxx1}   ${avePyy1}   ${avePzz1}   ${aveStress1}   ${visc1} \" file MicelleFix" + CurrentSystem.fileNameEnd + ".dat screen no \n");
	
// 	ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("def2 ", "left", 12) + FormatString("notwater", "left", 15) + " print  "  + itos(CurrentSystem.logDump) + " \"${time}   ${strain1}   ${strain2}   ${strain3}" + "   ${avePxx2}   ${avePyy2}   ${avePzz2}   ${aveStress2}   ${visc2} \" file NotWaterFix" + CurrentSystem.fileNameEnd + ".dat screen no \n");
	
// 	ReturnValue += ( FormatString("fix ", "left", 20) + FormatString("def3 ", "left", 12) + FormatString("all", "left", 15) + " print  "  + itos(CurrentSystem.logDump) + " \"${time}   ${strain1}   ${strain2}   ${strain3}"  + "   ${avePxx3}   ${avePyy3}   ${avePzz3}   ${aveStress3}   ${visc3} \" file SystemFix" + CurrentSystem.fileNameEnd + ".dat screen no \n");
	
	
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("print ", "left", 20) + "\"Initial Length, Lx0: ${Lx0}\" \n");
// 	ReturnValue +="\n";
// 	ReturnValue += ( FormatString("thermo ", "left", 20) + itos(CurrentSystem.logDump) + " \n");
// 	ReturnValue += ( FormatString("thermo_style " , "left", 20) + "custom  step  temp press  lx  ly  lz  pxx  pyy  pzz pxy  pxz  pyz  ke pe \n");
// 	ReturnValue += ( FormatString("thermo_modify ", "left", 20) + "flush   yes \n");
// 	ReturnValue += ( FormatString("thermo_modify ", "left", 20) + "norm    yes \n");
	
	
// 	ReturnValue +="\n";
	
// 	ReturnValue += ( FormatString("dump ", "left", 20) + FormatString("m1 ", "left", 10) + FormatString("micelle ", "left", 15)  + FormatString(" custom", "left", 10)   + FormatString(itos(CurrentSystem.trjDumpShort),"left",10) + FormatString("MicelleWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   x   y   z   vx   vy   vz \n");
	
// 	ReturnValue += ( FormatString("dump ", "left", 20) + FormatString("m2 ", "left", 10) + FormatString("micelle ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpShort),"left",10) + FormatString("MicelleUnWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   xu   yu   zu   c_atomprop1   c_atomprop2   c_atomprop4 \n");
	
	
// 	ReturnValue += ( FormatString("dump ", "left", 20) + FormatString("w1 ", "left", 10) + FormatString("notmicelle ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpLong),"left",10) + FormatString("WaterIonWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   x   y   z   vx   vy   vz \n");
	
// 	ReturnValue += ( FormatString("dump ", "left", 20) + FormatString("w2 ", "left", 10) + FormatString("notmicelle ", "left", 15)  + FormatString(" custom", "left", 10)  + FormatString(itos(CurrentSystem.trjDumpLong),"left",10)  + FormatString("WaterIonUnWrap" + CurrentSystem.fileNameEnd  + ".lammpstrj","left",45) + "  id   type   xu   yu   zu \n");
	
	
// 	ReturnValue +="\n";
	
// 	ReturnValue += ( FormatString("restart ", "left", 20)  + itos(CurrentSystem.restartFreq)   + "   Restart" + CurrentSystem.fileNameEnd + "-*.equil \n");
	
// 	ReturnValue += ( FormatString("run_style ", "left", 20) + "verlet \n");
// 	ReturnValue += ( FormatString("run ", "left", 20) + itos(CurrentSystem.runStep) + " \n");
	
// 	ReturnValue +="\n";
// 	ReturnValue +="\n";
	


// 	if (CurrentSystem.runType == "Tension")
// 	{
// 		ReturnValue += ( FormatString("unfix ", "left", 20) +"def4m \n");
// 	}
	

	
// 	ReturnValue += ( FormatString("undump ", "left", 20)+"m1 \n");
// 	ReturnValue += ( FormatString("undump ", "left", 20)+"m2 \n");
// 	ReturnValue += ( FormatString("undump ", "left", 20)+"w1 \n");
// 	ReturnValue += ( FormatString("undump ", "left", 20)+"w2 \n");
// 	ReturnValue += ( FormatString("unfix ", "left", 20) +"ensemble \n");
	
	
	
// 	ReturnValue += ( FormatString("unfix ", "left", 20) +"avtym1 \n");
// 	ReturnValue += ( FormatString("unfix ", "left", 20) +"avtym2 \n");
// 	ReturnValue += ( FormatString("unfix ", "left", 20) +"avtym3 \n");
// 	ReturnValue += ( FormatString("unfix ", "left", 20) +"def1 \n");
// 	ReturnValue += ( FormatString("unfix ", "left", 20) +"def2 \n");
// 	ReturnValue += ( FormatString("unfix ", "left", 20) +"def3 \n");
// 	ReturnValue += ( FormatString("write_data ", "left", 20)  + "WrittenData" + CurrentSystem.fileNameEnd + "-*.dat \n");
// 	ReturnValue +="\n";
// 	ReturnValue +="\n";

// 	return ReturnValue;
// }




	
			
			//if ( (countLineInFile/linePerDump)%skipDump == 0)
			//{
				//if (countTime%skipDump == 0 and countLineInFile >= countTime*CurrentSystem.size+6 and countLineInFile <= countTime*CurrentSystem.size+8)
				//{
					//boxLengthInfo += line1;
					
					//if (iss3>>x1>>x2>>x3>>x4>>x5>>x6)
					//{
						//CurrentSystem.MultiTrj[writeTime].xlo = x1;
						//CurrentSystem.MultiTrj[writeTime].xhi = x2;
						//CurrentSystem.MultiTrj[writeTime].xBoxLength = CurrentSystem.MultiTrj[writeTime].xhi - CurrentSystem.MultiTrj[writeTime].xlo;
						
						//CurrentSystem.MultiTrj[writeTime].ylo = x3;
						//CurrentSystem.MultiTrj[writeTime].yhi = x4;
						//CurrentSystem.MultiTrj[writeTime].yBoxLength = CurrentSystem.MultiTrj[writeTime].yhi - CurrentSystem.MultiTrj[writeTime].ylo;
						
						//CurrentSystem.MultiTrj[writeTime].zlo = x5;
						//CurrentSystem.MultiTrj[writeTime].zhi = x6;
						//CurrentSystem.MultiTrj[writeTime].zBoxLength = CurrentSystem.MultiTrj[writeTime].zhi - CurrentSystem.MultiTrj[writeTime].zlo;
						
						//boxLengthInfo = "";
					//}
				//}
				
				//if( (iss1>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
				//}
				//if( (iss2>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
					
					//count++;
				//}
				//if(count == (countTime+1)*CurrentSystem.size)
				//{
					//countTime++;
					//count = 0;
					
					//UnfixTrj.timeDump = countTime;
					//CurrentSystem.MultiTrj.push_back(UnfixTrj);
					
					//UnfixTrj.BeadInfo.clear();
					//UnfixTrj.BeadInfo.resize(CurrentSystem.size);
				//}
			//}
			
			
			//if ( (countLineInFile/linePerDump)%skipDump == 0)
			//{
				//if (countTime%skipDump == 0 and countLineInFile >= countTime*CurrentSystem.size+6 and countLineInFile <= countTime*CurrentSystem.size+8)
				//{
					//boxLengthInfo += line1;
					
					//if (iss3>>x1>>x2>>x3>>x4>>x5>>x6)
					//{
						//CurrentSystem.MultiTrj[writeTime].xlo = x1;
						//CurrentSystem.MultiTrj[writeTime].xhi = x2;
						//CurrentSystem.MultiTrj[writeTime].xBoxLength = CurrentSystem.MultiTrj[writeTime].xhi - CurrentSystem.MultiTrj[writeTime].xlo;
						
						//CurrentSystem.MultiTrj[writeTime].ylo = x3;
						//CurrentSystem.MultiTrj[writeTime].yhi = x4;
						//CurrentSystem.MultiTrj[writeTime].yBoxLength = CurrentSystem.MultiTrj[writeTime].yhi - CurrentSystem.MultiTrj[writeTime].ylo;
						
						//CurrentSystem.MultiTrj[writeTime].zlo = x5;
						//CurrentSystem.MultiTrj[writeTime].zhi = x6;
						//CurrentSystem.MultiTrj[writeTime].zBoxLength = CurrentSystem.MultiTrj[writeTime].zhi - CurrentSystem.MultiTrj[writeTime].zlo;
						
						//boxLengthInfo = "";
					//}
				//}
				
				//if( (iss1>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
				//}
				//if( (iss2>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
					
					//count++;
				//}
				//if(count == (countTime+1)*CurrentSystem.size)
				//{
					//countTime++;
					//count = 0;
					
					//UnfixTrj.timeDump = countTime;
					//CurrentSystem.MultiTrj.push_back(UnfixTrj);
					
					//UnfixTrj.BeadInfo.clear();
					//UnfixTrj.BeadInfo.resize(CurrentSystem.size);
				//}
			//}

		
		
		
			//float  x1, x2, x3, x4, x5, x6;
			//int    i1, i2;
			//string s1, s2;
			
			//countLineInFile++;
			
			//std::istringstream iss1(line1);
			//std::istringstream iss2(line2);
			//std::istringstream iss3(boxLengthInfo);
			
			
			//if (countTime%skipDump == 0 and countLineInFile >= countTime*CurrentSystem.size+6 and countLineInFile <= countTime*CurrentSystem.size+8)
			//{
				//boxLengthInfo += line1;
				
				//if (iss3>>x1>>x2>>x3>>x4>>x5>>x6)
				//{
					//UnfixTrj.xlo = x1;
					//UnfixTrj.xhi = x2;
					//UnfixTrj.xBoxLength = UnfixTrj.xhi - UnfixTrj.xlo;
					
					//UnfixTrj.ylo = x3;
					//UnfixTrj.yhi = x4;
					//UnfixTrj.yBoxLength = UnfixTrj.yhi - UnfixTrj.ylo;
					
					//UnfixTrj.zlo = x5;
					//UnfixTrj.zhi = x6;
					//UnfixTrj.zBoxLength = UnfixTrj.zhi - UnfixTrj.zlo;
					
					//boxLengthInfo = "";
				//}
			//}
			
			//if ( (countLineInFile/linePerDump)%skipDump == 0)
			//{
				//if( (iss1>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
				//}
				//if( (iss2>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
					
					//count++;
				//}
				//if(count == (countTime+1)*CurrentSystem.size)
				//{
					//countTime++;
					//count = 0;
					
					//UnfixTrj.timeDump = countTime;
					//CurrentSystem.MultiTrj.push_back(UnfixTrj);
					
					//UnfixTrj.BeadInfo.clear();
					//UnfixTrj.BeadInfo.resize(CurrentSystem.size);
				//}
			//}
			
		//}
		//ReadFile1.close();

		
		
		
		
		
		
		//while (std::getline(ReadFile1, line1) or std::getline(ReadFile2, line2) )
		//{
			//float  x1, x2, x3, x4, x5, x6;
			//int    i1, i2;
			//string s1, s2;
			
			//countLineInFile++;
			
			//std::istringstream iss1(line1);
			//std::istringstream iss2(line2);
			//std::istringstream iss3(boxLengthInfo);
			
			
			//if (countTime%skipDump == 0 and countLineInFile >= countTime*CurrentSystem.size+6 and countLineInFile <= countTime*CurrentSystem.size+8)
			//{
				//boxLengthInfo += line1;
				
				//if (iss3>>x1>>x2>>x3>>x4>>x5>>x6)
				//{
					//UnfixTrj.xlo = x1;
					//UnfixTrj.xhi = x2;
					//UnfixTrj.xBoxLength = UnfixTrj.xhi - UnfixTrj.xlo;
					
					//UnfixTrj.ylo = x3;
					//UnfixTrj.yhi = x4;
					//UnfixTrj.yBoxLength = UnfixTrj.yhi - UnfixTrj.ylo;
					
					//UnfixTrj.zlo = x5;
					//UnfixTrj.zhi = x6;
					//UnfixTrj.zBoxLength = UnfixTrj.zhi - UnfixTrj.zlo;
					
					//boxLengthInfo = "";
				//}
			//}
			
			//if ( (countLineInFile/linePerDump)%skipDump == 0)
			//{
				//if( (iss1>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
				//}
				//if( (iss2>>i1>>i2>>x1>>x2>>x3>>x4>>x5>>x6) and count >= 0 and count < CurrentSystem.size )
				//{
					//UnfixTrj.BeadInfo[i1-1].x = x1*10;
					//UnfixTrj.BeadInfo[i1-1].y = x2*10;
					//UnfixTrj.BeadInfo[i1-1].z = x3*10;
					
					//count++;
				//}
				//if(count == (countTime+1)*CurrentSystem.size)
				//{
					//countTime++;
					//count = 0;
					
					//UnfixTrj.timeDump = countTime;
					//CurrentSystem.MultiTrj.push_back(UnfixTrj);
					
					//UnfixTrj.BeadInfo.clear();
					//UnfixTrj.BeadInfo.resize(CurrentSystem.size);
				//}
			//}
			
		//}
		//ReadFile1.close();
