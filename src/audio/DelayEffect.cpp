/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/DelayEffect.h"

#include <vector>
#include <audio/AudioData.h>

namespace audio {
    DelayEffect::DelayEffect() {
        delayBuffer_ = std::vector<float>(delaySeconds_ * SAMPLE_RATE, 0);
    }

    DelayEffect::DelayEffect(const float delayTime, const float feedback) : delayBuffer_(SAMPLE_RATE * delayTime, 0),
                                                                            delaySeconds_(delayTime),
                                                                            feedback_(feedback) {
    }

    DelayEffect::~DelayEffect() = default;


    void DelayEffect::process(unsigned nFrames, float *input) {
        for (unsigned i = 0; i < nFrames; i++) {
            delayBuffer_[delayIndex_] = input[i];
            delayIndex_ = (delayIndex_ + 1) % delayBuffer_.size();
            input[i] = input[i] + feedback_ * delayBuffer_[delayIndex_];
        }
    }
}
