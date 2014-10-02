#!/bin/sh

for mdir in `ls -d /Users/benitez/work/smp/rootfiles/d3pds/*` ; do
    upfile=`ls $mdir/Up/fhistos.root`
    nominal=`ls $mdir/Nominal/fhistos.root`
    downfile=`ls $mdir/Down/fhistos.root`
    run=`echo $upfile | cut -f8 -d/`
    ./compare_histos.sh -file=$upfile,ISR_Down -file=$nominal,ISR_Nominal -file=$downfile,ISR_Down -outdir=ISR/$run -force -nonorm

done