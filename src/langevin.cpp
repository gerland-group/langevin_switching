/*
 * Copyright (c) 2020 Filipe Tostevin, Manon Wigbers
 *
 * This file contains the main routine that reads the simulation parameters 
 * from the input file, sets up the simulation, and calls the propagation
 * routines.
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <boost/property_tree/ini_parser.hpp>
#include "parameters.h"
#include "propagator.h"

using namespace std;

bool WITH_DATA=false;
bool WITH_SWITCH=false;

int main(
	int argc,
	char **argv
){
	int opt;
	while ((opt = getopt(argc, argv, "ts")) != -1) {
		switch (opt) {
			case 't':
				WITH_DATA=true;
				cout << "Outputting trajectories" << endl;
				break;
			case 's':
				WITH_SWITCH=true;
				cout << "Outputting switching times" << endl;
				break;
			default: /* '?' */
				cerr << "Usage: " << argv[0] << " [-t] [-s]" << endl;
				cerr << "       -t : Output trajectories" << endl;
				cerr << "       -s : Output switching times" << endl;
				exit(EXIT_FAILURE);
		}
	}

	boost::property_tree::ptree P;
	try{
		boost::property_tree::read_ini("params.in",P);
	} catch(...) {
		cerr << "Input file not found: params.in" << endl;
		exit(EXIT_FAILURE);
	}
	const int Nrep=P.get<int>("N_rep");                               // Number of realizations to run
	const double Xt_basal=P.get<double>("X_basal");                   // Xt level before and after the signal
	const double t_burnin=P.get<double>("t_burnin");                  // Time at which signal is applied
	const double t_total=P.get<double>("t_total");                    // Total simulation time
	const double N=P.get<double>("copy_number");                      // Inverse noise level (particle number)
	const int PMOD=P.get<int>("modified_parameter");                  // Index of reaction that is modified by X
	const bool down=P.get<bool>("down");                              // Whether X decreases rate

	const double Xt_signal=P.get<double>("signal.X_signal");          // Xt level during signal
	const double t_signal=P.get<double>("signal.duration");           // Signal duration
	const string profile=P.get<string>("signal.profile");
	double Xrate;                                                     // rate at which XT changes


	if(PMOD>=NPARAMS-2) {
		cerr << "Modified parameter is out of range, PMOD=" << PMOD << ">" << NPARAMS-3 << endl;
		exit(EXIT_FAILURE);
	}

	//RDE parameters
	params_array base_params;
	base_params[k_rA]=400;
	base_params[k_a]=2;
	base_params[k_ba]=400;
	base_params[k_B]=2;
	base_params[k_bB]=30;
	base_params[k_b]=2.8;
	base_params[k_M]=0.3;
	base_params[k_ab]=0.5*30;
	base_params[k_R]=0.1;
	base_params[k_bR]=1.5;
	base_params[k_r]=0.4;
	base_params[k_X]=20;
	base_params[k_x]=3;

	propagator p(PMOD, down, WITH_DATA, WITH_SWITCH);

	if(profile == "linear") {
		const double t_change=P.get<double>("signal.transition_time");
		if(t_change==0) {
			p.set_Xt_func(Xt_step);
			cout << "Using step Xt profile" << endl;
		} else {
			Xrate=(Xt_signal-Xt_basal)/t_change;
			p.set_Xt_func(Xt_linear);
			cout << "Using linear Xt profile" << endl;
		}
	} else {
		if(profile == "step") {
			p.set_Xt_func(Xt_step);
			cout << "Using step Xt profile" << endl;
		} else {
			if(profile == "exp") {
				Xrate=P.get<double>("signal.decay_rate");
				p.set_Xt_func(Xt_exp);
				cout << "Using exponential Xt profile" << endl;
			} else {
				cout << "Unknown Xt profile" << endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	ofstream omegas;
	omegas.open("omegas.txt");

	//Loop over realizations
	for(int n=0; n<Nrep; ++n) {
		if(p.output_data) {
			if(n<100) {
				p.data.open("data.txt." + to_string(n));
				p.data << "t A1 A2 At B1 B2 Bt R1 R2 Rt X1 X2 Xt" << endl;
			} else {
				p.output_data=false;
			}
		}
		if(p.output_switches) {
			p.switches.open("switches.txt." + to_string(n));
		}

		//initialize all polar concentrations at ~0.3*total
		state conc;
		conc[A][TOTAL]=1.;
		conc[A][LEFT]=0.35*conc[A][TOTAL];
		conc[A][RIGHT]=0.3*conc[A][TOTAL];
		conc[B][TOTAL]=1.;
		conc[B][LEFT]=0.35*conc[B][TOTAL];
		conc[B][RIGHT]=0.3*conc[B][TOTAL];
		conc[R][TOTAL]=1.;
		conc[R][LEFT]=0.35*conc[R][TOTAL];
		conc[R][RIGHT]=0.3*conc[R][TOTAL];
		conc[X][TOTAL]=Xt_basal;
		conc[X][LEFT]=0.35*conc[X][TOTAL];
		conc[X][RIGHT]=0.3*conc[X][TOTAL];

		double t=0;

		//Run until beginning of signal period
		p.propagate(conc, base_params, t, t_burnin, N, 0., Xt_basal);

		//Calculate omegas before signal is applied
		for(int i=0; i<NSPEC; ++i) {
			omegas << calculate_omega(conc,i) << " ";
		}

		//Run for the duration of the signal
		p.propagate(conc, base_params, t, t_burnin+t_signal, N, Xrate, Xt_signal);

		//Calculate omegas at the end of the signal period
		for(int i=0; i<NSPEC; ++i) {
			omegas << calculate_omega(conc,i) << " ";
		}

		//Run until the end of the simulation time
		p.propagate(conc, base_params, t, t_total, N, -Xrate, Xt_basal);

		//Calculate the final omegas
		for(int i=0; i<NSPEC; ++i) {
			omegas << calculate_omega(conc,i) << " ";
		} omegas << endl;

		if(p.output_switches) {
			p.switches.close();
		}
		if(p.output_data) {
			p.data.close();
		}
	}

	omegas.close();
	return EXIT_SUCCESS;
}
