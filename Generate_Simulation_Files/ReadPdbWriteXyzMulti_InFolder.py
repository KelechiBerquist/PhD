#!/usr/bin/python
import os
import math
#import numpy as np 
#import numpy.linalg as LA 


x = 0
y = 1
z = 2

FileName = 'Inserted-NPT2'

ReadFileName1   =  FileName + '.pdb'
ReadFile1       =  open(ReadFileName1, 'r')


Coords   =  []

countTime  =   0   
countLines =   0

CoordsInfo = ''


for line in ReadFile1:
	if 'ATOM' in line:
		countLines = countLines + 1
		#Lines  = [float(aj) for aj in line.split() [5:8] ]
		#Coords.append(Lines)
		#print (Lines)
		#print (CoordsInfo)
		#
		CoordsInfo = CoordsInfo + 'HaHa    ' + line[30:53] + '\n'
	
	elif 'TER' in line:
		continue
	#
	elif 'ENDMDL' in line:
		#WriteFileName1  =   FileName + '.xyz'
		WriteFileName1  =   'MicelleWaFromGromacs.xyz'
		WriteFile1      =   open(WriteFileName1, 'w')
		#print (Coords)
		#print ('')
		
		WriteFile1.write (str(countLines) + '\n' + WriteFileName1 + '\n')
		
		
		WriteFile1.write (CoordsInfo)
		WriteFile1.close()
		CoordsInfo = ''
		countTime  = countTime  + 1
		
		#WriteFile1.write (str(len(Coords)) + '\n' + WriteFileName1 + '\n')
		
		#for  ak  in  range  (0,  len(Coords) ):
			#WriteFile1.write ('Haha  ' + str(Coords[ak][x]) + '   ' +  str(Coords[ak][y]) +  '   ' + str(Coords[ak][z]) + ' ' + '\n' )
		##WriteFile1.write ('End of current Time Step  \n')
		#countTime  = countTime  + 1
		#Coords.clear()
		#WriteFile1.close()
		
#print ('MainPhase:', MainPhase, ' SoluteType:', SoluteType, ' Total Time Step written is ', countTime, ' \n \n')
#print ('Total Size of system after all TimeSteps  ',  countLines, ' \n \n')
#print ('For Side by Side Comparison  ')
#print ('MainPhase:', MainPhase, 'SoluteType:', SoluteType, 'R:', r, ' Countlines:',  countLines, 'Size*countTime', Size*countTime, ' \n')

#
ReadFile1.close()
