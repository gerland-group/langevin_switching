#!/bin/bash

# Extract the switching probabilities from the simulation data
# and arrange them to form the phase diagram with Xmax and tau
# on the two axes

if [[ $# -lt 1 ]]; then
	outputdir="switching_probabilities"
else
	outputdir="$1"
fi

if [[ ! -d "$outputdir" ]]; then
	mkdir $outputdir
else
	rm $outputdir/*
fi

cp params.phase.template "$outputdir"/

octave-cli << EOF

joblist=load('job_list');
switchtest=1; %A=1 B=2 R=3 X=4

Xup=26;
Xdown=21;
T=21;
P=11;


for p=0:(P-1)
	clear switched c;
	switched=zeros(Xup+Xdown-1,T);
	c=switched;
	for n=1:size(joblist,1)
		if(joblist(n,3)==p)
			JOB=joblist(n,1);
			d=joblist(n,2);
			i=joblist(n,4);
			j=joblist(n,5);

			if(isdir(sprintf('job_%d',JOB)))
				if(exist(sprintf('job_%d/omegas.txt',JOB),'file'))
					o=dlmread(sprintf('job_%d/omegas.txt',JOB));
					if(d==0)
						c(Xdown+i,j+1)+=size(o,1);
						switched(Xdown+i,j+1)+=sum((sign(o(:,switchtest)) ~= sign(o(:,switchtest+2*4))));
					else
						c(Xdown-i,j+1)+=size(o,1);
						switched(Xdown-i,j+1)+=sum((sign(o(:,switchtest)) ~= sign(o(:,switchtest+2*4))));
					end
				else
					sprintf('missing omegas: %d', JOB);
				end
			else
				sprintf('missing directory: %d',JOB)
			end
		end
	end
	scaled=switched./c;
	scaled(isnan(scaled)) = 0;
	dlmwrite(sprintf('$outputdir/switching_probability_%d.out',p),scaled,' ');
end

EOF
