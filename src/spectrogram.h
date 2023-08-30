// File spectrogram.h
#ifndef _SPECTROGRAM_H
#define _SPECTROGRAM_H

#include "fft.h"

// Constantes autogeneradas. ESTE ARCHIVO ESTÁ BIEN Y SE CORRESPONDE CON EL EJEMPLO DEL LIDI
#ifndef N_FFT
#define CONVERT_TO_DB 0
#define N_FFT 256
#define N_MELS 32
#define FRAME_LENGTH 6000
#define SAMPLE_RATE 6000
#define BLOCKS 23
#define N_FFT_TABLE 128
#define NOVERLAP 0
#define STEP 256
#define LEN_NFFT_NMELS 4
#define SPEC_SIZE 736
#define TS_US 83 //Tiempo de muestreo (166/2)
#endif

void create_spectrogram(float *data, float *result);

#endif
	