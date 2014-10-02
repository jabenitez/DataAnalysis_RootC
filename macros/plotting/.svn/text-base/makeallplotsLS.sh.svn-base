#!/bin/sh
#samplesAll="PythiaWmunu PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
#scalesAll="23.9148 2.61961 0.194426 0.14256 0.14256 0.14256 0.001232 0.001232 0.001232 0.00264 0.016368 0.00704"
samplesAll="PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
scalesAll="/work/LS/histos/oct1/htcutzmass/NL2NJ2/2.61961 0.194426 0.14256 0.14256 0.14256 0.001232 0.001232 0.001232 0.00264 0.016368 0.00704"

#samples[0]="PythiaWmunu PythiaZmumu T1_McAtNlo"
#scales[0]="23.9148 2.61961 0.194426"
#samples[4]="PythiaWmunu PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn600_Mg2000"
#scales[4]="23.9148 2.61961 0.194426 0.14256 0.001232 0.016368"
#samples[5]="PythiaWmunu PythiaZmumu T1_McAtNlo Ms1000_Mn600_Mg10000 Ms1000_Mn600_Mg2000"
#scales[5]="23.9148 2.61961 0.194426 0.001232 0.016368"

samples[0]="PythiaZmumu T1_McAtNlo"
scales[0]="2.69462 0.5376"
samples[1]="Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000"
scales[1]="0.010368 0.010368 0.010368"
samples[2]="Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000"
scales[2]="0.0000896 0.0000896 0.0000896"
samples[3]="Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
scales[3]="0.000192 0.00119046 0.000512"
samples[4]="PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn600_Mg2000"
scales[4]="2.69462 0.5376 0.010368 0.0000896 0.00119046"
samples[5]="PythiaZmumu T1_McAtNlo Ms1000_Mn600_Mg10000 Ms1000_Mn600_Mg2000"
scales[5]="2.69462 0.5376 0.0000896 0.00119046"

#for cuttype in abcd htcutetacut ; do
#for cuttype in abcd htcut htcutzmass zmass abcdreqN  htcutreqN  htcutzmassreqN zmassreqN; do
#for cuttype in abcd htcut htcutzmass zmass ; do
#for cuttype in abcd htcut htcutzmass zmass abcdreqN  htcutreqN  htcutzmassreqN zmassreqN; do
for cuttype in zmass htcutzmass ; do
    mdir=/work/LS/histos/oct1/${cuttype}
    for runtype in  NL2NJ3 NL1NJ1 NL2NJ2 ; do
#    for runtype in NL1NJ2 ; do
#    for runtype in NL2NJ3 NL1NJ1 NL1NJ2 NL1NJ3 NL2NJ1 NL2NJ2 ; do
#    for runtype in NL2NJ3 NL2NJ2 ; do
	j=0
	for sample in ${samplesAll}; do
	    j=$((j+1))
	    scale=`echo ${scalesAll} | cut -d ' ' -f$j`
	    file=`ls ${mdir}/${runtype}/*${sample}*/*root`
	    if [ "x$file" != "x" ]; then
		printf "./compare_histos.sh " 
		printf " -file=%s,%s,%s " "$file" "$sample" "$scale"
		printf " -outdir=${mdir}/${runtype}/plots/${sample} -force\n"
	    fi
	done
	
	for i in 0 1 2 3 4 5; do
	    printf "./compare_histos.sh "
	    j=0
	    for sample in ${samples[$i]}; do
		j=$((j+1))
		scale=`echo ${scales[$i]} | cut -d ' ' -f$j`
		file=`ls ${mdir}/${runtype}/*${sample}*/*root`
		if [ "x$file" != "x" ]; then
		    printf " -file=%s,%s,%s " "$file" "$sample" "$scale"
		fi
	    done
	    printf " -outdir=${mdir}/${runtype}/plots/Groupedby3_${i} -force\n"
	done
    done
done
