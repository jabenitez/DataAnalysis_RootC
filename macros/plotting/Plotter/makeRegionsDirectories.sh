###### eMuPlotter running script
# Pass one of the following arguments to run the eMuPlotter over the sample 
# General (ttbar, Ztautau, Zprime_tautau_SSM*, Diboson), Wenu, Wmunu

DIRECTORY1=$1
DIRECTORY2=$2
SAMPLE1=$3
mkdir ${DIRECTORY2}

for region in `cat regions.txt`; do
    mkdir ${DIRECTORY2}/${region}
	cp ${DIRECTORY1}/${region}/NormalizedHistos_${SAMPLE1}.root ${DIRECTORY2}/${region}
        cp eMu_Plotter_MCandDATA.C ${DIRECTORY2}/${region}
	cp eMu_Plotter_MCandDATA.in ${DIRECTORY2}/${region}
	cd ${DIRECTORY2}/${region}
	mkdir myplots
	root -l -b -q eMu_Plotter_MCandDATA.C
	cd ../../
done    



