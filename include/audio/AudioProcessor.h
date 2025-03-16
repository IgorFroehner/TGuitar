/*
* Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include <RtAudio.h>
#include <vector>

#include "Effect.h"
#include "Metronome.h"

namespace audio {
    struct AudioData {
        unsigned int inputChannels;
        unsigned int outputChannels;
    };

    class AudioProcessor final {
    public:
        // no copy or assignment for the singleton class
        AudioProcessor(const AudioProcessor &) = delete;

        void operator=(const AudioProcessor &) = delete;

        static AudioProcessor *GetInstance();

        void setInputStreamParameters(const RtAudio::StreamParameters &parameters);

        void setOutputStreamParameters(const RtAudio::StreamParameters &parameters);

        void applyEffects(unsigned nFrames, float *buffer) const;

        void addEffect(std::unique_ptr<Effect> effect);

        void setMetronome(bool value);

        void setBPM(unsigned bpm);

        [[nodiscard]] float mixIn(unsigned currentCount) const;

        RtAudio *getAudioInstance() { return &audio_; }

        void runStream(const std::function<void()> &interfaceFunction);

        const std::vector<std::unique_ptr<Effect>>& getEffects() const;

        static constexpr int clickDuration = 200; // length of the click in samples
    private:
        AudioProcessor();

        ~AudioProcessor();

        static AudioProcessor *instance_;
        static std::mutex mutex_;

        std::vector<std::unique_ptr<Effect> > effects_;
        RtAudio audio_;

        RtAudio::StreamParameters input_stream_parameters_;
        RtAudio::StreamParameters output_stream_parameters_;

        AudioData audio_data_{};

        Metronome metronome_;
    };
}

#endif //AUDIO_PROCESSOR_H
