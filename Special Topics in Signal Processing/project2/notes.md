[d,sr] = wavread('abc.wav')
d is sound signal,floating points, a vector
sr is the sample rate (FS) in Hertz
***
specgram(d, 256, sr);
spectrogram(d,256,128,128,sr);
Window 256, is the window length,to replace specgram,set it to 256 Hann window
Noverlap 128, is the number of overlapping segments that produces 50% overlap between segments.It should be less than the window.
nfft 128,is the FFT length and is the maximum of 256 or the next power of 2 greater than the length of each segment of x. (Instead of nfft, you can specify a vector of frequencies, F. See below for more information.)