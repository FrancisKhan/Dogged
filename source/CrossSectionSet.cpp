#include "CrossSectionSet.h"
#include "additionalPrintFuncs.h"

#include <algorithm>
#include <iomanip>

CrossSectionSet CrossSectionSet::operator/(const CrossSectionSet& rhs)
{
    std::vector<double> temperatures = getTemperatures();
    std::vector<double> dilutions = getBackgroundXSs();

    CrossSectionSet result;

    for(size_t i = 0; i < temperatures.size(); i++)
    {
        for(size_t j = 0; j < dilutions.size(); j++)
        {
            std::vector<double> rhsVec = rhs.getXSNoInterp(temperatures[i], dilutions[j]).getValues();
            std::vector<double> lhsVec = getXSNoInterp(temperatures[i], dilutions[j]).getValues();
                
            std::vector<double> resultVec(rhsVec.size(), 0.0);
            std::transform(lhsVec.begin(), lhsVec.end(), rhsVec.begin(), resultVec.begin(), std::divides<double>());

            CrossSection crossSection(temperatures[i], dilutions[j], resultVec);
            result.addXS(crossSection);
        }
    }

    return result;
}

CrossSectionSet CrossSectionSet::operator+(const CrossSectionSet& rhs)
{
    std::vector<double> temperatures = getTemperatures();
    std::vector<double> dilutions = getBackgroundXSs();

    CrossSectionSet result;

    for(size_t i = 0; i < temperatures.size(); i++)
    {
        for(size_t j = 0; j < dilutions.size(); j++)
        {
            std::vector<double> rhsVec = rhs.getXSNoInterp(temperatures[i], dilutions[j]).getValues();
            std::vector<double> lhsVec = getXSNoInterp(temperatures[i], dilutions[j]).getValues();
                
            std::vector<double> resultVec(rhsVec.size(), 0.0);
            std::transform(lhsVec.begin(), lhsVec.end(), rhsVec.begin(), resultVec.begin(), std::plus<double>());

            CrossSection crossSection(temperatures[i], dilutions[j], resultVec);
            result.addXS(crossSection);
        }
    }

    return result;
}

CrossSectionSet CrossSectionSet::operator-(const CrossSectionSet& rhs)
{
    std::vector<double> temperatures = getTemperatures();
    std::vector<double> dilutions = getBackgroundXSs();

    CrossSectionSet result;

    for(size_t i = 0; i < temperatures.size(); i++)
    {
        for(size_t j = 0; j < dilutions.size(); j++)
        {
            std::vector<double> rhsVec = rhs.getXSNoInterp(temperatures[i], dilutions[j]).getValues();
            std::vector<double> lhsVec = getXSNoInterp(temperatures[i], dilutions[j]).getValues();

            std::vector<double> resultVec(rhsVec.size(), 0.0);
            std::transform(lhsVec.begin(), lhsVec.end(), rhsVec.begin(), resultVec.begin(), std::minus<double>());

            CrossSection crossSection(temperatures[i], dilutions[j], resultVec);
            result.addXS(crossSection);
        }
    }

    return result;
}

CrossSectionSet CrossSectionSet::operator*(const double rhs)
{
    std::vector<double> temperatures = getTemperatures();
    std::vector<double> dilutions = getBackgroundXSs();

    CrossSectionSet result;

    for(size_t i = 0; i < temperatures.size(); i++)
    {
        for(size_t j = 0; j < dilutions.size(); j++)
        {
            std::vector<double> lhsVec = getXSNoInterp(temperatures[i], dilutions[j]).getValues();
                
            std::vector<double> resultVec(lhsVec.size(), 0.0);
            std::transform(lhsVec.begin(), lhsVec.end(), resultVec.begin(), [rhs](auto k){return k * rhs;});

            CrossSection crossSection(temperatures[i], dilutions[j], resultVec);
            result.addXS(crossSection);
        }
    }

    return result;
}

CrossSectionSet CrossSectionSet::operator/(const double rhs)
{
    std::vector<double> temperatures = getTemperatures();
    std::vector<double> dilutions = getBackgroundXSs();

    CrossSectionSet result;

    for(size_t i = 0; i < temperatures.size(); i++)
    {
        for(size_t j = 0; j < dilutions.size(); j++)
        {
            std::vector<double> lhsVec = getXSNoInterp(temperatures[i], dilutions[j]).getValues();
                
            std::vector<double> resultVec(lhsVec.size(), 0.0);
            std::transform(lhsVec.begin(), lhsVec.end(), resultVec.begin(), [rhs](auto k){return k / rhs;});

            CrossSection crossSection(temperatures[i], dilutions[j], resultVec);
            result.addXS(crossSection);
        }
    }

    return result;
}


CrossSection CrossSectionSet::getXSNoInterp(double t, double b) const
{
    std::vector<CrossSection>::const_iterator it = std::find_if(XSSet_.begin(), XSSet_.end(), 
    [t, b] (const CrossSection &c) 
    {return Numerics::is_equal(c.getTemperature(), t) && Numerics::is_equal(c.getBackgroundXS(), b);});

    if (it != XSSet_.end())
        return *it;
    else
        return CrossSection {};
}

void CrossSectionSet::calcXSs() 
{
    using namespace Numerics;

    for(unsigned i = 0; i < getSize(); i++)
    {
        double sigma0 = getXSNoInterp(i).getBackgroundXS();
        double temp   = getXSNoInterp(i).getTemperature();

        if(Numerics::not_equal(sigma0, Numerics::DINF))
        {
            
            std::vector<double> infValues = getXSNoInterp(temp, Numerics::DINF).getValues();
            CrossSection dilXS = getXSNoInterp(i);
            std::vector<double> dilValues = dilXS.getValues();
            std::vector<double> newValues = infValues + dilValues;

            //debugCalcXS(newValues, infValues, dilValues, temp, sigma0);

            std::pair<unsigned, unsigned> resInterval = dilXS.getResonanceInterval();
            CrossSection xs(temp, sigma0, newValues, resInterval);
            setXS(i, xs);
        }
    }
}

std::vector<double> CrossSectionSet::getTemperatures() const
{
    std::vector<double> result;

    for(const auto& i : XSSet_)
        result.push_back(i.getTemperature());

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;   
}

std::vector<double> CrossSectionSet::getBackgroundXSs() const
{
    std::vector<double> result;

    for(const auto& i : XSSet_)
        result.push_back(i.getBackgroundXS());

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;   
}

// It is unlikely that a XSSet has a XS(t_i, b_i) empty and others with values
bool CrossSectionSet::isEmpty() const
{
    if(XSSet_[0].getSize() != 0)
    {
        if(XSSet_[0].hasOnlyZeroes())
            return true;
        else
            return false;
    }
    else
        return true;
}

void CrossSectionSet::debugCalcXS(std::vector<double> &newValues, std::vector<double> &infValues,
        std::vector<double> &dilValues, double temp, double sigma0) 
{
    auto it = std::find_if(newValues.begin(), newValues.end(), 
              [] (auto &v) {return Numerics::is_lower(v, 0.0);});

    if (it != newValues.end())
    {
        out.print(TraceLevel::CRITICAL, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
        out.print(TraceLevel::CRITICAL, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
        out.print(TraceLevel::CRITICAL, "XS kind: {}", get_name(getKind()));
        out.print(TraceLevel::CRITICAL, "Temperature: {}", temp);
        out.print(TraceLevel::CRITICAL, "Background XS: {}", sigma0);

        out.print(TraceLevel::CRITICAL, "\nInfinite XS:");
        PrintFuncs::printVector(infValues, out, TraceLevel::CRITICAL);

        out.print(TraceLevel::CRITICAL, "Diluted XS:");
        PrintFuncs::printVector(dilValues, out, TraceLevel::CRITICAL);

        out.print(TraceLevel::CRITICAL, "New XS:");
        PrintFuncs::printVector(newValues, out, TraceLevel::CRITICAL);

        throw std::runtime_error("ERROR: error in CalcXS");
    }
}