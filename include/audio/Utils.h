/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

namespace audio {
    enum DeviceType {
        INPUT,
        OUTPUT,
    };

    inline float clip(const float sample) {
        if (sample > 1.0f) return 1.0f;
        if (sample < -1.0f) return -1.0f;
        return sample;
    }
}

#endif //AUDIO_UTILS_H
