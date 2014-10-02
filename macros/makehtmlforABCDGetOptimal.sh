#!/bin/sh

samples="PythiaWmunu PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
#samples="PythiaWmunu PythiaZmumu T1_McAtNlo Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000"
#samples="Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000"
#samples="Ms500_Mn200_Mg10000"
plots="hHtAllJetsAllLeptons_LowBetaMuon1Eta2DBkg.png hHtAllJetsAllLeptons_LowBetaMuon1Eta2DSBRatio.png hHtAllJetsAllLeptons_LowBetaMuon1Eta2DSig.png  hHtAllJetsAllLeptons_LowBetaMuon1Eta2DRatio.png hHtAllJetsAllLeptons1DBackground.png hHtAllJetsAllLeptons1DSignal.png hHtAllJetsAllLeptons1DSignalOverSqrtSignalBackground.png hMuLowBetaMuon1EtaAbs1DBackground.png hMuLowBetaMuon1EtaAbs1DSignal.png hMuLowBetaMuon1EtaAbs1DSignalOverSqrtSignalBackground.png"

plots="hHtAllJetsAllLeptons_LowBetaMuon1Eta2DBkg.png hHtAllJetsAllLeptons_LowBetaMuon1Eta2DSig.png hHtAllJetsAllLeptons_LowBetaMuon1Eta2DRatio.png hHtAllJetsAllLeptons_LowBetaMuon1Eta2DSBRatio.png hHtAllJetsAllLeptons1DBackground.png hHtAllJetsAllLeptons1DSignal.png hHtAllJetsAllLeptons1DSignalOverSqrtSignalBackground.png hHtAllJetsAllLeptons1DSignalOverSqrtBackground.png hMuLowBetaMuon1EtaAbs1DBackground.png hMuLowBetaMuon1EtaAbs1DSignal.png hMuLowBetaMuon1EtaAbs1DSignalOverSqrtSignalBackground.png hMuLowBetaMuon1EtaAbs1DSignalOverSqrtBackground.png"

cat > index.html <<EOF
<html>
<head></head>
<body>
<table border="2">
EOF


#for runtype in NL1NJ1 NL2NJ2 NL3NJ2 NL2NJ3 NL3NJ3 NL3NJ4 ; do
for runtype in  NL2NJ3 NL3NJ3 ; do

    echo "<tr><td>$runtype </td>" >> index.html
    echo "<td><pre>" >> index.html
    cat ${runtype}/logfile_2DRatio_${runtype}.log | grep -v Processing >> index.html
    echo "</pre></td>" >> index.html

    for plot in  $plots ; do 
	echo "<td><img src=\"${runtype}/${plot}\"></td>" >> index.html
    done

    echo "</tr>" >> index.html
done


cat >> index.html <<EOF

</table></body></html>


EOF