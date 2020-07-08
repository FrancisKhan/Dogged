#include "PolynomialFunction.h"
#include "Material.h"

#include <algorithm>
#include <iostream>

using namespace Eigen;

void Material::setThermalConductivityLaw(std::vector<std::string> &strVec)
{
    std::vector<double> dVec(strVec.size(), 0.0);
    std::transform(strVec.begin(), strVec.end(), dVec.begin(), 
    [](std::string &i){return std::stod(i);});

    m_thermalConductivityLaw = std::make_shared<PolynomialFunction>(dVec);
}

double Material::getThermalConductivity()
{
    return m_thermalConductivityLaw->calc(m_temperature);
}

void Material::setScattMatrix(const Numerics::Tensor2d &scattMatrix) 
{
    m_scattMatrix = MatrixXd::Zero(scattMatrix.dimension(0), scattMatrix.dimension(1));

    for(int i = 0; i < scattMatrix.dimension(0); i++)
    {
        for(int j = 0; j < scattMatrix.dimension(1); j++)
        {
            m_scattMatrix(i, j) = scattMatrix(i, j);
        }
    }
}