#pragma once

#include <vector>
using std::vector;
#include <cmath>
#include <string>

class FFT
{
  public:
    FFT(vector<double> real_input, vector<double> imaginary_input, bool inverse_transform, bool use_hanning_window);
    vector<double> getMagnitudeSpectrum();
    vector<double> getPowerSpectrum();
    vector<double> getPhaseAngles();
    vector<double> getBinLabels(double sampling_rate);
    vector<double> getRealValues() { return real_output; }
    vector<double> getImaginaryValues() { return imaginary_output; }

  private:
    vector<double> real_output;
    vector<double> imaginary_output;
    vector<double> output_angle;
    vector<double> output_magnitude;
    vector<double> output_power;

    int ensureIsPowerOfN(int x, int n);
    double logBaseN(double x, double n) { return (std::log10(x) / std::log10(n)); }
    int pow(int a, int b) { return std::pow(a, b); }
};