#ifndef NUCLIDEBLOCK_H
#define NUCLIDEBLOCK_H

#include <vector>
#include <string>
#include <iostream>

#include "additionalPrintFuncs.h"
#include "Nuclide.h"

using namespace PrintFuncs;

class NuclideBlock
{
public:
	NuclideBlock(std::vector<std::string> &dataVec, unsigned numberOfEnergyGroups) : 
	xsDataLines_(dataVec), numberOfEnergyGroups_(numberOfEnergyGroups),
	nuclide_(std::make_shared<Nuclide>()) {}

	std::shared_ptr<Nuclide> getNuclide();
	
private:
	std::pair<unsigned, unsigned> getNumberOfValuesToRead(unsigned line);
	unsigned getNumberOfLinesToRead(unsigned line);
	std::vector<double> readParameters(const std::string &key, unsigned lowerBound = 0, 
	unsigned upperBound = std::numeric_limits<unsigned>::max());
	std::vector< std::pair<unsigned, unsigned> > readTemperatureBlocks();
	std::vector< std::pair<unsigned, unsigned> > readDilutionBlocks(std::pair<unsigned, unsigned> &block);
	std::pair<unsigned, unsigned> readInfDilutionBlock(std::pair<unsigned, unsigned> &block);
	void readName();
	void readAWR();
	std::vector<double> readTemperatures();
	std::vector<double> readLambdas();
	std::vector<double> readDilutions(unsigned firstLine, unsigned lastLine);
	void readGroupConstants();
	std::vector<Nuclide::XSSetType> readXSs();
	std::vector<Nuclide::XSMatrixSetType> readXSMatrices();
	void isNuclideResonant();
	void isNuclideFissionable() const;
	void setNumberOfEnergyGroups();
	unsigned getNumberOfEnergyGroups() {return numberOfEnergyGroups_;}
	std::vector<double> populateXS(std::vector<double> &xsVec);
	void fromRImatrixToXSmatrix();
	void fromRItoXS();
	void addNu();
	void addScatteringL0XS();
	void addScatteringL1XS();
	void addAbsXS();
	void addTranspXS();
	void modifyChi();
	void additionalXSs();

	std::pair<unsigned, unsigned> readResInterval(const std::vector<double> &vec);

	void calcFineStructureFunc();
	std::vector<double> getWimsEnergyGroups();
	std::vector<double> fromEnergiesToLethargies(const std::vector<double>& vec);

	std::tuple< std::vector<double>, std::vector<int32_t>, std::vector<int32_t> > 
	readMatrixComponents(XSMatrixKind xsKind, unsigned lowBound, unsigned upperBound);
	std::pair<std::string, std::string> getMatrixKeys(XSMatrixKind xsKind);

	Eigen::MatrixXd assembleMatrixXS(XSMatrixKind xsKind, unsigned lowBound, unsigned upperBound);

	std::vector<std::string> xsDataLines_;
	unsigned numberOfEnergyGroups_;
	std::shared_ptr<Nuclide> nuclide_;
};

#endif