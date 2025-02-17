/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

namespace audio {
    extern std::vector<float> g_FFTBuffer;
    extern std::atomic<bool> g_FFTReady;
    extern std::atomic<unsigned> g_MetronomeCounter;
}

#endif //GLOBALS_H
