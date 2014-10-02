#!/bin/sh

#samples="AMSB_LL01 AMSB_LL02 AMSB_LL03 AMSB_LL11 AMSB_LL12 AMSB_LL13 J0_pythia J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J4_pythia J5_pythia J5_pythia J6_pythia J6_pythia J7_pythia J7_pythia J8_pythia PythiaWenu PythiaWenu PythiaWmunu PythiaZee PythiaZmumu PythiaZmumu PythiaZmumu T1_McAtNlo T1_McAtNlo st_tchan_enu"
samples="PythiaWmunu PythiaZmumu T1_McAtNlo Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200 Ms500_Mn200_Mg10000"
cutname="nocut NJets JetspT NLeptons NLowBetaMu Trigger AJALHtCut LowBMu1Eta ZMassCut ReqNeutralino HtAllJets HtAllLeptons"

date=Aug1_nLBM
#for seltype in abcd        htcut       htcutetacut ; do
#for cuttype in htjets1000 htjets1200 htjets300  htjets400  htjets500  htjets600  htjets800 zmass htcutzmass abcd htcut htleptons htleptons150 htleptons200 htleptons250 htleptons300 htleptons350 ; do
for cuttype in zmass htcutzmass htleptons350 ; do
#for cuttype in abcd htcutetacut ; do
for runtype in NL1NJ2 NL2NJ3 ; do
    mdir=/work/LS/histos/${date}/${cuttype}/${runtype}/
    outdir=${date}/${cuttype}/${runtype}
    echo OUTDIR RRRR ${outdir}
    mkdir -p ${outdir}
    htmlfile="index.html"
    echo htmlfile ${htmlfile}
    
    cat > ${htmlfile} <<EOF
<html><head><title>${runtype}  efficiencies </title></head>
<body>
EOF

    echo "<h2>Cut Efficiencies for ${runtype} - Individual </h2>" >> ${htmlfile}
    echo "<table border="2"></tr><td>SAMPLE</td>" >> ${htmlfile}
    
    for cut in $cutname ; do
	echo "<td>${cut}</td> " >> ${htmlfile}
    done
    echo "</tr><tr>" >> ${htmlfile}
    for sample in ${samples} ; do
	echo "<td> ${sample} </td>" >> ${htmlfile}
	
	myfile=`ls ${mdir}/${sample}/*root `
	echo $myfile
	root -b -l -q 'GetCutEfficiencies.C("'${myfile}'")'   > runlog
	for cut in $cutname ; do
	    relative=`cat runlog | grep Rel | grep ${cut}| awk '{print $2}'` 
	    echo "<td>${relative}</td> " >> ${htmlfile}
	done
	echo "</tr>" >> ${htmlfile}
    done
	echo "</table>" >> ${htmlfile}


    echo "<h2>Cut Efficiencies for ${runtype} - Relative to previous cut </h2>" >> ${htmlfile}
    echo "<table border="2"></tr><td>SAMPLE</td>" >> ${htmlfile}
    
    for cut in $cutname ; do
	echo "<td>${cut}</td> " >> ${htmlfile}
    done
    echo "</tr><tr>" >> ${htmlfile}
    for sample in ${samples} ; do
	echo "<td> ${sample} </td>" >> ${htmlfile}
	
	myfile=`ls ${mdir}/${sample}/*root `
	echo $myfile
	root -b -l -q 'GetCutEfficiencies.C("'${myfile}'")'   > runlog
	for cut in $cutname ; do
	    relative=`cat runlog | grep Rel | grep ${cut}| awk '{print $4}'` 
	    echo "<td>${relative}</td> " >> ${htmlfile}
	done
	echo "</tr>" >> ${htmlfile}
    done
    echo "</table>" >> ${htmlfile}


    cat >> ${htmlfile} <<EOF
</body></html>

EOF

mv *html ${outdir}
done

done

#Processing GetCutEfficiencies.C("/work/LS/histos/may29ht940/NL3NJ3/Ms500_Mn200_Mg10000/llp_hist.root")...
#          NJets:     95.23999786     Rel:     95.23999786
#         JetspT:     94.55999756     Rel:     90.73999786
#       NLeptons:     40.74000168     Rel:     35.20000076
#     NLowBetaMu:     63.45999908     Rel:     30.36000061
#        Trigger:     76.90000153     Rel:     26.45999908
#      AJALHtCut:     71.86000061     Rel:     25.00000000
#     LowBMu1Eta:    100.00000000     Rel:     25.00000000
