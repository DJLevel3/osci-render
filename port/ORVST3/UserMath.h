#pragma once

#include <cmath>
#include <string>

#define EPSILON 0.0001

double round(double value, double places) {
    const double mult = pow(10, places);
    return std::round(value * mult) / mult;
}

bool parseable(std::string value) {
    try
    {
        std::stod(value);
        return true;
    }
    catch(std::exception e)
    {
        return false;
    }
}

double tryParse(std::string value) {
    try
    {
        return std::stod(value);
    }
    catch (std::exception e)
    {
        return 0;
    }
}