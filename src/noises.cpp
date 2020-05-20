/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file defines the functions to calculate the noise terms in the model.
 */

#include "noises.h"

double noiseA (
	const state& conc_prev,
	const params_array& params,
	const int p
){
	double noise=A_binding(conc_prev,params,p)
	            +A_dissociation(conc_prev,params,p)
	            +AB_dissociation(conc_prev,params,p);
	check_noise_sign(conc_prev, noise, 'A');
	return noise;
}

double noiseB (
	const state& conc_prev,
	const params_array& params,
	const int p
){
	double noise=B_binding(conc_prev,params,p)
	            +B_dissociation(conc_prev,params,p)
	            +BA_dissociation(conc_prev,params,p);
	check_noise_sign(conc_prev,noise, 'B');
	return noise;
}

double noiseR (
	const state& conc_prev,
	const params_array& params,
	const int p
){
	double noise=R_binding(conc_prev,params,p)
	            +R_dissociation(conc_prev,params,p);
	check_noise_sign(conc_prev,noise, 'R');
	return noise;
}

double noiseX (
	const state& conc_prev,
	const params_array& params,
	const int p
){
	double noise=X_binding(conc_prev,params,p)
	            +X_dissociation(conc_prev,params,p);
	check_noise_sign(conc_prev,noise, 'X');
	return noise;
}

void check_noise_sign(
	const state& conc_prev,
	const double noise,
	const char S
){
	if(noise<0.) {
		std::cerr << "Negative noise" << S << ": " << noise << std::endl;
		dump_state(conc_prev);
		exit(EXIT_FAILURE);
	}
}
