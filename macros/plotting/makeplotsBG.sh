#!/bin/sh

#samples="AMSB_LL01 AMSB_LL02 AMSB_LL03 AMSB_LL11 AMSB_LL12 AMSB_LL13 J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J4_pythia J5_pythia J6_pythia  J7_pythia J8_pythia PythiaWenu  PythiaWmunu PythiaZee PythiaZmumu T1_McAtNlo st_tchan_enu"
samples="J0_pythia PythiaW PythiaZ st_tchan signal"

#runtype=NL1NJ1
for runtype in NL1NJ1 NL1NJ2 NL2NJ1 NL2NJ2 ; do
printf "./compare_histos.sh " 
    for sample in $samples ; do
    file=`ls /work/LS/histos/${runtype}/*${sample}*/*root`
    printf " -file=%s,%s " "$file" "$sample"
    done
printf " -outdir=/work/LS/histos/${runtype}/plots/ -force\n"
done


