import os
import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
# import matplotlib.pylab as plt
import matplotlib.image as mpimg
import numpy as np
from scipy.stats import linregress




def GetSlope (X, Y):
	slope, intercept, r_value, p_value, std_err = linregress(X, Y)
	return slope




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



# /media/KaceHDD1/DifferentStructures/1/X/Load/MicelleFixLoad-Rate1-Spn1-En1.dat  /media/KaceHDD1/Struct/1/X/Load/MicelleFixLoad-Rate0.3-Spn1-En1.dat
Chosen            =   [1, 2, 3, 4]
Structure         =   ['Sph', 'RLM', 'WLM', 'BWLM']
Rate              =   [0.1, 0.3, 1, 3, 10, 30]
Time              =   []
Msd               =   []
ScalingFactor     =   []
timeStep          =   30
timeIdx           =   0
RlxTime           =   3
legendCols        =   3
femtosecToNanosec =   1e-6
Direct            =   ["X", "Y"]
rootDir           =   "/media/KaceHDD1/Struct/"
WriteFolderName   =   "/media/KaceHDD1/Struct/ImageVideos/Load_Images"
#			 /media/KaceHDD1/Struct/ImageVideos/Load_Tasmd/TamsdLoad-Rate0.3-Spn3-X.dat






for chosen in Chosen:
	for direct in Direct:
		for rate in  Rate:
			ReadFolderName    =  "/media/KaceHDD1/Struct/ImageVideos/Load_Tasmd"
			ReadFileNameEnd   =  "Load-Rate" + str(rate) + "-Spn" + str(chosen) + "-" + direct
			ReadFileName      =  os.path.join(ReadFolderName, "Tamsd" + ReadFileNameEnd  + ".dat")
			ReadFile          =  open(ReadFileName, 'r')
			ThisFile          =  ReadFile.readlines()
			ReadFile.close()
			ThatTime          =  []
			ThatMsd           =  []
			for line in ThisFile:
				theLine         =   [float(i) for i in line.split()]
				ThatTime.append(femtosecToNanosec*timeStep*int(line.split()[0]))
				ThatMsd.append(sum(theLine[1:])/len(theLine))

			Time.append(ThatTime)
			Msd.append(ThatMsd)
			ScalingFactor.append(GetSlope( np.log(ThatTime[1:]), np.log(ThatMsd[1:]) ) )
			# ScalingFactor.append(GetSlope( np.log(np.array(Time[0:])), np.log(np.array(Msd[0:])) ) )
			# ContentFile.append([ThatFile])




Rate                    =  np.array(Rate)
Time                    =  np.array(Time)
Msd                     =  np.array(Msd)
ScalingFactor           =  np.array(ScalingFactor)
Time.shape              =  (len(Chosen), len(Direct), len(Rate), -1)
Msd.shape               =  (len(Chosen), len(Direct), len(Rate), -1)
ScalingFactor.shape     =  (len(Chosen), len(Direct), len(Rate), -1)
print(Time.shape,  Msd.shape)

# print(Time[0, 0, 0, 0])
# print()
# print(Msd[0, 0, 0, 0])


# ORDER: (len(Chosen), len(Direct), len(Rate), -1)
#For Plotting Each Bead Type Against Itself In Time
for i in range (len(Chosen)):
	for j in range (len(Direct)):
			fig = plt.figure(j, figsize=(3.5, 3.5))
			ax = plt.subplot(111)
			for k in range (len(Rate)):
				plt.loglog(Time[i, j, k, 0], Msd[i, j, k, 0], '--', linewidth=2, label="W~" + '{:1.1f}'.format(RlxTime*Rate[k]))
				# plt.semilogx(Time[i, j, k, 0], Msd[i, j, k, 0], '-', linewidth=2, label="W~" + '{:1.1f}'.format(RlxTime*Rate[k]))
				# plt.plot(Time[i, j, k, 0], Msd[i, j, k, 0], '-', linewidth=2, label="W~" + '{:1.1f}'.format(RlxTime*Rate[k]))
			yminThis, ymaxThis = plt.ylim()
			xminThis, xmaxThis = plt.xlim()
			# plt.ylim(ymax=1.01*ymaxThis)
			# plt.ylim(ymin=0)
			# plt.ylim((yminThis, ymaxThis))
			plt.xlim((0.95*xminThis, 1.05*xmaxThis))
			ax.set_ylim(ymin=0)
			plt.ylabel('Mean Squared Displacement ('+ r'$A^2$' +')' )
			# plt.ylabel('Mean Squared Displacement ('+ r'$\frac{A}{2}$' +')' )
			plt.xlabel('Time (ns)')
			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
			plt.xticks(rotation=45)
			plt.tight_layout()
			plt.savefig(os.path.join(WriteFolderName, "Tamsd-Spn" + str(Chosen[i]) + "-" + str(Direct[j]) + ".png"))
			plt.show()
			plt.close(fig)


# ORDER: (len(Chosen), len(Direct), len(Rate), -1)
#For Plotting Each Bead Type Against Itself In Time
for i in range (len(Chosen)):
	for j in range (len(Direct)):
			fig = plt.figure(j, figsize=(3.5, 3.5))
			plt.subplot(111)
			plt.semilogx(RlxTime*Rate, ScalingFactor[i, j], '--', linewidth=2, label=Structure[i])
			yminThis, ymaxThis = plt.ylim()
			xminThis, xmaxThis = plt.xlim()
			# plt.ylim(ymax=1.01*ymaxThis)
			plt.ylim((yminThis, ymaxThis))
			plt.xlim((0.95*xminThis, 1.05*xmaxThis))
			plt.ylabel('Diffusion scaling factor' )
			plt.xlabel('Wi')
			plt.legend(bbox_to_anchor=(0.01, 0.85, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
			plt.xticks(rotation=45)
			plt.tight_layout()
			plt.savefig(os.path.join(WriteFolderName, "ScaleFac-Spn" + str(Chosen[i]) + "-" + str(Direct[j]) + ".png"))
			plt.show()
			plt.close(fig)



# ************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************************#


# #For Plotting Max Stress Against Erate For Each Structure
# for i in range (len(Chosen)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	plt.semilogx(Rate*RlxTime, MaxPxx[i], 'o', linewidth=2, label=Structure[i])
# 	plt.ylabel('Stress (MPa)')
# 	plt.xlabel('Wi')
# 	plt.legend(bbox_to_anchor=(0.05, 0.90, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
# 	plt.xlim([0.01, 300])
# 	plt.ylim([np.min(MaxPxx[i])-0.5, np.max(MaxPxx[i])+0.5])
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "MaxPxxWi-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)




# #For Plotting Max Stress Against Structure For Each Erate
# for i in range (len(Rate)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	x = np.array(range(0,len(Structure)))
# 	plt.xticks(x, Structure, rotation=45)
# 	plt.ylabel('Stress (MPa)')
# 	plt.xlabel('Structure')
# 	plt.scatter(x, MaxPxx[:,i], linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[i]*RlxTime)+')')
# 	plt.legend(bbox_to_anchor=(0.60, 0.90, .99, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
# 	plt.xlim([x[0]-1, x[len(x)-1]+1])
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "MaxPxxStruct-R" + str(i+1) + ".png"))
# 	# plt.show()
# 	plt.close(fig)



# #For Plotting Emod Against Erate For Each Structure
# for i in range (len(Chosen)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	plt.semilogx(Rate*RlxTime, Emod[i], 'o', linewidth=2, label=Structure[i])
# 	plt.ylabel('Elastic Modulus (MPa)')
# 	plt.xlabel('Wi')
# 	plt.legend(bbox_to_anchor=(0.05, 0.88, .3, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
# 	plt.xlim([0.01, 300])
# 	plt.ylim([np.min(Emod[i])-0.5, np.max(Emod[i])+0.5])
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "EmodWi-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)



# #For Plotting Max Emod Against Structure For Each Erate
# for i in range (len(Rate)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	x = np.array(range(0,len(Structure)))
# 	plt.xticks(x, Structure, rotation=45)
# 	plt.ylabel('Emod (MPa)')
# 	plt.xlabel('Structure')
# 	plt.scatter(x, Emod[:,i], linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[i]*RlxTime)+')')
# 	plt.legend(bbox_to_anchor=(0.60, 0.88, .99, .99), loc=3, ncol=legendCols, borderaxespad=0., fontsize = 'xx-small')
# 	plt.xlim([x[0]-1, x[len(x)-1]+1])
# 	minY   = np.min(Emod[:,i])
# 	maxY   = np.max(Emod[:,i])
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "EmodStruct-R" + str(i+1) + ".png"))
# 	# plt.show()
# 	plt.close(fig)




# #For Plotting Stress Against Strain For Each Structure
# for i in range (len(Time)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	for j in range (len(Time[i])):
# 		for k in range (len(Time[i,j])):
# 			# plt.plot(Strain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[0,j][0]/RlxTime)+')')
# 			plt.plot(Strain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label='Wi~' + '{:1.2f}'.format(Rate[j]*RlxTime))
# 	plt.ylim([np.min(np.min(MaxPxx[i]))-0.5, np.max(np.max(MaxPxx[i]))+0.05])
# 	plt.ylabel('Stress (MPa)')
# 	plt.xlabel('Strain')
# 	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 	plt.tight_layout()
# 	plt.xlim([0., 4.51])
# 	plt.xticks(rotation=45)
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "PxxStrain-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)



# # #For Plotting Stress Against Time For Each Structure
# for i in range (len(Time)):
# 	fig = plt.figure(i, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	for j in range (len(Time[i])):
# 		for k in range (len(Time[i,j])):
# 			# plt.plot(Time[i,j,k]*timeStep*1e-6, Pxx[i,j,k], '-', linewidth=2, label='Wi~O(' + '{:1.1f}'.format(Rate[0,j][0]/RlxTime)+')')
# 			plt.plot(Time[i,j,k]*timeStep*1e-6, Pxx[i,j,k], '-', linewidth=2, label='Wi~' + '{:1.2f}'.format(Rate[j]*RlxTime))
# 	plt.ylim([np.min(np.min(MaxPxx[i]))-0.5, np.max(np.max(MaxPxx[i]))+0.5])
# 	plt.ylabel('Stress (MPa)')
# 	plt.xlabel('Time (ns)')
# 	plt.xticks(rotation=45)
# 	plt.legend(bbox_to_anchor=(0.1, 0.88, 0.95, .995), loc=3, ncol=legendCols, borderaxespad=0., fontsize=6)
# 	plt.tight_layout()
# 	plt.xticks(rotation=45)
# 	plt.savefig(os.path.join(WriteFolderName, "PxxTime-Sp" + str(Chosen[i]) + ".png"))
# 	# plt.show()
# 	plt.close(fig)



# #For Comparing Stress Release Mechanism For Each Structure at each Wi
# for j in range (len(Time[0])):
# 	fig = plt.figure(j, figsize=(3.5, 3.5))
# 	plt.subplot(111)
# 	for i in range(len(Time)):
# 		for k in range(len(Time[i,j])):
# 			plt.plot(Strain[i,j,k], Pxx[i,j,k], '-', linewidth=2, label=Structure[i])
# 	plt.ylabel('Stress (MPa)')
# 	plt.xlabel('Strain')
# 	plt.xticks(rotation=45)
# 	plt.tight_layout()
# 	plt.legend(bbox_to_anchor=(0., 0.95, 1., .110), loc=1, ncol=legendCols, mode="expand", borderaxespad=0., fontsize = 6)
# 	# plt.xlim([0.4, 9.51])
# 	plt.tight_layout()
# 	plt.savefig(os.path.join(WriteFolderName, "PxxStrain-R" + str(j+1) + ".png"))
# 	# plt.show()
# 	plt.close(fig)
