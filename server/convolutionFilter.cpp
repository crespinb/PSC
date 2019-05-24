#include "convolutionFilter.h"

ConvolutionFilter::ConvolutionFilter(vec& inCoeffs)
{
    fftwTool = sp::FFTW(SAMPLE_COUNT_PER_BUFFER*2, FFTW_ESTIMATE);
    coeffs = vec(SAMPLE_COUNT_PER_BUFFER*2, arma::fill::zeros);
    fftCoeffs = cx_vec(SAMPLE_COUNT_PER_BUFFER*2);
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)//change end condition?
    {
        coeffs[i] = inCoeffs[i];
    }
    fftCoeffs = fftwTool.fft(coeffs);
}

void ConvolutionFilter::setCoeffs(vec& inCoeffs)
{
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)//change end condition?
    {
        coeffs[i] = inCoeffs[i];
    }
    fftCoeffs = fftwTool.fft(coeffs);
}

vec ConvolutionFilter::filter(vec& input)
{
    vec input0 = vec(SAMPLE_COUNT_PER_BUFFER*2, arma::fill::zeros);
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)
    {
        input0[i] = input[i];
    }
    cx_vec fftInput = fftwTool.fft(input);
    cx_vec fftOutput = fftInput * fftCoeffs;
    return fftwTool.ifft(fftOutput);
}