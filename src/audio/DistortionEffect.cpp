/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/DistortionEffect.h"

namespace audio {
    unsigned DistortionEffect::distortion_count_ = 0;

    DistortionEffect::DistortionEffect(const float timbre) : timbre_(timbre) {
        name_ = "Distortion " + std::to_string(distortion_count_++);
    }

    DistortionEffect::DistortionEffect(const std::string &name, const float timbre) : timbre_(timbre) {
        name_ = name;
    }

    void DistortionEffect::setTimbre(const float timbre) {
        timbre_ = timbre;
    }

    void DistortionEffect::process(const unsigned int nFrames, float *in) {
        if (passThrough_) {
            return;
        }

        const float depth = depth_;
        const float timbre = timbre_;
        const float timbreInverse = (1 - (timbre * 0.099f)) * 10;
        for (int i = 0; i < nFrames; i++) {
            in[i] = in[i] * depth;
            in[i] = tanh((in[i] * (timbre + 1.0f)));
            in[i] = (in[i] * ((0.1f + timbre) * timbreInverse));
            in[i] = cos((in[i] + (timbre + 0.25f)));
            in[i] = tanh(in[i] * (timbre + 1.0f));
            in[i] = in[i] * 0.125f;
        }
    }
}
