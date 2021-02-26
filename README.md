# Langevin simulations of polarity switching

(c) 2020 Filipe Tostevin, Manon Wigbers

This software was used in the manuscript "Four different mechanisms for
switching cell polarity" by Tostevin F.\*, Wigbers M.\*, Søgaard-Andersen L. &
Gerland U. ([PLoS Comput Biol 17(1): e1008587](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1008587)).

The software simulates the dynamics of the Langevin model of the Mgl-Rom _M.
xanthus_ polarity system described in the above work, which is based on the
model by Guzzo et al \([Nat Microbiol 3, 948–959,
2018](https://www.nature.com/articles/s41564-018-0203-x)\).

## Build

This program requires the Property Tree module from the [Boost C++
libraries](https://www.boost.org/) for reading the simulation input file.

A `Makefile` is provided for GNU Make and the GNU C++ compiler `g++`, which can
be invoked using
```
make -C ./src
```

## Usage

Simulation and switching parameters are defined in [params.in](params.in).

The main executable is at `./src/langevin`. By default the asymmetry values
&omega; for each protein are output at the start and end of the signal period
and at the end of the simulation.

Additional options are:
- `-t` produces time-series output (for the first 100 realisations if `N_rep`
is set to be larger than this.
- `-s` outputs a list of the times at which polarity switches happen (defined
by changes in the sign of the asymmetry of MglA, &omega;<sub>A</sub>.)

## Scripts

Convenience scripts are provided to run the simulations needed to evaluate the
switching probability in different signaling scenarios. These scripts replace
placeholders in the `params.*.template` files to generate a new `params.in` file
for each job.
Due to the large number of simulations involved, it is recommended to edit
these scripts to distribute these jobs among different compute hosts.
To extract the corresponding switching statistics, corresponding scripts
are provided in the [./processing_scripts](./processing_scripts) folder. These
in turn use [GNU Octave](https://www.gnu.org/software/octave/) to process the
numerical data.

- [run_phase.sh](run_phase.sh) loops over parameters, signal amplitude and
duration.
	- [process_phase.sh](./processing_scripts/process_phase.sh) extracts the
		switching probabilities from the raw simulation data and arranges them in
		two-dimensional arrays, one for each model parameter, with signal amplitude
		and duration on the axes.

- [run_switch_intervals.sh](run_switch_intervals.sh) runs long simulations for
different parameters and signal amplitudes, and outputs a list of switching
times.
	- [process_switch_intervals.sh](./processing_scripts/process_switch_intervals.sh)
		extracts the mean and standard deviation of the intervals between switches,
		and outputs an array for each model parameter with these statistics as a
		function of the signal amplitude.

- [run_vs_noise.sh](run_vs_noise.sh) simulates different noise levels for specified
parameter and signal combinations, and outputs trajectories in addition to &omega;
values.
	- [process_vs_noise.sh](./processing_scripts/process_vs_noise.sh) to convert
		these to an array of switching probabilities as a function of the noise level.

