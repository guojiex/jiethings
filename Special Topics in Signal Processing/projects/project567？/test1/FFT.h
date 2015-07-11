#pragma once
#ifndef FFT_H
#define FFT_H
#include <cstring>
#include <cmath>
#define pi 3.141592
class FFT
{
private:
 void changeOrder(double* xr,double* xi,int n);
public:
 FFT(void);
 void FFT_1D(double* ctxr,double* ctxi,double* cfxr,double* cfxi,int len);
 void rFFT_1D(double* ctxr,double* cfxr,double* cfxi,int len);
 void IFFT_1D(double* cfxr,double* cfxi,double* ctxr,double* ctxi,int len);
 void rIFFT_1D(double* cfxr,double* cfxi,double* ctxr,int len);
 ~FFT(void);
};
#endif

