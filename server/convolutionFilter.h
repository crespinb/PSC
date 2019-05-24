#ifndef INCLUDED_CONVOLUTION_FILTER
#define INCLUDED_CONVOLUTION_FILTER

#include "config.h"
#include <armadillo>
#include "fftw/fftw.h"

using namespace arma;

class ConvolutionFilter
{
    public:
    ConvolutionFilter(vec& inCoeffs);
    void setCoeffs(vec& inCoeffs);
    vec filter(vec& input);
    private:
    vec coeffs;
    cx_vec fftCoeffs;
    sp::FFTW fftwTool;
};

#endif