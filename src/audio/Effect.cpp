/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/Effect.h"

namespace audio {
    Effect::Effect() = default;

    Effect::~Effect() = default;

    void Effect::process(unsigned int nFrames, float *in) {}
}
