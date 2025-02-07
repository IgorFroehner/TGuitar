/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/AudioProcessor.h"
#include <iostream>

namespace audio {
    AudioProcessor::AudioProcessor() = default;

    AudioProcessor::~AudioProcessor() = default;

    void AudioProcessor::applyEffects(const unsigned int nFrames, float *buffer) const {
        for (const auto& effect: effects) {
            effect->process(nFrames, buffer);
        }
    }

    void AudioProcessor::addEffect(std::unique_ptr<Effect> effect) {
        effects.push_back(std::move(effect));
    }
}
