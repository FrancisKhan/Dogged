#include "BaseHeatCode.h"
#include "HeatCodeFactory.h"
#include "HeatSolver.h"

#include <iostream>

using namespace Eigen;

void HeatSolver::solve(int max_iter_number, double accuracy)
{
    std::shared_ptr<BaseHeatCode> heatCode = HeatCodeFactory::setHeatCode(m_mesh, m_library);

    VectorXd oldTemps = VectorXd::Ones(m_mesh.getCellsNumber());
    VectorXd newTemps = VectorXd::Zero(m_mesh.getCellsNumber());

    for(int i = 0; i < max_iter_number; i++)
    {
        auto [T, source]   = heatCode->setupSystem();	
        auto [Tb, sourceb] = heatCode->applyBoundaryConditions(T, source);
        heatCode->solveSystem(Tb, sourceb);

        newTemps = m_mesh.getTemperatures("C");

        // max difference between new and old temperatures
        double maxValue = 0.0;
        for(int i = 0; i < newTemps.size(); i++)
        {
           double diff = fabs(newTemps(i) - oldTemps(i));
           if (diff > maxValue) maxValue = diff;
        }

        // exit condition
		if (maxValue < accuracy) break;

        oldTemps = newTemps;
    }

    out.getLogger()->debug("Mesh middle points:");
    printVector(m_mesh.getMeshMiddlePoints(), out, TraceLevel::DEBUG);

    out.getLogger()->debug("Temperatures:");
    printVector(m_mesh.getTemperatures("C"), out, TraceLevel::DEBUG);
}