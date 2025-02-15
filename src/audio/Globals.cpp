/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <vector>

#include <audio/Globals.h>

namespace audio {
    std::vector<float> g_FFTBuffer;
    std::atomic<bool> g_FFTReady{false};
}

