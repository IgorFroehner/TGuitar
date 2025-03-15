/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include "audio/DelayEffect.h"

#include <vector>
#include <audio/AudioData.h>
#include <audio/Constants.h>
#include <ui/Utils.h>

namespace audio {
    unsigned DelayEffect::delay_count_ = 0;

    DelayEffect::DelayEffect() {
        delay_buffer_ = std::vector<float>(delay_seconds_ * SAMPLE_RATE, 0);
        name_ = "Delay " + std::to_string(delay_count_++);
    }

    DelayEffect::DelayEffect(const float delayTime, const float feedback) : delay_buffer_(SAMPLE_RATE * delayTime, 0),
                                                                            delay_seconds_(delayTime),
                                                                            feedback_(feedback) {
        name_ = "Delay " + std::to_string(delay_count_++);
    }

    DelayEffect::DelayEffect(const std::string &name, float delayTime, float feedback) : delay_buffer_(SAMPLE_RATE * delayTime, 0),
                                                                                   delay_seconds_(delayTime),
                                                                                   feedback_(feedback) {
        name_ = name;
    }

    DelayEffect::~DelayEffect() = default;


    void DelayEffect::process(const unsigned nFrames, float *input) {
        for (unsigned i = 0; i < nFrames; i++) {
            delay_buffer_[delay_index_] = input[i];
            delay_index_ = (delay_index_ + 1) % delay_buffer_.size();
            input[i] = input[i] + feedback_ * delay_buffer_[delay_index_];
        }
    }

    ftxui::Element DelayEffect::toUI() const {
        using namespace ftxui;

        auto effect = vbox({
                   text(name_ + ":"),
                   text("  Delay Time = " + ui::floatToString(delay_seconds_)),
                   text("  Feedback = " + ui::floatToString(delay_seconds_))
               }) | bold;

        if (passThrough_) {
            return effect | color(Color::GrayDark);
        }

        return effect;
    }
}
