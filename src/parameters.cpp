/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file defines a function to update the local model parameters based on
 * the local X level.
 */

#include "parameters.h"

void update_local_parameters(
	const double localX,
	const params_array& base_params,
	params_array& current_params,
	const int PMOD,
	const bool down
){
	current_params[PMOD]=std::max(base_params[PMOD]*(1.+(down?-1.:1.)*localX),0.);
}
