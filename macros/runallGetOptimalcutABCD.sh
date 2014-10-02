#!/bin/sh

samples="PythiaWmunu PythiaZmumu T1_McAtNlo Ms500_Mn200_Mg10000 Ms500_Mn600_Mg10000 Ms500_Mn1000_Mg10000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1500_Mn600_Mg1200 Ms1000_Mn600_Mg2000 Ms10000_Mn600_Mg1000"
#samples="PythiaWmunu PythiaZmumu T1_McAtNlo Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000"
#samples="Ms10000_Mn600_Mg1000 Ms1000_Mn200_Mg10000 Ms1000_Mn600_Mg2000  Ms500_Mn1000_Mg10000 Ms500_Mn600_Mg10000 Ms1000_Mn1000_Mg10000 Ms1000_Mn600_Mg10000 Ms1500_Mn600_Mg1200  Ms500_Mn200_Mg10000"
#samples="Ms500_Mn200_Mg10000"

#for seltype in abcd        htcut       htcutetacut ; do 
for seltype in abcd htcutetacut ; do 
#for runtype in NL1NJ1 NL2NJ2 NL2NJ3 NL3NJ3 ; do
for runtype in NL2NJ3 NL3NJ3 ; do

    mdir=/work/LS/histos/jun14/${seltype}/${runtype}/
    outdir=Trigger/jun14/${seltype}/${runtype}/
    logfile=logfile_2DRatio_${runtype}.log
    
    root -l -b -q 'plotRatioSignalBackground2D.C("'${mdir}'/Ms500_Mn200_Mg10000/llp_hist.root","'${mdir}'/T1_McAtNlo/llp_hist.root")' > $logfile
    
    root -l -b -q 'plotRatioSignalBackground.C("'${mdir}'/Ms500_Mn200_Mg10000/llp_hist.root","'${mdir}'/T1_McAtNlo/llp_hist.root")' >> $logfile
    
    root -l -b -q 'plotRatioSignalBackground.C("'${mdir}'/Ms500_Mn200_Mg10000/llp_hist.root","'${mdir}'/T1_McAtNlo/llp_hist.root","hMuLowBetaMuon1EtaAbs")' >> $logfile
    
#	root -l -b -q 'plotRatioSignalBackground.C("'${mdir}'/Ms500_Mn200_Mg10000/llp_hist.root","'${mdir}'/T1_McAtNlo/llp_hist.root","hJet1pT")'  >> "pt"${logfile}
    
#	root -l -b -q 'plotRatioSignalBackgroundEta.C("'${mdir}'/Ms500_Mn200_Mg10000/llp_hist.root","'${mdir}'/T1_McAtNlo/llp_hist.root")' >> $logfile

    mkdir -p ${outdir}
    mv *png ${logfile} ${outdir}
    
#    done
done
done
