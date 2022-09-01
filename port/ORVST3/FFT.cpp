#include "FFT.h"

#define M_PI 3.14159265358979323846

int FFT::ensureIsPowerOfN(int x, int n)
{
    int log_value = (int)(logBaseN((double)x, (double)n));
    int valid_size = pow(n, log_value);
    return (valid_size != x) ? (valid_size * n) : (valid_size);
}

FFT::FFT(vector<double> real_input, vector<double> imaginary_input, bool inverse_transform, bool use_hanning_window)
{
    // Throw an exception if non-matching input signals are provided
    if (!imaginary_input.empty())
        if (real_input.size() != imaginary_input.size())
            throw new std::exception("Imaginary and real inputs are of different sizes.");

    // Throw an exception if less than three samples are provided
    if (real_input.size() < 3)
        throw new std::exception(std::string("Only " + std::to_string(real_input.size()) + " samples provided.\n" + "At least three are needed.").c_str());

    // Verify that the input size has a number of samples that is a
    // power of 2. If not, then increase the size of the array using
    // zero-padding. Also creates a zero filled imaginary component
    // of the input if none was specified.
    int valid_size = ensureIsPowerOfN(real_input.size(), 2);
    if (valid_size != real_input.size())
    {
        vector<double> temp;
        for (int i = 0; i < real_input.size(); i++)
            temp[i] = real_input[i];
        for (int i = real_input.size(); i < valid_size; i++)
            temp[i] = 0.0;
        real_input = temp;

        if (imaginary_input.empty())
        {
            imaginary_input.clear();
            for (int i = 0; i < valid_size; i++)
                imaginary_input[i] = 0.0;
        }
        else
        {
            temp.clear();
            for (int i = 0; i < imaginary_input.size(); i++)
                temp[i] = imaginary_input[i];
            for (int i = imaginary_input.size(); i < valid_size; i++)
                temp[i] = 0.0;
            imaginary_input = temp;
        }
    }
    else if (imaginary_input.empty())
    {
        imaginary_input.clear();
        for (int i = 0; i < valid_size; i++)
            imaginary_input[i] = 0.0;
    }

    // copy the input to output arrays since the algorithm used here is self-processing
    real_output = real_input;
    imaginary_output = imaginary_input;

    // Apply a Hanning window to the real values if this option is
    // selected
    if (use_hanning_window)
    {
        for (int i = 0; i < real_output.size(); i++)
        {
            double hanning = 0.5 - 0.5 * std::cos(2 * M_PI * i / valid_size);
            real_output[i] *= hanning;
        }
    }

    // Determine whether this is a forward or inverse transform
    int forward_transform = 1;
    if (inverse_transform)
        forward_transform = -1;

    // Reorder the input data into reverse binary order
    double scale = 1.0;
    int j = 0;
    for (int i = 0; i < valid_size; ++i)
    {
        if (j >= i)
        {
            double tempr = real_output[j] * scale;
            double tempi = imaginary_output[j] * scale;
            real_output[j] = real_output[i] * scale;
            imaginary_output[j] = imaginary_output[i] * scale;
            real_output[i] = tempr;
            imaginary_output[i] = tempi;
        }
        int m = valid_size / 2;
        while (m >= 1 && j >= m)
        {
            j -= m;
            m /= 2;
        }
        j += m;
    }

    // Perform the spectral recombination stage by stage
    int stage = 0;
    int max_spectra_for_stage;
    int step_size;
    for (max_spectra_for_stage = 1, step_size = 2 * max_spectra_for_stage; max_spectra_for_stage < valid_size; max_spectra_for_stage = step_size, step_size = 2 * max_spectra_for_stage)
    {
        double delta_angle = forward_transform * M_PI / max_spectra_for_stage;

        // Loop once for each individual spectra
        for (int spectra_count = 0; spectra_count < max_spectra_for_stage; ++spectra_count)
        {
            double angle = spectra_count * delta_angle;
            double real_correction = std::cos(angle);
            double imag_correction = std::sin(angle);

            int right = 0;
            for (int left = spectra_count; left < valid_size; left += step_size)
            {
                right = left + max_spectra_for_stage;
                double temp_real = real_correction * real_output[right] - imag_correction * imaginary_output[right];
                double temp_imag = real_correction * imaginary_output[right] + imag_correction * real_output[right];
                real_output[right] = real_output[left] - temp_real;
                imaginary_output[right] = imaginary_output[left] - temp_imag;
                real_output[left] += temp_real;
                imaginary_output[left] += temp_imag;
            }
        }
        max_spectra_for_stage = step_size;
    }

    // Set the angle and magnitude to null originally
    output_angle.clear();
    output_power.clear();
    output_magnitude.clear();
}

vector<double> FFT::getMagnitudeSpectrum() {
    // Only calculate the magnitudes if they have not yet been calculated
    if (output_magnitude.empty())
    {
        for (int i = 0; i < imaginary_output.size() / 2; i++)
            output_magnitude[i] = (std::sqrt(real_output[i] * real_output[i] + imaginary_output[i] * imaginary_output[i])) / real_output.size();
    }

    // Return the magnitudes
    return output_magnitude;
}

vector<double> FFT::getPowerSpectrum() {
    // Only calculate the powers if they have not yet been calculated
    if (output_power.empty())
    {
        for (int i = 0; i < imaginary_output.size() / 2; i++)
            output_power[i] = (real_output[i] * real_output[i] + imaginary_output[i] * imaginary_output[i]) / real_output.size();
    }

    // Return the power
    return output_power;
}

vector<double> FFT::getPhaseAngles() {
    if (output_angle.empty())
    {
        for (int i = 0; i < imaginary_output.size() / 2; i++)
        {
            if (imaginary_output[i] == 0.0 && real_output[i] == 0.0)
                output_angle[i] = 0.0;
            else
                output_angle[i] = std::atan(imaginary_output[i] / real_output[i]) * 180.0 / M_PI;

            if (real_output[i] < 0.0 && imaginary_output[i] == 0.0)
                output_angle[i] = 180.0;
            else if (real_output[i] < 0.0 && imaginary_output[i] == -0.0)
                output_angle[i] = -180.0;
            else if (real_output[i] < 0.0 && imaginary_output[i] > 0.0)
                output_angle[i] += 180.0;
            else if (real_output[i] < 0.0 && imaginary_output[i] < 0.0)
                output_angle[i] += -180.0;
        }
    }

    // Return the phase angles
    return output_angle;
}

vector<double> FFT::getBinLabels(double sampling_rate) {
    double bin_width = sampling_rate / (double)real_output.size();
    vector<double> labels;
    labels[0] = 0.0;
    for (int bin = 1; bin < imaginary_output.size() / 2; bin++)
        labels[bin] = bin * bin_width;
    return labels;
}