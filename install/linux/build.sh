#!/bin/bash

#Copy binary file from build directory to source install
cp ~/build/SaliSynth/build-saliSynthMusic-Desktop_Qt_5_14_0_GCC_64bit-Release/saliSynthMusic ~/saliSynthMusic/saliSynthMusic

#Pack source install directory to archive
~/Qt/Tools/QtInstallerFramework/3.1/bin/archivegen  packages/SaliSynth/data/sali ~/saliSynthMusic/*

#Convert archive to binary install file
~/Qt/Tools/QtInstallerFramework/3.1/bin/binarycreator -c config/config.xml -p packages SaliSynth_0_47_Linux_x86_64_Install

#Remove archive because it integrated into binary install file
#rm packages/SaliSynth/data/sali.7z
