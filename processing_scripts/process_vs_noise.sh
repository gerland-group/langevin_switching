#!/bin/bash

# Extract the switching probabilities as a function of the noise level from the simulation data

octave-cli << EOF

joblist=load('job_list');
switchtest=1; %A=1 B=2 R=3 X=4

Nsites=7;
Nnoises=53;
switched=zeros(Nnoises,Nsites+1);
c=switched;

for i=1:size(joblist,1)
	JOB=joblist(i,1);
	s=joblist(i,2);
	n=joblist(i,3);
	i

	if(isdir(sprintf('job_%d',JOB)))
		if(exist(sprintf('job_%d/omegas.txt',JOB),'file'))
			o=dlmread(sprintf('job_%d/omegas.txt',JOB));
			c(n+1,s+2)=size(o,1);
			switched(n+1,s+2)=sum((sign(o(:,switchtest))~=sign(o(:,switchtest+2*4))));
		else
			sprintf('missing omegas: %d', JOB);
		end
	else
		sprintf('missing directory: %d',JOB)
	end
end
scaled=switched./c;
scaled(isnan(scaled)) = 0;

for n=0:(Nnoises-1)
	scaled(n+1,1)=10**-(6.5-n/8.0);
end
dlmwrite('switching_probability_vs_noise.out',scaled,' ');

EOF
