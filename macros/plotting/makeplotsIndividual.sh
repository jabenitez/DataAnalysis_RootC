#!/bin/sh

#samples="AMSB_LL01 AMSB_LL02 AMSB_LL03 AMSB_LL11 AMSB_LL12 AMSB_LL13 J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J4_pythia J5_pythia J6_pythia  J7_pythia J8_pythia PythiaWenu  PythiaWmunu PythiaZee PythiaZmumu T1_McAtNlo st_tchan_enu"

#samples="J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J5_pythia J6_pythia J7_pythia J8_pythia PythiaWenu PythiaWmunu PythiaZee PythiaZmumu T1_McAtNlo st_tchan_enu"
#samples="Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"

samples[0]="PythiaWmunu PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
scales[0]="23.9148 2.61961 0.194426 0.14256 0.14256 0.14256 0.001232 0.001232 0.001232 0.00264 0.016368 0.00704"

mdir="/work/LS/histos/jun12/abcd"
for runtype in NL1NJ1 NL2NJ2 NL2NJ3 NL3NJ3 ; do
    for i in 0 ; do
	j=0
	for sample in ${samples[$i]}; do
	    j=$((j+1))
	    scale=`echo ${scales[$i]} | cut -d ' ' -f$j`
	    file=`ls ${mdir}/${runtype}/*${sample}*/*root`
	    if [ "x$file" != "x" ]; then
		printf "./compare_histos.sh " 
		printf " -file=%s,%s,%s " "$file" "$sample" "$scale"
		printf " -outdir=${mdir}/${runtype}/plots/${sample} -force\n"
	    fi
	done
    done
done
