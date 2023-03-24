#!/usr/bin/python
import os
import math
#import numpy as np 
#import numpy.linalg as LA 

x = 0
y = 1
z = 2

SimulationType = 'Equilibrium'
#LocatePC  =  '/media/KaceHDD1/1.0k/'
#LocatePC  = '/media/OkwuchisHDD/'
LocatePC = '/media/vanessa/Kay3TBMob/Lipids/2/B6P'

#MainVector = ['CTA', 'CCN', 'MCN']
MainVector = ['CCN']

MainChainVector  = [1, 2, 4]
MainBeadVector   = [2, 4, 6]
SoluteBeadVector = [1, 2, 3, 4, 6]
R                = [0.3, 1.0, 3.0]
SystemIndex      = ['PN', 'ZZ']

WA    =   400e3                   
AFW   =   40e3

for chain in MainChainVector:
	
	MainChain = chain*1000
	
	
	for mainBead in MainBeadVector:
		for index in SystemIndex:
			
			MainTop  =  'B' + str(mainBead+1) + index[0]
			
			
			for soluteBead in SoluteBeadVector:
				
				SoluteTop  =  'B' + str(soluteBead+1) + index[1]
			
			
				for r in R:
				
					Folder  =  LocatePC + str(chain) + '/' + MainTop +  '/' + SoluteTop + '/' + str(float(r)) + '/'
					
					
					ReadWriteFolderName    =  Folder + str(float(r)) + '-EquilibriumGromacs/'
					
					
					CounterMainChain   =   MainChain                 
					SoluteChain        =   int(r*MainChain)  
					CounterSoluteChain =   SoluteChain 
					
					MainBead           =   mainBead + 1
					SoluteBead         =   soluteBead + 1
					MainChaB           =   MainBead*MainChain 
					SoluteChaB         =   SoluteBead*SoluteChain
					Size               =   int (MainChaB + SoluteChaB + CounterMainChain + CounterSoluteChain + WA + AFW)
					
					
					ReadFileName1   =  ReadWriteFolderName + 'NPT2.pdb'
					ReadFile1       =  open(ReadFileName1, 'r')
					
					
					Coords   =  []
					
					countTime  =   0   
					countLines =   0
					
					
					for line in ReadFile1:
						if 'ATOM' in line:
							countLines = countLines + 1
							Lines  = [float(aj) for aj in line.split() [5:8] ]
							Coords.append(Lines)
							#print (Lines)
							#
						#
						elif 'TER' in line:
							continue
						#
						elif 'ENDMDL' in line:
							WriteFileName1  =  ReadWriteFolderName + 'NPT2-' + SoluteType + '_4.5-' + str(float(r)) + '_'+ str(countTime) + '.xyz'
							WriteFile1      =   open(WriteFileName1, 'w')
							#print (Coords)
							#print ('')
							
							WriteFile1.write (str(len(Coords)) + '\n' + WriteFileName1 + '\n')
							
							for  ak  in  range  (0,  len(Coords) ):
								WriteFile1.write ('Haha  ' + str(Coords[ak][x]) + '   ' +  str(Coords[ak][y]) +  '   ' + str(Coords[ak][z]) + ' ' + '\n' )
							#WriteFile1.write ('End of current Time Step  \n')
							countTime  = countTime  + 1
							Coords.clear()
							WriteFile1.close()
							
					print ('Total Time Step written is ', countTime, ' \n \n')
					print ('Total Size of system after all TimeSteps  ',  countLines, ' \n \n')
					print ('For Side by Side Comparison  ')
					print ('Countlines is  ',  countLines, ' and it ought to be  ', Size*countTime, ' \n \n')
					
					#
					ReadFile1.close()

