/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file contains definitions of functions associated with the current
 * state of the system, and the definitions for the different possible forms
 * of Xmax over time.
 */

#include "state.h"

void dump_state(
	const state& conc_prev
){
	FILE* outerr=fopen("error.txt","w");
	std::cerr << "State:" << std::endl;
	for(int i=0; i<NSPEC; ++i) {
		for(int p=0; p<NPOOL; ++p) {
			std::cerr << conc_prev[i][p] << std::endl;
		}
	}
	fclose(outerr);
}

double calculate_omega(
	const state& conc_prev,
	const int index
){
	if(conc_prev[index][LEFT]+conc_prev[index][RIGHT]>0.) {
		return (conc_prev[index][LEFT]-conc_prev[index][RIGHT])/(conc_prev[index][LEFT]+conc_prev[index][RIGHT]);
	} else {
		return 0;
	}
}

void correct_abundances(
	state& conc
){
	for(int i=0; i<NSPEC; ++i) {
		//check whether any concentration is greater than total or less than 0
		for(int p=0; p<TOTAL; ++p) {
			if(conc[i][p]>conc[i][TOTAL])
				conc[i][p]=conc[i][TOTAL];
			if(conc[i][p]<0.)
				conc[i][p]=0.;
		}
		//check whether sum of abundances exceeds the total
		if(conc[i][LEFT]+conc[i][RIGHT]>conc[i][TOTAL]) {
			double excess=conc[i][LEFT]+conc[i][RIGHT]-conc[i][TOTAL];
			//if so, subtract from the as evenly as possible
			if(conc[i][LEFT]<excess/2.) {
				conc[i][LEFT] =0;
				conc[i][RIGHT]=conc[i][TOTAL];
			} else {
				if(conc[i][RIGHT]<excess/2.) {
					conc[i][LEFT] =conc[i][TOTAL];
					conc[i][RIGHT]=0;
				} else {
					conc[i][LEFT] -=excess/2.;
					conc[i][RIGHT]-=excess/2.;
				}
			}
		}
	}
}

double Xt_linear(
	const double t,
	const double slope,
	const double Xinitial,
	const double Xfinal
){
	if(((slope>0) && (Xinitial+t*slope<Xfinal)) ||
	   ((slope<0) && (Xinitial+t*slope>Xfinal))
		) {
		return Xinitial+t*slope;
	} else {
		return Xfinal;
	}
}

double Xt_exp(
	const double t,
	const double decay_const,
	const double Xinitial,
	const double Xfinal
){
	if(Xinitial>Xfinal) {
		return Xfinal+(Xinitial-Xfinal)*std::exp(-t*std::fabs(decay_const));
	} else {
		return Xinitial+(Xfinal-Xinitial)*(1.-std::exp(-t*fabs(decay_const)));
	}
}

double Xt_step(
	const double /*t*/,
	const double /*Xrate*/,
	const double /*Xinitial*/,
	const double Xfinal
){
	return Xfinal;
}
