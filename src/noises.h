/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file contains declarations of functions associated with the noise terms
 * in the model
 */

#ifndef NOISE_H
#define NOISE_H

#include <iostream>
#include <cstdlib>
#include "state.h"
#include "parameters.h"
#include "reactions.h"

typedef double (noise_func)(const state&, const params_array&, const int);

noise_func noiseA;
noise_func noiseB;
noise_func noiseR;
noise_func noiseX;

void check_noise_sign(const state&, const double, const char);

#endif
