/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef DISTORTIONEFFECT_H
#define DISTORTIONEFFECT_H

#include <audio/Effect.h>
#include <ftxui/dom/elements.hpp>

namespace audio {
    class DistortionEffect final : public Effect {
    public:
        explicit DistortionEffect(float timbre);
        explicit DistortionEffect(const std::string &name, float timbre);

        ~DistortionEffect() override = default;

        void setTimbre(float timbre);

        void process(unsigned int nFrames, float *in) override;

        ftxui::Element toUI() const override;

    private:
        float timbre_ = 1.0f;
        float depth_ = 1.0f;

        static unsigned distortion_count_;
    };
}

#endif //DISTORTIONEFFECT_H
