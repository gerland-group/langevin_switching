/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file defines functions associated with the propagator class that
 * integrates the dynamical equations.
 */

#include "propagator.h"

propagator::propagator(
	const int new_PMOD,
	const bool new_down,
	const bool WITH_DATA,
	const bool WITH_SWITCH
){
	unsigned int seed;
	std::ifstream dev_random("/dev/urandom", std::ios::binary|std::ios::in);
	dev_random.read(reinterpret_cast<char*>(&seed), sizeof(seed));
	dev_random.close();
	RNG.seed(seed);
	std::cout << "Random seed: " << seed << std::endl;

	PMOD=new_PMOD;
	down=new_down;
	output_data=WITH_DATA;
	output_switches=WITH_SWITCH;
}

void propagator::set_Xt_func(
	const Xt_profile* new_Xt_func
){
	Xt_func=new_Xt_func;
}

void propagator::propagate(
	state& conc,
	const params_array& base_params,
	double& t,
	const double tmax,
	const double N,
	const double Xrate,
	const double Xfinal
){
	params_array current_params;
	for(int i=0; i<NPARAMS; ++i) {
		current_params[i]=base_params[i];
	}

	int step_counter=0;
	state conc_prev;
	const double Xinitial=conc[X][TOTAL];
	const double tinitial=t;

	//variable to store the time at which the last switch occurred
	double last_switch=std::min(0.,t-1.);

	while(t<tmax) {
		conc[X][TOTAL]=Xt_func(t-tinitial, Xrate, Xinitial, Xfinal);
		correct_abundances(conc);

		for(int i=0; i<NSPEC; ++i) {
			for(int p=0; p<NPOOL; ++p) {
				conc_prev[i][p]=conc[i][p];
			}
		}

		for(int p=0; p<TOTAL; ++p) {
			update_local_parameters(conc_prev[X][p], base_params, current_params, PMOD, down);
			conc[A][p]+=dt*(
			                A_binding(conc_prev,current_params,p)
			               -A_dissociation(conc_prev,current_params,p)
			               -AB_dissociation(conc_prev,current_params,p)
			               )
			              +sqrt(dt*noiseA(conc_prev,current_params,p)/N)*dist(RNG)
			             ;
			conc[B][p]+=dt*(
			                B_binding(conc_prev,current_params,p)
			               -B_dissociation(conc_prev,current_params,p)
			               -BA_dissociation(conc_prev,current_params,p)
			               )
			              +sqrt(dt*noiseB(conc_prev,current_params,p)/N)*dist(RNG)
			             ;
			conc[R][p]+=dt*(
			                R_binding(conc_prev,current_params,p)
			               -R_dissociation(conc_prev,current_params,p)
			               )
			              +sqrt(dt*noiseR(conc_prev,current_params,p)/N)*dist(RNG)
			             ;
			conc[X][p]+=dt*(
			                X_binding(conc_prev,current_params,p)
			               -X_dissociation(conc_prev,current_params,p)
			               )
			              +sqrt(dt*noiseX(conc_prev,current_params,p)/N)*dist(RNG)
			             ;
		}

		correct_abundances(conc);
		t+=dt;

		//Output
		if(output_switches) {
			if( std::signbit(conc[A][LEFT]-conc[A][RIGHT])!=std::signbit(conc_prev[A][LEFT]-conc_prev[A][RIGHT]) && //sign of omega has changed
			    t-last_switch>0.1      //limit reporting of diffusion in the vicinity of symmetry
			) {
				switches << t << std::endl;
				last_switch=t;
			}
		}
		if(step_counter%100==0) {
			if(output_data) {
				data << t << " ";
				for(int i=0; i<NSPEC; ++i) {
					for(int p=0; p<NPOOL; ++p) {
						data << conc[i][p] << " ";
					}
				}
				data << std::endl;
			}
		}
		++step_counter;
	}
}
