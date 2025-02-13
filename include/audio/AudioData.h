/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef AUDIODATA_H
#define AUDIODATA_H
#include <atomic>
// #include <fftw3.h>
#include <vector>

constexpr unsigned int SAMPLE_RATE = 48000; // 44.1 kHz (CD quality)
constexpr unsigned int BUFFER_SIZE = 256; // Frames per buffer

// constexpr int FFT_SIZE = 1024;
//
// double *fftInput = (double *) fftw_malloc(sizeof(double) * FFT_SIZE);
// fftw_complex *fftOutput = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * (FFT_SIZE / 2 + 1));
//
// fftw_plan fftPlan = fftw_plan_dft_r2c_1d(FFT_SIZE, fftInput, fftOutput, FFTW_ESTIMATE);
//
// std::vector<float> fftBuffer;

namespace audio {
    extern std::atomic<float> inputLevel;
    extern std::atomic<float> outputLevel;
    extern std::atomic<bool> fftReady;
}

#endif //AUDIODATA_H
