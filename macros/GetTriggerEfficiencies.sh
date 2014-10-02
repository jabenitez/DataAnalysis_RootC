#!/bin/sh

#samples="AMSB_LL01 AMSB_LL02 AMSB_LL03 AMSB_LL11 AMSB_LL12 AMSB_LL13 J0_pythia J0_pythia J1_pythia J2_pythia J3_pythia J4_pythia J4_pythia J5_pythia J5_pythia J6_pythia J6_pythia J7_pythia J7_pythia J8_pythia PythiaWenu PythiaWenu PythiaWmunu PythiaZee PythiaZmumu PythiaZmumu PythiaZmumu T1_McAtNlo T1_McAtNlo st_tchan_enu"
#samples="PythiaWmunu PythiaZmumu T1_McAtNlo Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200 Ms500_Mn200_Mg10000"
samples="PythiaWmunu PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
signals="Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
backgrounds="PythiaWmunu PythiaZmumu T1_McAtNlo"
scales="23.9148 2.61961 0.194426 0.14256 0.14256 0.14256 0.001232 0.001232 0.001232 0.00264 0.016368 0.00704"

triggername="EF_mu22_medium EF_2mu10_loose EF_mu18_medium EF_mu40_slow OrAllTriggers ORMuonTriggers EF_mu22_medium_OR_EF_mu18_medium EF_mu22_medium_OR_EF_2mu10_loose EF_mu18_medium_OR_EF_2mu10_loose"


date=jun14

#for seltype in abcd       ; do
#for runtype in NL2NJ3  ; do
#    mdir=/work/LS/histos/jun6/${seltype}/${runtype}/
#    for sample in ${samples} ; do
#	myfile=`ls ${mdir}/${sample}/*root `
#	echo $myfile
#	root -b -l -q 'GetTriggerEfficiencies.C("'${myfile}'")'  
#    done
#done
#done


#for seltype in abcd        htcut       htcutetacut ; do
for seltype in abcd htcutetacut ; do
for runtype in NL2NJ3 NL3NJ3 ; do
#for runtype in NL2NJ3 ; do
    mdir=/work/LS/histos/${date}/${seltype}/${runtype}/
    outdir=Trigger/${date}/${seltype}/
    mkdir -p $outdir
    echo OUTDIR RRRR ${outdir}
    htmlfile="${runtype}TriggerTable.html"
    echo htmlfile ${htmlfile}
    
    cat > ${htmlfile} <<EOF
<html><head><title>${runtype}  efficiencies </title></head>
<body>
EOF

    echo "<h2>Trigger Efficiencies for ${runtype} - No Cuts </h2>" >> ${htmlfile}
    echo "<table border="2"></tr><td>SAMPLE</td>" >> ${htmlfile}
    
    for trigger in $triggername ; do
	echo "<td>${trigger}</td> " >> ${htmlfile}
    done
    echo "</tr><tr>" >> ${htmlfile}
    for sample in ${samples} ; do
	echo "<td> ${sample} </td>" >> ${htmlfile}
	
	myfile=`ls ${mdir}/${sample}/*root `
	echo $myfile
	root -b -l -q 'GetTriggerEfficiencies.C("'${myfile}'")'   > runlog
	for trigger in $triggername ; do
	    relative=`cat runlog | grep -w ${trigger}| awk '{print $2}'` 
	    echo "<td>${relative}</td> " >> ${htmlfile}
	done
	echo "</tr>" >> ${htmlfile}
    done
	echo "</table>" >> ${htmlfile}


    echo "<h2>Trigger Efficiencies for ${runtype} - All Cuts Included </h2>" >> ${htmlfile}
    echo "<table border="2"></tr><td>SAMPLE</td>" >> ${htmlfile}
    
    for trigger in $triggername ; do
	echo "<td>${trigger}</td> " >> ${htmlfile}
    done
    echo "</tr><tr>" >> ${htmlfile}
    for sample in ${samples} ; do
	echo "<td> ${sample} </td>" >> ${htmlfile}
	myfile=`ls ${mdir}/${sample}/*root `
	echo $myfile
	root -b -l -q 'GetTriggerEfficiencies.C("'${myfile}'")'   > runlog
	for trigger in $triggername ; do
	    relative=`cat runlog | grep -w ${trigger}| awk '{print $4}'` 
	    echo "<td>${relative}</td> " >> ${htmlfile}
	done
	echo "</tr>" >> ${htmlfile}
    done
    echo "</table>" >> ${htmlfile}

    echo "<h2>NEvents Scaled ${runtype} - All Cuts Included </h2>" >> ${htmlfile}
    echo "<table border="2"></tr><td>SAMPLE</td>" >> ${htmlfile}
    
    for trigger in $triggername ; do
	echo "<td>${trigger}</td> " >> ${htmlfile}
    done
    echo "</tr><tr>" >> ${htmlfile}
    j=0
    for sample in ${samples} ; do
	j=$((j+1))
	echo "<td> ${sample} </td>" >> ${htmlfile}
	
	myfile=`ls ${mdir}/${sample}/*root `
	echo $myfile
	root -b -l -q 'GetTriggerEfficiencies.C("'${myfile}'")'   > runlog
	for trigger in $triggername ; do
	    relative=`cat runlog | grep -w ${trigger}| awk '{print $8}'` 
            scale=`echo ${scales} | cut -d ' ' -f$j`
	    relscale=$(echo | awk '{print '${scale}'*'${relative}'}' )
	    echo "<td>${relscale}</td> " >> ${htmlfile}
	    eval "${trigger}${sample}=${relscale}"
	done
	echo "</tr>" >> ${htmlfile}
    done
    echo "</table>" >> ${htmlfile}



    echo "<h2>Signal Over Background ratio ${runtype} </h2>" >> ${htmlfile}
    echo "<table border="2"></tr><td>SAMPLE</td>" >> ${htmlfile}
    
    for trigger in $triggername ; do
	echo "<td>${trigger}</td> " >> ${htmlfile}
    done
    echo "</tr><tr>" >> ${htmlfile}
    for sample in ${signals} ; do
	echo "<td> ${sample} </td>" >> ${htmlfile}
	for trigger in $triggername ; do
	    bkgtotal=0
	    for bkg in ${backgrounds} ; do
		varn=${trigger}${bkg}
#		echo ${varn}
		eval "addthis=${!varn}"
#		echo addthis ${addthis} bkgtotal ${bkgtotal}
		bkgtotal=$(echo | awk '{print '${bkgtotal}'+'${addthis}'}' ) 
	    done
	    varn=${trigger}${sample}
	    eval "signal=${!varn}"
#	    echo trigger ${trigger} sample ${sample} signal ${signal} bkg ${bkgtotal}
	    ratio=$(echo | awk '{print '${signal}'/'${bkgtotal}'}' )
	    echo "<td>${ratio}</td> " >> ${htmlfile}
	    eval "${trigger}${sample}=${relscale}"
	done
	echo "</tr>" >> ${htmlfile}
    done
    echo "</table>" >> ${htmlfile}


    echo "<h2>Trigger Efficiencies for ${runtype} - Relative to previous cut </h2>" >> ${htmlfile}
    echo "<table border="2"></tr><td>SAMPLE</td>" >> ${htmlfile}
    
    for trigger in $triggername ; do
	echo "<td>${trigger}</td> " >> ${htmlfile}
    done
    echo "</tr><tr>" >> ${htmlfile}
    for sample in ${samples} ; do
	echo "<td> ${sample} </td>" >> ${htmlfile}
	
	myfile=`ls ${mdir}/${sample}/*root `
	echo $myfile
	root -b -l -q 'GetTriggerEfficiencies.C("'${myfile}'")'   > runlog
	for trigger in $triggername ; do
	    relative=`cat runlog | grep -w ${trigger}| awk '{print $6}'` 
	    echo "<td>${relative}</td> " >> ${htmlfile}
	done
	echo "</tr>" >> ${htmlfile}
    done
    echo "</table>" >> ${htmlfile}


    cat >> ${htmlfile} <<EOF
</body></html>

EOF


done
mv *html ${outdir}
done
