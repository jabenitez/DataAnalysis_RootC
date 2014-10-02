#!/bin/sh

#samples="PythiaWmunu PythiaZmumu Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000"
#samples="Ms500_Mn200_Mg10000"
samples="T1_McAtNlo"
for runtype in NL1NJ1 NL2NJ2 NL3NJ2 ; do
#for runtype in NL1NJ1 ; do
    cp configs/${runtype}.config CutsLS.config
    for sample in $samples ; do 
	outdir=/work/LS/histos/may22/${runtype}/${sample}/
	logfile=logfile_hist_${runtype}_${sample}.log
	mkdir -p ${outdir}
	cp /work/VMshared/${sample}/input.txt input.txt
	root -l -b -q run.C > logfile_hist_${runtype}_${sample}.log
	mv llp_hist.root $outdir
	mv $logfile $outdir
    done
done
