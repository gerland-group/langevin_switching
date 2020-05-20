/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file contains a function template that is used by each of the
 * individual reaction terms in the dynamical equations.
 */

#ifndef RATES_H
#define RATES_H

#include "parameters.h"
#include "state.h"

typedef double (rate_func)(const state&, const params_array&, const int);

rate_func A_binding;
rate_func A_dissociation;
rate_func AB_dissociation;
rate_func B_binding;
rate_func B_dissociation;
rate_func BA_dissociation;
rate_func R_binding;
rate_func R_dissociation;
rate_func X_binding;
rate_func X_dissociation;

#endif
