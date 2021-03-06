#ifndef CROSSSECTION_H
#define CROSSSECTION_H

#include "numeric_tools.h"

#include <algorithm>
#include <vector>

class CrossSection
{
public:
    CrossSection() : temperature_(0.0), backgroundXS_(0.0), 
                     values_(std::vector<double> {}), 
                     resInterval_(std::pair<unsigned, unsigned>(0, 0)) {}

    CrossSection(double temperature, double backgroundXS, const std::vector<double> &values) :
    temperature_(temperature), backgroundXS_(backgroundXS), values_(values), 
    resInterval_(std::pair<unsigned, unsigned>(0, 0)) {}

	CrossSection(double temperature, double backgroundXS, const std::vector<double> &values,
                 std::pair<unsigned, unsigned> resInterval) :
    temperature_(temperature), backgroundXS_(backgroundXS), values_(values), resInterval_(resInterval) {}

    void setTemperature(double t) {temperature_ = t;}
    double getTemperature() const {return temperature_;} 
    void setBackgroundXS(double b) {backgroundXS_ = b;}
    double getBackgroundXS() const {return backgroundXS_;}
    void setValues(const std::vector<double> &v) {values_ = v;}
    std::vector<double> getValues() const {return values_;}
    unsigned getSize() const {return values_.size();}
    bool hasOnlyZeroes() const;
    void setToZero();
    void setResonanceInterval(const std::pair<unsigned, unsigned>& resInterval) {resInterval_ = resInterval;}
    std::pair<unsigned, unsigned> getResonanceInterval() const {return resInterval_;} 

    CrossSection operator+(const CrossSection& rhs);
    CrossSection operator-(const CrossSection& rhs);
    CrossSection& operator+=(const CrossSection& rhs);
    CrossSection& operator-=(const CrossSection& rhs);

    friend CrossSection& operator*=(CrossSection& lhs, double rhs);

private:
    double temperature_;
    double backgroundXS_;
    std::vector<double> values_;
    std::pair<unsigned, unsigned> resInterval_;
};

inline CrossSection operator*(const double lhs, const CrossSection& rhs)
{
    std::vector<double> rhsVec = rhs.getValues();
    std::vector<double> resultVec(rhsVec.size(), 0.0);
    std::transform(rhsVec.begin(), rhsVec.end(), resultVec.begin(), [lhs](auto k){return k * lhs;});

    CrossSection xs(rhs.getTemperature(), rhs.getBackgroundXS(), resultVec);

    return xs;
}

#endif