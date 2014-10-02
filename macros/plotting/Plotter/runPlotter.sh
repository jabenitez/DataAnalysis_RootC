###### eMuPlotter running script
# Pass one of the following arguments to run the eMuPlotter over the sample 
# General (ttbar, Ztautau, Zprime_tautau_SSM*, Diboson), QCD_J*, WenuNp*, WmunuNp*

SAMPLE=$1
Directory=$2
mkdir ${Directory}

for region in `cat regions.txt`; do
    cp eMuPlotter_${SAMPLE}_SAFE.in eMuPlotter.in
    mkdir ${Directory}/${region}
    sed -i .bak s/REGION/${region}/g eMuPlotter.in
    echo    sed -i .bak s/REGION/${region}/g eMuPlotter.in
    rm *.bak
    root -l -b -q eMuPlotter.C
    mv NormalizedHistos.root ${Directory}/${region}/NormalizedHistos_${SAMPLE}.root
    mv ProbabilityHistos.root ${Directory}/${region}/ProbabilityHistos_${SAMPLE}.root
    mv output.log ${Directory}/${region}/output_${SAMPLE}.log
done    



