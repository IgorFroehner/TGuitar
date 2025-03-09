/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef EFFECT_H
#define EFFECT_H
#include <string>

namespace audio {
    class Effect {
    public:
        Effect();

        virtual ~Effect();

        virtual void process(unsigned int nFrames, float *in);

        virtual void setPassThrough(bool);

    protected:
        std::string name_ = "Effect";

        bool passThrough_ = false;
    };
}

#endif //EFFECT_H
