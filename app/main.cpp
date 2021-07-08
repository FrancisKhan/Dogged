#include "Reactor.h"
#include "Output.h"
#include "Input.h"

#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
	Reactor reactor;
	Library library;

	Input input(reactor, library);
	input.getArguments(argc, argv);
	out.printStart();

	auto start = std::chrono::steady_clock::now();

	std::string inputPath = "/home/why/ALMOST_libs/draglibendfb7r0.txt";
	library.setXSLibraryPath(inputPath);

	std::vector<std::string> nucVec = {"Pu239", "La139"};
	std::vector < std::shared_ptr<Nuclide> > nuclides = library.getNuclides(nucVec);

	for(const auto& nuc : nuclides)
		nuc->printDebugData();

	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout << "Main, Timing: " 
			  << std::chrono::duration <double, std::milli> (diff).count() / 1000.0 
			  << std::endl;
}
