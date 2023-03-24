import os
import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
# import matplotlib.pylab as plt
import matplotlib.image as mpimg
import numpy as np
from scipy.stats import linregress



def GetMaxPxx (ThisArray):
	ThisArray = np.array(ThisArray)
	# print(np.max(ThisArray[:, pxxIdx]))
	return 10*np.max(ThisArray[:, pxxIdx])

def GetEmod (ThisArray):
	ThisArray    =  np.array(ThisArray)
	TruncArray   =  ThisArray[(ThisArray[:, strainIdx] >=0) & (ThisArray[:, strainIdx] <=emodCutOff)]
	XEmod        =  TruncArray [:, strainIdx]
	YEmod        =  TruncArray [:, pxxIdx]
	slope, intercept, r_value, p_value, std_err = linregress(XEmod, YEmod)
	return slope

def GetTimeStrainPxx (ThisArray):
	ThisArray  =  np.array(ThisArray)
	Data       =  []
	avePxx     =  0
	countFreq  =  1
	countTime  =  1

	for line in ThisArray:
		if (line[strainIdx] > (countTime)*freq):
			Data.append([int(line[timeIdx]),   round((countTime)*freq, 2),   10*avePxx/countFreq])
			avePxx     =  0
			countFreq  =  1
			countTime  += 1

		if (line[strainIdx] <= (countTime)*freq):
			avePxx     += line[pxxIdx]
			countFreq  += 1

	Data = np.array(Data)
	return Data



Chosen            =   [1, 2, 3, 4]
Structure         =   ['Sph', 'RLM', 'WLM', 'BWLM']
# Rate              =   [0.3, 3, 30]
# Rate              =   [0.1, 1, 10]
Rate              =   [0.1, 0.3, 1, 3, 10, 30]
Bead              =   [4, 7]
Strain            =   [0, 4.5]
Direct            =   ["X", "Y"]
Bd2               =   []
Bd4               =   []
Bd5               =   []
Bd7               =   []
Bin               =   []
timeStep          =   30
binIdx            =   0
t2Idx             =   2
t4Idx             =   4
t5Idx             =   5
t7Idx             =   7
RlxTime           =   3
legendCols        =   3

rootDir           =   "/media/KaceHDD1/Struct/"
WriteFolderName   =   "/media/KaceHDD1/Struct/ImageVideos/Load_Hist_Images"
# /media/KaceHDD1/Struct/ImageVideos/Load_Hist/Hist-Bd7-Load-Rate1-Spn3-S3-Y.dat


for chosen in Chosen:
	for rate in  Rate:
		for direct in Direct:
			for bead in  Bead:
				for strain in  Strain:
					ReadFolderName    =  "/media/KaceHDD1/Struct/ImageVideos/Load_Hist"
					ReadFileNameEnd   =  "-Bd" + str(bead) + "-Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-En" + str(1) + "-S" + str(strain) + "-" + direct
					# ReadFileNameEnd   =  "-Bd" + str(bead) + "-Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-S" + str(strain) + "-" + direct
					ReadFileName      =  os.path.join(ReadFolderName, "Gofr" + ReadFileNameEnd  + ".dat")
					ReadFile          =  open(ReadFileName, 'r')
					ThisFile          =  ReadFile.readlines()
					ReadFile.close()
					ThatBd2           =   []
					ThatBd4           =   []
					ThatBd5           =   []
					ThatBd7           =   []
					ThatBin           =   []
					for line in ThisFile:
						ThatBd2.append(float(line.split()[t2Idx]))
						ThatBd4.append(float(line.split()[t4Idx]))
						ThatBd5.append(float(line.split()[t5Idx]))
						ThatBd7.append(float(line.split()[t7Idx]))
						ThatBin.append(float(line.split()[binIdx]))

					# print(ThatFile)
					# ThatFile          =  np.array(ThatFile)

					Bd2.append(ThatBd2)
					Bd4.append(ThatBd4)
					Bd5.append(ThatBd5)
					Bd7.append(ThatBd7)
					Bin.append(ThatBin)

Bd2           =   np.array(Bd2)
Bd4           =   np.array(Bd4)
Bd5           =   np.array(Bd5)
Bd7           =   np.array(Bd7)
Bin           =   np.array(Bin)

Bd2.shape     =   (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)
Bd4.shape     =   (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)
Bd5.shape     =   (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)
Bd7.shape     =   (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)
Bin.shape     =   (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)



print(Bin.shape)
# print(Bin[0, 0, 0, 0, 0])





# ORDER: (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)
#For Plotting Apolar-Apolar In Time
for i in range (len(Chosen)):
	for k in range (len(Direct)):
		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		if (Chosen[i] == 2):
			plt.plot(Bin[i, 0, k, 1, 0], Bd7[i, 0, k, 1, 0], '-', linewidth=1, label="Initial")
		else:
			plt.plot(Bin[i, 0, k, 0, 0], Bd4[i, 0, k, 0, 0], '-', linewidth=1, label="Initial")
		for j in range (1, len(Rate), 2):
			if (Chosen[i] == 2):
				plt.plot(Bin[i, j, k, 1, 1], Bd7[i, j, k, 1, 1], '-', linewidth=1.5, label="W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
			else:
				plt.plot(Bin[i, j, k, 0, 1], Bd4[i, j, k, 0, 1], '-', linewidth=1.5, label="W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.2*ymaxThis)
		# plt.xlim((0, 14))
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Radial Distribution Function')
		plt.xlabel("Distance ($\AA$)")
		# plt.xlabel('Distance (A)')
		plt.legend(bbox_to_anchor=(0.01, 0.87, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-" + str(Direct[k]) + "-Ap-Ap" + ".png"))
		plt.show()
		plt.close(fig)


		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		if (Chosen[i] == 2):
			plt.plot(Bin[i, 0, k, 1, 0], Bd2[i, 0, k, 1, 0], '-', linewidth=1, label="Initial")
		else:
			plt.plot(Bin[i, 0, k, 0, 0], Bd2[i, 0, k, 0, 0], '-', linewidth=1, label="Initial")
		for j in range (1, len(Rate), 2):
			if (Chosen[i] == 2):
				plt.plot(Bin[i, j, k, 1, 1], Bd2[i, j, k, 1, 1], '-', linewidth=1.5, label="W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
			else:
				plt.plot(Bin[i, j, k, 0, 1], Bd2[i, j, k, 0, 1], '-', linewidth=1.5, label="W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.1*ymaxThis)
		# plt.xlim((0, 14))
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Radial Distribution Function')
		plt.xlabel("Distance ($\AA$)")
		# plt.xlabel('Distance (A)')
		plt.legend(bbox_to_anchor=(0.01, 0.87, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-" + str(Direct[k]) + "-Ap-P" + ".png"))
		plt.show()
		plt.close(fig)


		


# ORDER: (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)
#For Plotting Apolar-Apolar In Time
for i in range (len(Chosen)):
	for j in range (len(Rate)):
		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		if (Chosen[i] == 2):
			plt.plot(Bin[i, 0, 0, 1, 0], Bd7[i, 0, 0, 1, 0], '-', linewidth=1, label="Initial")
			plt.plot(Bin[i, j, 0, 1, 1], Bd7[i, j, 0, 1, 1], '-', linewidth=1.5, label=str(Direct[0]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
			plt.plot(Bin[i, j, 1, 1, 1], Bd7[i, j, 1, 1, 1], '-', linewidth=1.5, label=str(Direct[1]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
		else:
			plt.plot(Bin[i, 0, 0, 0, 0], Bd4[i, 0, 0, 0, 0], '-', linewidth=1, label="Initial")
			plt.plot(Bin[i, j, 0, 0, 1], Bd4[i, j, 0, 0, 1], '-', linewidth=1.5, label=str(Direct[0]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
			plt.plot(Bin[i, j, 1, 0, 1], Bd4[i, j, 1, 0, 1], '-', linewidth=1.5, label=str(Direct[1]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.2*ymaxThis)
		# plt.xlim((0, 14))
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Radial Distribution Function')
		plt.xlabel("Distance ($\AA$)")
		# plt.xlabel('Distance (A)')
		plt.legend(bbox_to_anchor=(0.01, 0.92, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-R" + str(j+1) + "-Ap-Ap" + ".png"))
		plt.show()
		plt.close(fig)



		fig = plt.figure(k, figsize=(3.5, 3.5))
		plt.subplot(111)
		if (Chosen[i] == 2):
			plt.plot(Bin[i, 0, 0, 1, 0], Bd2[i, 0, 0, 1, 0], '-', linewidth=1, label="Initial")
			plt.plot(Bin[i, j, 0, 1, 1], Bd2[i, j, 0, 1, 1], '-', linewidth=1.5, label=str(Direct[0]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
			plt.plot(Bin[i, j, 1, 1, 1], Bd2[i, j, 1, 1, 1], '-', linewidth=1.5, label=str(Direct[1]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
		else:
			plt.plot(Bin[i, 0, 0, 0, 0], Bd2[i, 0, 0, 0, 0], '-', linewidth=1, label="Initial")
			plt.plot(Bin[i, j, 0, 0, 1], Bd2[i, j, 0, 0, 1], '-', linewidth=1.5, label=str(Direct[0]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
			plt.plot(Bin[i, j, 1, 0, 1], Bd2[i, j, 1, 0, 1], '-', linewidth=1.5, label=str(Direct[1]) + "  W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
		yminThis, ymaxThis = plt.ylim()
		xminThis, xmaxThis = plt.xlim()
		plt.ylim(ymax=1.1*ymaxThis)
		# plt.xlim((0, 14))
		plt.xlim((0.95*xminThis, 1.05*xmaxThis))
		plt.ylabel('Radial Distribution Function')
		plt.xlabel("Distance ($\AA$)")
		# plt.xlabel('Distance (A)')
		plt.legend(bbox_to_anchor=(0.01, 0.92, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
		plt.xticks(rotation=45)
		plt.tight_layout()
		plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-R" + str(j+1) + "-Ap-P" + ".png"))
		plt.show()
		plt.close(fig)


		













# # ORDER: (len(Chosen), len(Rate), len(Direct), len(Bead), len(Strain), -1)
# #For Plotting Each Bead Type Against Itself In Time
# for i in range (len(Chosen)):
# 		for l in range (len(Bead)):
# 			fig = plt.figure(i, figsize=(3.5, 3.5))
# 			plt.subplot(111)
# 			for j in range (0, len(Rate)-2, 2):
# 			# for j in range (len(Rate)):
# 				plt.plot(Bin[i, j, 0, l, 1], Bd2[i, j, 0, l, 1], '-', linewidth=1.5, label=Direct[0] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
# 				plt.plot(Bin[i, j, 1, l, 1], Bd2[i, j, 1, l, 1], '-', linewidth=1.5, label=Direct[1] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))	
# 			yminThis, ymaxThis = plt.ylim()
# 			xminThis, xmaxThis = plt.xlim()
# 			plt.ylim(ymax=1.25*ymaxThis)
# 			plt.xlim((0, 14))
# 			# plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 			plt.ylabel('Radial Distribution Function')
# 			plt.xlabel('Distance (A)')
# 			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols-1, borderaxespad=0., fontsize = 'xx-small')
# 			plt.tight_layout()
# 			plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-PR1-Bd" + str(Bead[l]) + "-Bd2" + ".png"))
# 			plt.show()
# 			plt.close(fig)


# 			fig = plt.figure(i, figsize=(3.5, 3.5))
# 			plt.subplot(111)
# 			for j in range (0, len(Rate)-2, 2):
# 			# for j in range (len(Rate)):
# 				plt.plot(Bin[i, j, 0, l, 1], Bd4[i, j, 0, l, 1], '-', linewidth=1.5, label=Direct[0] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
# 				plt.plot(Bin[i, j, 1, l, 1], Bd4[i, j, 1, l, 1], '-', linewidth=1.5, label=Direct[1] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))	
# 			yminThis, ymaxThis = plt.ylim()
# 			xminThis, xmaxThis = plt.xlim()
# 			plt.ylim(ymax=1.25*ymaxThis)
# 			plt.xlim((0, 14))
# 			# plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 			plt.ylabel('Radial Distribution Function')
# 			plt.xlabel('Distance (A)')
# 			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols-1, borderaxespad=0., fontsize = 'xx-small')
# 			plt.tight_layout()
# 			plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-PR1-Bd" + str(Bead[l]) + "-Bd4" + ".png"))
# 			plt.show()
# 			plt.close(fig)


# 			fig = plt.figure(i, figsize=(3.5, 3.5))
# 			plt.subplot(111)
# 			for j in range (0, len(Rate)-2, 2):
# 			# for j in range (len(Rate)):
# 				plt.plot(Bin[i, j, 0, l, 1], Bd7[i, j, 0, l, 1], '-', linewidth=1.5, label=Direct[0] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
# 				plt.plot(Bin[i, j, 1, l, 1], Bd7[i, j, 1, l, 1], '-', linewidth=1.5, label=Direct[1] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))	
# 			yminThis, ymaxThis = plt.ylim()
# 			xminThis, xmaxThis = plt.xlim()
# 			plt.ylim(ymax=1.25*ymaxThis)
# 			plt.xlim((0, 14))
# 			# plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 			plt.ylabel('Radial Distribution Function')
# 			plt.xlabel('Distance (A)')
# 			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols-1, borderaxespad=0., fontsize = 'xx-small')
# 			plt.tight_layout()
# 			plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-PR1-Bd" + str(Bead[l]) + "-Bd7" + ".png"))
# 			plt.show()
# 			plt.close(fig)



# 			fig = plt.figure(i, figsize=(3.5, 3.5))
# 			plt.subplot(111)
# 			for j in range (len(Rate)-3, len(Rate), 2):
# 			# for j in range (len(Rate)):
# 				plt.plot(Bin[i, j, 0, l, 1], Bd2[i, j, 0, l, 1], '-', linewidth=1.5, label=Direct[0] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
# 				plt.plot(Bin[i, j, 1, l, 1], Bd2[i, j, 1, l, 1], '-', linewidth=1.5, label=Direct[1] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))	
# 			yminThis, ymaxThis = plt.ylim()
# 			xminThis, xmaxThis = plt.xlim()
# 			plt.ylim(ymax=1.25*ymaxThis)
# 			plt.xlim((0, 14))
# 			# plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 			plt.ylabel('Radial Distribution Function')
# 			plt.xlabel('Distance (A)')
# 			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols-1, borderaxespad=0., fontsize = 'xx-small')
# 			plt.tight_layout()
# 			plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-PR2-Bd" + str(Bead[l]) + "-Bd2" + ".png"))
# 			plt.show()
# 			plt.close(fig)


# 			fig = plt.figure(i, figsize=(3.5, 3.5))
# 			plt.subplot(111)
# 			for j in range (len(Rate)-3, len(Rate), 2):
# 			# for j in range (len(Rate)):
# 				plt.plot(Bin[i, j, 0, l, 1], Bd4[i, j, 0, l, 1], '-', linewidth=1.5, label=Direct[0] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
# 				plt.plot(Bin[i, j, 1, l, 1], Bd4[i, j, 1, l, 1], '-', linewidth=1.5, label=Direct[1] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))	
# 			yminThis, ymaxThis = plt.ylim()
# 			xminThis, xmaxThis = plt.xlim()
# 			plt.ylim(ymax=1.25*ymaxThis)
# 			plt.xlim((0, 14))
# 			# plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 			plt.ylabel('Radial Distribution Function')
# 			plt.xlabel('Distance (A)')
# 			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols-1, borderaxespad=0., fontsize = 'xx-small')
# 			plt.tight_layout()
# 			plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-PR2-Bd" + str(Bead[l]) + "-Bd4" + ".png"))
# 			plt.show()
# 			plt.close(fig)


# 			fig = plt.figure(i, figsize=(3.5, 3.5))
# 			plt.subplot(111)
# 			for j in range (len(Rate)-3, len(Rate), 2):
# 			# for j in range (len(Rate)):
# 				plt.plot(Bin[i, j, 0, l, 1], Bd7[i, j, 0, l, 1], '-', linewidth=1.5, label=Direct[0] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))
# 				plt.plot(Bin[i, j, 1, l, 1], Bd7[i, j, 1, l, 1], '-', linewidth=1.5, label=Direct[1] + " W~" + '{:1.1f}'.format(RlxTime*Rate[j]))	
# 			yminThis, ymaxThis = plt.ylim()
# 			xminThis, xmaxThis = plt.xlim()
# 			plt.ylim(ymax=1.25*ymaxThis)
# 			plt.xlim((0, 14))
# 			# plt.xlim((0.95*xminThis, 1.05*xmaxThis))
# 			plt.ylabel('Radial Distribution Function')
# 			plt.xlabel('Distance (A)')
# 			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols-1, borderaxespad=0., fontsize = 'xx-small')
# 			plt.tight_layout()
# 			plt.savefig(os.path.join(WriteFolderName, "Gofr-Spn" + str(Chosen[i]) + "-PR2-Bd" + str(Bead[l]) + "-Bd7" + ".png"))
# 			plt.show()
# 			plt.close(fig)























