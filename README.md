# embedded-energy-mgtm
A collection of experiments to analyze, optimize and demonstrate energy management techniques in embedded systems.
There are 4 main folders, one for each energy aspect that was taken into consideration. Code is usually C++, Matlab or a mix of the two. A detailed report (available in pdf format) is associated to each experiment and can be found in the Reports folder. It is reccomended to go through these before looking at the code and the results.

## DPM
The purpose was to experience basic DPM policies by using a C++ simulator. A sample PSM (Power State Machine) was given and was integrated with some advanced policies and additional features. By default, an example workload file was provided and five more were generated. These files were then the basis for testing the efficiency of different policies.

## Encoding
The purpose was to familiarize with energy estimation in buses and data encoding. For this purpose several different data and address traces were produced and the energy required for their transmission was evaluated taking into consideration a simple wire
model (i.e. approximating a wire to a capacitor). These traces were then encoded to understand how different encodings can provide energy savings.

## Image processing
This covers	how image	filtering	and	compensation techniques	can	be	applied	and	how	they	differ	in	their	behaviour	based	on	the	original	image’s	characteristics. Two	are	the	main parameters	that	were	taken	into	account	when	evaluating	a	technique’s effectiveness:	distortion	and	power	consumption. The	latter being	the	most	important	factor	in	our	considerations.	
All	of	the	image	processing	was	done	in	the	Matlab	environment	and	heavily	automated	with two	main scripts.

## Electrical energy system (PV module, converter, battery, load, controller)
Starting from an initial skeleton Simulink model the photovoltaic (PV) module, DC-DC coverter, battery and load models were implemented and enhanced based on given industrial datasheets. Moreover, a controller was devised to manage which of the two between PV module or battery would be providing energy to the load.

### About
These works were done by Robert Margelli (github: sinkswim) and Andrea Floridia (github: florix) at Politecnico di Torino under the supervision of Prof. Massimo Poncino and Sara Vinco.
