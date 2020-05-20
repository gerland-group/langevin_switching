/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file defines a propagator class that wraps the integration of the dynamical
 * model equations.
 */

#ifndef PROP_H
#define PROP_H

#include <fstream>
#include <random>
#include "parameters.h"
#include "state.h"
#include "reactions.h"
#include "noises.h"

class propagator{
	private:
		std::mt19937 RNG;
		std::normal_distribution<double> dist;
		const double dt=0.0001;
		int PMOD;
		bool down;
		Xt_profile* Xt_func;

	public:
		bool output_data;
		bool output_switches;
		std::ofstream data;
		std::ofstream switches;

		propagator(const int, const bool, const bool, const bool);
		void propagate(state&, const params_array&, double&, const double, const double, const double, const double);
		void set_Xt_func(const Xt_profile*);
};

#endif
