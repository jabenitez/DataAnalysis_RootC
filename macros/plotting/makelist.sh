#!/bin/sh

#sample=ttbar
sample=zee

for LG in 12.5 15.625 18.75 21.875 25.0 28.125 31.25 34.375 37.5 40.625 43.75 46.875 50.0 ;do 
    printf "./compare_histos.sh "
    for WS in 6.25 9.375 12.5 15.625 18.75 21.875 25.0 ; do 
	HG=`echo $LG + $WS | bc`
	f=`ls ~/work/trtsoft/histos/aug16/${sample}/*Histos*LG${LG}*HG${HG}*/*root`
	
	printf  " -file=%s,LG%f_HG%f " $f $LG $HG 

    done
    printf " --nonorm -force -outdir=aug18/${sample}/LG${LG}\n\n"
done

