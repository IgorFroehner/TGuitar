/*
* Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H
#include <vector>

#include "Effect.h"

namespace audio {
    class AudioProcessor final {
    public:
        AudioProcessor();

        ~AudioProcessor();

        void applyEffects(unsigned nFrames, float *buffer) const;

        void addEffect(std::unique_ptr<Effect> effect);
    private:
        std::vector<std::unique_ptr<Effect> > effects;
    };
}

#endif //AUDIO_PROCESSOR_H
