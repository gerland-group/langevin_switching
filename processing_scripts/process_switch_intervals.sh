#!/bin/bash

# Extracts statistics for the time intervals between switches from the simulation data


if [[ $# -lt 1 ]]; then
	outputdir="switch_intervals"
else
	outputdir="$1"
fi

if [[ ! -d "$outputdir" ]]; then
	mkdir $outputdir
else
	rm $outputdir/*
fi

cp params.switch.template "$outputdir"/

octave-cli << EOF

joblist=load('job_list');
switchtest=1; %A=1 B=2 R=3 X=4

P=11;
Xup_array=[0 0.1 0.133352 0.177828 0.237137 0.316228 0.421697 0.562341 0.749894 1. 1.33352 1.77828 2.37137 3.16228 4.21697 5.62341 7.49894 10 13.3352 17.7828 23.7137 31.6228 42.1697 56.2341 74.9894 100];
Xdn_array=[0 0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5 0.55 0.6 0.65 0.7 0.75 0.8 0.85 0.9 0.95 1.0];
Xup=size(Xup_array,2)
Xdown=size(Xdn_array,2)

for p=0:(P-1)
	clear c;
	c=zeros(Xup+Xdown-1,5);
	for n=1:size(joblist,1)
		if(joblist(n,3)==p)
			JOB=joblist(n,1);
			d=joblist(n,2);
			i=joblist(n,4);

			if(isdir(sprintf('job_%d',JOB)))
				if(exist(sprintf('job_%d/switches.txt.0',JOB),'file'))
					s=dlmread(sprintf('job_%d/switches.txt.0',JOB));
					safterX=s(s>20);
					if(d==0)
						c(Xdown+i,1)=Xdown+i;
						c(Xdown+i,2)=Xup_array(i+1);
					else
						c(Xdown-i,1)=Xdown-i;
						c(Xdown-i,2)=-Xdn_array(i+1);
					end
					if(size(safterX,1)>1)
						t=diff(safterX);
						if(d==0)
							c(Xdown+i,3)=mean(t);
							c(Xdown+i,4)=std(t);
							c(Xdown+i,5)=size(t,1);
						else
							c(Xdown-i,3)=mean(t);
							c(Xdown-i,4)=std(t);
							c(Xdown-i,5)=size(t,1);
						end
					end
				else
					sprintf('missing switches: %d', JOB);
				end
			else
				sprintf('missing directory: %d',JOB)
			end
		end
	end
	dlmwrite(sprintf('$outputdir/switch_stats_%d.out',p),c,' ');
end

EOF
