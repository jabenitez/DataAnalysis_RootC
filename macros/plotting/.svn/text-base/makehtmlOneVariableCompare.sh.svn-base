#!/bin/sh

indir=${1:-/work/LS/histos/may17/NL1NJ1/plots/}
variable=${2:-hNOkJets}

samples="J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J5_pythia J6_pythia J7_pythia J8_pythia PythiaWenu PythiaWmunu PythiaZee PythiaZmumu T1_McAtNlo st_tchan_enu"
htmlfile=${indir}/${variable}.html
rm -f ${htmlfile}

cat >> ${htmlfile} <<EOF
<html>
<head><title>${variable}</title></head>
<body>
<p align=center>
EOF
for sample in $samples; do
    echo "<a href=\"${sample}/${variable}.eps\"><img src=\"${sample}/${variable}.gif\"></a><br>" >>  ${htmlfile} 
done
cat >> ${htmlfile} <<EOF 
</p>
</body>
</html>
EOF


