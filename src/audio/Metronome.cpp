/*
* Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <audio/AudioData.h>
#include <audio/Metronome.h>

namespace audio {
    Metronome::Metronome() {
        click_buffer_.resize(click_duration);
        // Generate a click: a short sine burst that decays (you can design your own)
        for (int i = 0; i < click_duration; ++i) {
            // Simple decaying sine: amplitude decays linearly from 1.0 to 0.0
            const float amplitude = 1.0f - (static_cast<float>(i) / click_duration);
            // Using a frequency of 1kHz for the click
            click_buffer_[i] = amplitude * std::sin(2.0f * M_PI * 1000.0f * (static_cast<float>(i) / SAMPLE_RATE));
        }
    }

    void Metronome::setBPM(const unsigned bpm) {
        bpm_ = bpm;
        runTimeValues.metronome_bpm = bpm;
        samples_per_beat_ = SAMPLE_RATE * 60 / bpm_;
    }

    float Metronome::mixIn(const unsigned currentCount) const {
        if (!active_) return 0.0;

        if (currentCount % samples_per_beat_ < click_duration) {
            return click_buffer_[currentCount % samples_per_beat_];
        }

        return 0.0;
    }

    void Metronome::setActive(bool active) {
        runTimeValues.metronome = active;
        active_ = active;
    }
}
