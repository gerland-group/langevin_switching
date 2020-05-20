/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file contains declarations associated with the state of the system,
 * including declarations of the different protein species and subpools that
 * are used to access the dynamical variables of the model.
 */

#ifndef STATE_H
#define STATE_H

#include <iostream>
#include "parameters.h"

enum species{
	A,
	B,
	R,
	X,
	NSPEC
};

enum pool{
	LEFT,
	RIGHT,
	TOTAL,
	NPOOL
};

typedef double state[NSPEC][NPOOL];

void dump_state(const state&);
double calculate_omega(const state&, const int);
void correct_abundances(state&);

typedef double (Xt_profile)(const double, const double, const double, const double);
Xt_profile Xt_linear;
Xt_profile Xt_exp;
Xt_profile Xt_step;

#endif
