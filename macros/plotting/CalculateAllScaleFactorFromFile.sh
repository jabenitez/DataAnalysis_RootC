#!/bin/sh

#samples=(Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000 PythiaWmunu PythiaZmumu T1_McAtNlo)
#xsecs=(162 162 162 1.4 1.4 1.4 3.0 18.6 8 8701700 834700 168000 )

samples=(Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000 PythiaZmumu T1_McAtNlo)
xsecs=(162 162 162 1.4 1.4 1.4 3.0 18.6 8 834700 168000 )

arraysize=${#samples[@]}
mysize=$((${arraysize}-1))

for i in `seq 0 ${mysize}` ; do
    
    file=`ls /work/LS/histos/may23/NL1NJ1/*${samples[i]}*/*root`
    file=`ls /work/LS/histos/oct1/htcutzmass/NL2NJ2/*${samples[i]}*/*root`
    if [ "x$file" != "x" ]; then

	sf=`root -l -b -q 'CalculateScaleFactorFromFile.C("'$file'",'${xsecs[i]}')'  | grep "Scale factor" | awk '{print $3}'`
	echo ${samples[i]} ${xsecs[i]} ${sf}
    fi
done


