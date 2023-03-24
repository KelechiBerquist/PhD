import numpy as np
import os

# /media/KaceHDD1/Lipids/OrderedStructures/1-B2P-B1N-0.3_M2-Rot1.xyz
x = 0
y = 1
z = 2

extensions = ['.xyz']
first = '/media/KaceHDD1/Lipids/'

for name in ['1', '2', '4']:
	rootdir = os.path.join(first, name)
	
	for subdir, dirs, files in os.walk(rootdir):
		for file in files:
			ext = os.path.splitext(file)[-1].lower()
			if ext in extensions:
				SubDirs = subdir.split('-')
				WriteDir = SubDirs[0] + '-ReorderStruct'
				# print (subdir, WriteDir)
				# print (dirs, subdir, file)
				# print (os.path.join(subdir, file))
				# print (file.split('.x'))

				WriteData = ''
				FileName = os.path.join(subdir, file)
				ReadFile = open(FileName, 'r')
				count = 0
				for line in ReadFile:
					if 'Bead' in line:
						Type = line.split()[0]
						Line = [float (i) for i in line.split()[1:] ]
						WriteData += ( '{:6s}'.format('ATOM ') +  '{:5d}'.format(count+1) + '{:3s}'.format(Type[0]+Type[-2:]) + 
							'{:1s}'.format(' ') + '{:3s}'.format(Type[0]+Type[-2:]) + '{:11s}'.format(' ') + 
							'{:8.3f}'.format(Line[x]) + '{:8.3f}'.format(Line[y]) + '{:8.3f}'.format(Line[z]) +
							'\n')
				ReadFile.close()


				WriteFileName = os.path.join(WriteDir, file.split('.x')[0] + '.pdb') 
				WriteFile = open(WriteFileName, 'w')

				WriteFile.write(WriteData)
				WriteFile.close()


