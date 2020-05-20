/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file contains definitions of the individual terms in the dynamical equations.
 */

#include "reactions.h"

double A_binding(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return (conc_prev[A][TOTAL]-conc_prev[A][LEFT]-conc_prev[A][RIGHT])*params[k_rA]*conc_prev[R][p];
}

double A_dissociation(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return params[k_a]*conc_prev[A][p];
}

double AB_dissociation(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return params[k_ba]*conc_prev[A][p]*conc_prev[B][p]*conc_prev[B][p];
}

double B_binding(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return (conc_prev[B][TOTAL]-conc_prev[B][LEFT]-conc_prev[B][RIGHT])*(params[k_B]+params[k_bB]*conc_prev[B][p]);
}

double B_dissociation(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return params[k_b]*conc_prev[B][p]/(1.+conc_prev[B][p]/params[k_M]);
}

double BA_dissociation(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return params[k_ab]*conc_prev[A][p]*conc_prev[B][p]*conc_prev[B][p];
}

double R_binding(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return (conc_prev[R][TOTAL]-conc_prev[R][LEFT]-conc_prev[R][RIGHT])*(params[k_R]+params[k_bR]*conc_prev[B][p]);
}

double R_dissociation(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return params[k_r]*conc_prev[R][p];
}

double X_binding(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return (conc_prev[X][TOTAL]-conc_prev[X][LEFT]-conc_prev[X][RIGHT])*params[k_X]*conc_prev[B][p];
}

double X_dissociation(
	const state& conc_prev,
	const params_array& params,
	const int p
){
	return params[k_x]*conc_prev[X][p];
}
