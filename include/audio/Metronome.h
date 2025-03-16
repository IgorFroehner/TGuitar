/*
* Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef METRONOME_H
#define METRONOME_H

#include <vector>

#include "Constants.h"

namespace audio {
    class Metronome {
    public:
        Metronome();
        virtual ~Metronome() = default;

        void setBPM(unsigned bpm);

        float mixIn(unsigned currentCount) const;

        void setActive(bool active);

        static constexpr int click_duration = 200;
    private:
        bool active_ = false;
        unsigned bpm_ = 60;
        unsigned samples_per_beat_ = SAMPLE_RATE * 60 / bpm_;
        std::vector<float> click_buffer_;
    };
}

#endif //METRONOME_H
