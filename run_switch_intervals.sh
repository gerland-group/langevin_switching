#!/bin/bash

# This script executes loops to run long simulations outputting the switching times
# for the signals affecting all reaction parameters in turn, with different signal
# amplitudes

function set_parameters() {
	sed -e s/_D/$1/ -e s/_P/$2/ -e s/_X/$3/ ../params.switch.template > params.in
}

rm job_list
make -C src

Xup_array=(0 0.1 0.133352 0.177828 0.237137 0.316228 0.421697 0.562341 0.749894 1. 1.33352 1.77828 2.37137 3.16228 4.21697 5.62341 7.49894 10. 13.3352 17.7828 23.7137 31.6228 42.1697 56.2341 74.9894 100.)
Xdn_array=(0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5 0.55 0.6 0.65 0.7 0.75 0.8 0.85 0.9 0.95 1.0)

job_counter=1
for D in 0; do
	for P in 0 1 2 3 4 5 6 7 8 9 10; do
		for (( X=0; X<${#Xup_array[@]}; X++ )); do
			jobdir="job_${job_counter}"
			echo $job_counter $D $P $X >> job_list
			mkdir $jobdir
			cd $jobdir
			set_parameters $D $P ${Xup_array[$X]}
			../src/langevin -s
			let job_counter=$(($job_counter+1))
			cd ..
		done
	done
done

for D in 1; do
	for P in 0 1 2 3 4 5 6 7 8 9 10; do
		for (( X=0; X<${#Xdn_array[@]}; X++ )); do
			jobdir="job_${job_counter}"
			echo $job_counter $D $P $X >> job_list
			mkdir $jobdir
			cd $jobdir
			set_parameters $D $P ${Xdn_array[$X]}
			../src/langevin -s
			let job_counter=$(($job_counter+1))
			cd ..
		done
	done
done
