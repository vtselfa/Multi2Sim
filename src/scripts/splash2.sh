# cpu and cache configuration files path
config="./configuracions/cpuconfig.prefetchwork" 
cacheconfig="./configuracions/cacheconfig.8nodes.L2-2MB-64B-32ways" 
netconfig="./configuracions/netconfig.mesh"

# simulator path
sim="./m2s" 

# All the Splash benchmarks	
for bench in cholesky fft radix; 
do 	
	# context configuration file path for each benchmark
	ctxconfig="./configuracions/ctxconfigs/ctxconfig.${bench}" 
	
	# Run m2s 
	$sim --cpu-sim detailed --net-config $netconfig --report-net mesh.report --cpu-config $config --mem-config $cacheconfig --ctx-config $ctxconfig 2>${bench}.net.err 

	echo "${bench}... THE END" 
done 
