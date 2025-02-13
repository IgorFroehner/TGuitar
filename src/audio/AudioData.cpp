/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/AudioData.h"

#include <atomic>

namespace audio {
    std::atomic inputLevel{0.0f};
    std::atomic outputLevel{0.0f};
    std::vector<float> g_FFTBuffer;
    std::atomic<bool> g_FFTReady{false};
}
