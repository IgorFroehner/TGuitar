/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef AUDIODATA_H
#define AUDIODATA_H
#include <atomic>

constexpr unsigned int SAMPLE_RATE = 48000; // 44.1 kHz (CD quality)
constexpr unsigned int BUFFER_SIZE = 256; // Frames per buffer

namespace audio {
    extern std::atomic<float> inputLevel;
    extern std::atomic<float> outputLevel;
}

#endif //AUDIODATA_H
