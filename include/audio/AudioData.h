/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef AUDIODATA_H
#define AUDIODATA_H

#include <atomic>

#include "AudioProcessor.h"

namespace audio {
    struct RunTimeValues {
        std::atomic<float> input_level {0.0f};
        std::atomic<float> output_level {0.0f};
        std::atomic<bool> metronome {false};
        std::atomic<unsigned> metronome_bpm {0};
    };

    extern RunTimeValues runTimeValues;
}

#endif //AUDIODATA_H
