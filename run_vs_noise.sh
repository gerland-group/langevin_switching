#!/bin/bash

# This script executes loops to run the simulations for specified signalling scenarios
# at a variety of noise levels

function set_parameters() {
	sed -e s/_D/$1/ -e s/_P/$2/ -e s/_X/$3/ -e s/_T/$4/ -e s/_N/$5/ ../params.noise.template > params.in
}

rm job_list
make -C src

X_array=(0.18 0.9 1 1 100 0.7 0)
T_array=(8    6   3 9 6   5 0)
P_array=(0    7   7 7 7   7 0)
D_array=(0    1   0 0 0   1 0)
N_array=(3162300 2371400 1778300 1333500 1000000 749890 562340 421700 316230 237140 177830 133350 100000 74989 56234 42170 31623 23714 17783 13335 10000 7498.9 5623.4 4217.0 3162.3 2371.4 1778.3 1333.5 1000 749.9 562.3 421.7 316.2 237.1 177.8 133.4 100.0 74.989 56.234 42.170 31.623 23.714 17.783 13.335 10 7.4989 5.6234 4.2170 3.1623 2.3714 1.7783 1.3335 1.0)

job_counter=1
for (( i=0; i<${#X_array[@]}; i++ )); do
	for (( j=0; j<${#N_array[@]}; j++ )); do
		jobdir="job_${job_counter}"
		echo $job_counter $i $j >> job_list
		mkdir $jobdir
		cd $jobdir
		set_parameters ${D_array[$i]} ${P_array[$i]} ${X_array[$i]} ${T_array[$i]} ${N_array[$j]}
		../src/langevin -t
		let job_counter=$(($job_counter+1))
		cd ..
	done
done

