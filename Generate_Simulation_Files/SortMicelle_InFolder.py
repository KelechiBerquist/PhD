#!/usr/bin/python
import os
import math
import numpy as np 
#import numpy.linalg as LA 


#/media/KaceHDD1/SomeStructures/MCN-Bta-2_M3-T0.xyz
#/media/KaceHDD1/SomeStructures/MCN-Cta-4_M2-T0.xyz
#/media/KaceHDD1/SomeStructures/MCN-Ota-7_M4-T0.xyz
#/media/KaceHDD1/SomeStructures/PCN-Mcn-1_M2-T0.xyz
#/media/KaceHDD1/SomeStructures/PCN-Mcn-7_M1-T0.xyz

x = 0
y = 1
z = 2

FolderName =  '/media/KaceHDD1/DifferentStructures/SortStructures/'
FilesForSorting = ['MCN-Ota-7_M4-T0',
					'PCN-Mcn-1_M2-T0',
					'PCN-Mcn-7_M2-T0',
					'CTA-NaBnt-0.3_M4-T0',
					]


for Micelle in FilesForSorting:
	
	AtomIndex             =  []
	UnsortedMicelleInfo   =  []
	SortedMicelleInfo     =  []
	
	Header = ''
	CoordsInfo = ''
	
	
	ReadFileName1   =  FolderName + Micelle + '.xyz'
	ReadFile1       =  open(ReadFileName1, 'r')
	
	for line in ReadFile1:
		if 'Bead' in line:
			UnsortedMicelleInfo.append(line)
		else:
			Header += line
			
	ReadFile1.close()
	
	
	ReadFileName1   =  FolderName +'W-' +  Micelle + '.dat'
	ReadFile1       =  open(ReadFileName1, 'r')
	
	for line in ReadFile1:
		AtomIndex.append(line)
	ReadFile1.close()
	
	
	AtomIndex = np.array(AtomIndex)
	UnsortedMicelleInfo = np.array(UnsortedMicelleInfo)
	
	print (AtomIndex)
	
	
	AtomIndex = np.char.replace(AtomIndex, '\n', '   ')
	#AtomIndex = np.char.strip(AtomIndex, '\n')
	
	print (AtomIndex)
	
	JoinedInfo = np.core.defchararray.add(AtomIndex, UnsortedMicelleInfo)
	
	
	SortedMicelleInfo = sorted(JoinedInfo)
	
	
	WriteFileName1   =  FolderName + 'Sorted-' + Micelle + '.xyz'
	WriteFile1       =  open(WriteFileName1, 'w')
	
	WriteFile1.write (Header)
	
	
	for line in SortedMicelleInfo:
		WriteFile1.write (line[8:])
		#print (line[3:])
	WriteFile1.close()
