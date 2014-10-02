#source /afs/cern.ch/atlas/offline/external/GRID/DA/panda-client/latest/etc/panda/panda_setup.sh
#source /afs/cern.ch/project/gd/LCG-share/current/etc/profile.d/grid_env.sh     
#voms-proxy-init --voms atlas -rfc --valid 96:00
#!/bin/sh


#sample=J0


inputsmc=( \
group.phys-susy.mc11_7TeV.105017.J8_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105014.J5_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105011.J2_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.106043.PythiaWenu_no_filter.SUSYLLP.e815_s1272_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105015.J6_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105013.J4_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105010.J1_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.106046.PythiaZee_no_filter.SUSYLLP.e815_s1272_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105012.J3_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.117360.st_tchan_enu_AcerMC.SUSYLLP.e835_s1310_s1300_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.106044.PythiaWmunu_no_filter.SUSYLLP.e815_s1272_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105016.J7_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105200.T1_McAtNlo_Jimmy.SUSYLLP.e835_s1272_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.105009.J0_pythia_jetjet.SUSYLLP.e815_s1273_s1274_r3043.170643_0123_120224/ \
group.phys-susy.mc11_7TeV.106047.PythiaZmumu_no_filter.SUSYLLP.e815_s1272_s1274_r3043.170643_0123_120224/ \
) 


inputs=( \
group.phys-susy.data11_7TeV.periodB.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodD.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodE.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodF.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodG.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodH.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodI.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodJ.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodK.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodL.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
group.phys-susy.data11_7TeV.periodM.physics_Muons.PhysCont.SUSYLLP_SGLMU.pro10_v01.170643_0123_120224/ \
)

NJ=3
NL=3


data=1

#for i in `seq 1 28` ; do
arraysize=${#inputs[@]}
mysize=$((${arraysize}-1))

#for cuttype in abcd  htcutetacut ; do
for cuttype in abcd  ; do
for i in `seq 0 ${mysize}` ; do

echo $i
echo ${inputs[i]}
if [ $data -eq 1 ]; then
sample=`echo ${inputs[i]} | tr "." " " | awk '{print $4}'`
nFilesPerJob=120

else
sample=`echo ${inputs[i]} | tr "." " " | awk '{print $5}'`
nFilesPerJob=20

fi

echo $sample
mjobid=Jul4Sel${cuttype}DATA${sample}NL${NL}NJ${NJ}
#mjobid=Feb20Sel${sample}NL2NJ1
outputDS=user.benitez.${sample}${mjobid}.LeptosusyHistos/
echo prun --exec "echo %IN | sed 's/,/\n/g' > input.txt; cp configs/jetpt/${cuttype}/NL${NL}NJ${NJ}.config  CutsLS.config ; root.exe -b -q run.C" --athenaTag=17.0.4.5 --inDS ${inputs[i]} --outDS ${outputDS}  --nFilesPerJob ${nFilesPerJob} --mergeOutput --outputs llp_hist.root --match "*root*"
prun --exec "echo %IN | sed 's/,/\n/g' > input.txt; cp configs/jetpt/${cuttype}/NL${NL}NJ${NJ}.config  CutsLS.config ; root.exe -b -q run.C" --athenaTag=17.0.4.5 --inDS ${inputs[i]} --outDS ${outputDS}  --nFilesPerJob ${nFilesPerJob} --mergeOutput --outputs llp_hist.root --match "*root*"
sleep 5

done
done
echo $arraysize
echo $mysize