#!/bin/sh

samples="Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000 PythiaZmumu T1_McAtNlo"
#samples="Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000 PythiaWmunu PythiaZmumu T1_McAtNlo"
#samples="Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000"
#samples="Ms500_Mn200_Mg10000"
#for cuttype in htjets1000 htjets1200 htjets300  htjets400  htjets500  htjets600  htjets800 ; do
#for cuttype in zmass htcutzmass htleptons150 htleptons200 htleptons250 htleptons300 htleptons350 htleptons abcd htcut; do

#for cuttype in abcd htcut htcutzmass zmass abcdreqN  htcutreqN  htcutzmassreqN zmassreqN; do
for cuttype in zmass htcutzmass ; do
    outdirbase=/work/LS/histos/oct1/${cuttype}
    for runtype in  NL2NJ3 NL1NJ1 NL2NJ2 ; do
#    for runtype in NL1NJ2 ; do
	cp configs/lowpt/${cuttype}/${runtype}.config CutsLS.config
	for sample in $samples ; do 
	    outdir=${outdirbase}/${runtype}/${sample}/
	    logfile=logfile_hist_${runtype}_${sample}.log
	    mkdir -p ${outdir}
	    cp /work/VMshared/${sample}/input.txt input.txt
	    root -l -b -q run.C > logfile_hist_${runtype}_${sample}.log
	    mv llp_hist.root $outdir
	    cp CutsLS.config $outdir
	    mv $logfile $outdir
	done
    done
done
