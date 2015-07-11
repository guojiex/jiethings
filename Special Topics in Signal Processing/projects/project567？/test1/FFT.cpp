#include "FFT.h"

FFT::FFT()
{
}
///////////////////////////
//倒序实现
//xr实部，xi虚部，n为2的幂
///////////////////////////
void FFT::changeOrder(double *xr,double *xi,int n)
{
 double temp;
 int k;
 int lh=n/2;
 int j=lh;
 int n1=n-2;
 for(int i=1;i<=n1;i++)
 {
  if(i<j)
  {
   temp=xr[i];
   xr[i]=xr[j];
   xr[j]=temp;
   temp=xi[i];
   xi[i]=xi[j];
   xi[j]=temp;
  }
  k=lh;
  while(j>=k)
  {
   j=j-k;
   k=(int)(k/2+0.5);
  }
  j=j+k;
 }
}
//////////////////////////
//复数FFT
//ctxr和ctxi的长度为len
//cfxr和cfxi的长度为2的幂
//////////////////////////
void FFT::FFT_1D(double *ctxr,double *ctxi,double *cfxr,double *cfxi,int len)
{ 
 int m=ceil(log((double)len)/log(2.0));
 int l,b,j,p,k;
 double rkb,ikb;
 int n=1<<m;
 double* rcos=new double[n/2];
 double* isin=new double[n/2];
 for(l=0;l<n/2;l++)           
 {
  rcos[l]=cos(l*pi*2/n);
  isin[l]=sin(l*pi*2/n);
 }                           
 memcpy(cfxr,ctxr,sizeof(double)*len);
 memcpy(cfxi,ctxi,sizeof(double)*len);
 for(l=len;l<n;l++)
 {
  cfxr[l]=0;
  cfxi[l]=0;
 }
 changeOrder(cfxr,cfxi,n);  //倒序
 for(l=1;l<=m;l++)
 {
  b=(int)(pow(2,l-1)+0.5);
  for(j=0;j<b;j++)
  {
   p=j*(int)(pow(2,m-l)+0.5);
   for(k=j;k<n;k+=(int)(pow(2,l)+0.5))
   {
    rkb=cfxr[k+b]*rcos[p]+cfxi[k+b]*isin[p];
    ikb=cfxi[k+b]*rcos[p]-cfxr[k+b]*isin[p];
    cfxr[k+b]=cfxr[k]-rkb;
    cfxi[k+b]=cfxi[k]-ikb;
    cfxr[k]=cfxr[k]+rkb;
    cfxi[k]=cfxi[k]+ikb;
   }
  }
 }
 delete []rcos;
 delete []isin;
}
/////////////////////////////
//实数FFT
//ctxr的长度为len
//cfxr和cfxi的长度为2的幂
////////////////////////////
void FFT::rFFT_1D(double *ctxr,double *cfxr,double *cfxi,int len)
{
 int m=ceil(log((double)len)/log(2.0));
 int n=1<<m;
 double* rcos=new double[n/2];
 double* isin=new double[n/2];
 for(int l=0;l<n/2;l++)          
 {
  rcos[l]=cos(l*pi*2/n);
  isin[l]=sin(l*pi*2/n);
 }    
 double* txr=new double[(len+1)/2];
 double* txi=new double[(len+1)/2];
 for(int i=0;i<len/2;i++)
 {
  txr[i]=ctxr[i*2];
  txi[i]=ctxr[i*2+1];
 }
 if(len%2==1)
 {
  txr[(len-1)/2]=ctxr[len-1];
  txi[(len-1)/2]=0;
 }
 FFT_1D(txr,txi,cfxr,cfxi,(len+1)/2);
 double* x1r=new double[n/2];
 double* x1i=new double[n/2];
 double* x2r=new double[n/2];
 double* x2i=new double[n/2];
 x1r[0]=cfxr[0];
 x1i[0]=0;
 x2r[0]=cfxi[0];
 x2i[0]=0;
 for(int k=1;k<n/2;k++)
 {
  x1r[k]=(cfxr[k]+cfxr[n/2-k])/2.0;
  x1i[k]=(cfxi[k]-cfxi[n/2-k])/2.0;
  x2r[k]=(cfxi[k]+cfxi[n/2-k])/2.0;
  x2i[k]=(-cfxr[k]+cfxr[n/2-k])/2.0;
 }
 double rkb,ikb;
 for(int i=0;i<n/2;i++)
 {
  rkb=x2r[i]*rcos[i]+x2i[i]*isin[i];
  ikb=x2i[i]*rcos[i]-x2r[i]*isin[i];
  cfxr[i+n/2]=x1r[i]-rkb;
  cfxi[i+n/2]=x1i[i]-ikb;
  cfxr[i]=x1r[i]+rkb;
  cfxi[i]=x1i[i]+ikb;
 }
 delete []txr;
 delete []txi;
 delete []x1r;
 delete []x1i;
 delete []x2r;
 delete []x2i;
 delete []rcos;
 delete []isin;
}
///////////////////////////////
//复数IFFT
//cfxr和cfxi的长度为n(2的幂)
//ctxr和ctxi的长度为len
///////////////////////////////
void FFT::IFFT_1D(double *cfxr,double *cfxi,double *ctxr,double *ctxi,int len)
{
 int m=ceil(log((double)len)/log(2.0));
 int n=1<<m;
 double* txr=new double[n];
 double* txi=new double[n];
 for(int i=0;i<n;i++)
  cfxi[i]=-cfxi[i];
 int i=0;
 FFT_1D(cfxr,cfxi,txr,txi,n);
 for(i=0;i<len;i++)
 {
  ctxr[i]=txr[i]/n;
  ctxi[i]=-txi[i]/n;
 }
 delete []txr;
 delete []txi;
}
//////////////////////////////
//实数IFFT
//cfxr和cfxi的长度为n(2的幂)
//ctxr的长度为len
//////////////////////////////
void FFT::rIFFT_1D(double *cfxr,double *cfxi,double *ctxr,int len)
{
 int m=ceil(log((double)len)/log(2.0));
 int n=1<<m;
 double* txr=new double[n];
 double* txi=new double[n];
 for(int i=0;i<n;i++)
  cfxi[i]=-cfxi[i];
 FFT_1D(cfxr,cfxi,txr,txi,n);
 for(int i=0;i<len;i++)
 {
  ctxr[i]=txr[i]/n;
 }
 delete []txr;
 delete []txi;
}
FFT::~FFT(void)
{
}
