#!/bin/sh

indir=${1:-/work/LS/histos/may22/NL1NJ1/plots/}

samples="Ms1000_Mn600_Mg10000 PythiaWmunu PythiaZmumu T1_McAtNlo"
#samples="Groupedby3_0 Groupedby3_1 Groupedby3_2 Groupedby3_3"
htmlfile=${indir}/bkgcomparison.html
#htmlfile=${indir}/bkgcomparisonGR.html
rm -f ${htmlfile}

cat >> ${htmlfile} <<EOF
<html>
<head><title> Histo comparison </title>
 <style type="text/css">
div.centered 
{
text-align: center;
}
div.centered table 
{
margin: 0 auto; 
text-align: center;
}

div.centered td{
text-align:center;
font-size:20px;
}
</style>


</head>

<body>
<div class="centered">
<table>
EOF

echo "<tr>" >>  ${htmlfile}
for sample in $samples; do
    echo "<td>${sample}</td>" >>  ${htmlfile}
done
echo "</tr>" >>  ${htmlfile}

for variable in `cat HistogramsToCompare` ; do
    echo "<tr>" >>  ${htmlfile}
    for sample in $samples; do
	echo "<td><a href=\"${sample}/${variable}.eps\"><img src=\"${sample}/${variable}.gif\"></a></td>" >>  ${htmlfile} 
    done
    echo "</tr>" >>  ${htmlfile}
    echo "<tr>" >>  ${htmlfile}
    for sample in $samples; do
	echo "<td>${variable} - ${sample} </a></td>" >>  ${htmlfile} 
    done
    echo "</tr>" >>  ${htmlfile}
done
cat >> ${htmlfile} <<EOF 
</table>
</div>
</body>
</html>
EOF


