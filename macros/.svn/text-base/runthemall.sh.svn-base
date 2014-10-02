#!/bin/sh

g=10000
outdir=/work/LS/histos/may21/NL1NJ1/
sq=(500 500 500 1000 1000 1000 1500 1000 10000)
n=(200 600 1000 200 600 1000 600 600 600)
g=(10000 10000 10000 10000 10000 10000 1200 2000 1000)
for i in 0 1 2 3 4 5 6 7 8 ; do
#for i in 6 ; do
    echo $i
    echo Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}
#   continue
    rm -f runD3PDSelector.C
    sed "s/MYFILENAME/Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}/"  runD3PDSelectorBase.C > runD3PDSelector.C
    root -l -b -q runD3PDSelector.C > logfile_hist_Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}.log
    mv llp_hist.root llp_hist_Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}.root

    echo logfile_hist_Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}.log
    tail -25 logfile_hist_Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}.log | grep Cut >> templog
    mv templog cutflow_hist_Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}.log
    mkdir -p ${outdir}/Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}
    mv *root *log ${outdir}/Ms${sq[i]}_Mn${n[i]}_Mg${g[i]}
done
#mkdir -p /Users/benitez/work/LS/histos/mar26/$1/
#mv *root *log /Users/benitez/work/LS/histos/mar26/$1/



