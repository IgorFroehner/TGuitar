/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef DELAYEFFECT_H
#define DELAYEFFECT_H

#include <vector>
#include <audio/Effect.h>

namespace audio {
    class DelayEffect : public Effect {
    public:
        DelayEffect();
        DelayEffect(float delayTime, float feedback);

        ~DelayEffect() override;

        void process(unsigned nFrames, float *input) override;

    private:
        std::vector<float> delay_buffer_;
        float delay_seconds_ = 0.5;
        float feedback_ = 0.5;
        bool active_ = true;

        unsigned delay_index_ = 0;
    };
}

#endif //DELAYEFFECT_H
