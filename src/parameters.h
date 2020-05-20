/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file contains the list of model parameters used to access entries in
 * the parameters array
 */

#ifndef PARAMS_H
#define PARAMS_H

#include <algorithm>
#include <cmath>

enum params{
	k_rA,
	k_a,
	k_ba,
	k_B,
	k_bB,
	k_b,
	k_M,
	k_ab,
	k_R,
	k_bR,
	k_r,
	k_X,
	k_x,
	NPARAMS
};
typedef double params_array[NPARAMS];

void update_local_parameters(const double, const params_array&, params_array&, const int, const bool);

#endif
