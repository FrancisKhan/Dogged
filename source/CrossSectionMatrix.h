#ifndef CROSSSECTIONMATRIX_H
#define CROSSSECTIONMATRIX_H

#include "numeric_tools.h"
#include "CrossSection.h"

class CrossSectionMatrix
{
public:
    CrossSectionMatrix() : m_temperature(0.0), m_backgroundXS(0.0) {}
    
	CrossSectionMatrix(double temperature, double backgroundXS, Eigen::MatrixXd &values) :
    m_temperature(temperature), m_backgroundXS(backgroundXS), m_values(values) {}

    void setTemperature(double t) {m_temperature = t;}
    double getTemperature() const {return m_temperature;}
    void setBackgroundXS(double b) {m_backgroundXS = b;}
    double getBackgroundXS() const {return m_backgroundXS;}
    void setValues(Eigen::MatrixXd &m) {m_values = m;}
    Eigen::MatrixXd getValues() {return m_values;}
    unsigned getSize() {return m_values.rows();}
    CrossSection condenseToXS() const;

private:
    double m_temperature;
    double m_backgroundXS;
    Eigen::MatrixXd m_values;
};

#endif