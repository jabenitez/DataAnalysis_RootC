#!/bin/sh

#samples="AMSB_LL01 AMSB_LL02 AMSB_LL03 AMSB_LL11 AMSB_LL12 AMSB_LL13 J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J4_pythia J5_pythia J6_pythia  J7_pythia J8_pythia PythiaWenu  PythiaWmunu PythiaZee PythiaZmumu T1_McAtNlo st_tchan_enu"

#samples="J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J5_pythia J6_pythia J7_pythia J8_pythia PythiaWenu PythiaWmunu PythiaZee PythiaZmumu T1_McAtNlo st_tchan_enu"
samples="Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"

for runtype in NL2NJ2 ; do
#for runtype in NL1NJ1 NL2NJ2 ; do
    for sample in $samples; do
	file=`ls /work/LS/histos/may17/${runtype}/*${sample}*/*root`
	if [ "x$file" != "x" ]; then
	    printf "./compare_histos.sh " 
	    printf " -file=%s,%s " "$file" "$sample"
	    printf " -outdir=/work/LS/histos/may17/${runtype}/plots/${sample} -force\n"
	fi
    done
done


