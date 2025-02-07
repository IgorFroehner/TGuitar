/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/DistortionEffect.h"

#include <cmath>

namespace audio {
    DistortionEffect::DistortionEffect(const float timbre) : _timbre(timbre) {
    }

    void DistortionEffect::setTimbre(const float timbre) {
        _timbre = timbre;
    }

    void DistortionEffect::process(const unsigned int nFrames, float *in) {
        float depth = 1.0f;
        float timbre = _timbre;
        float timbreInverse = (1 - (timbre * 0.099f)) * 10;
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
