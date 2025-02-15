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

namespace audio {
    struct AudioData {
        unsigned int inputChannels;
        unsigned int outputChannels;
    };

    class AudioProcessor final {
    public:
        AudioProcessor(const AudioProcessor &) = delete;
        void operator=(const AudioProcessor &) = delete;

        static AudioProcessor *GetInstance();

        void setInputStreamParameters(const RtAudio::StreamParameters &parameters);

        void setOutputStreamParameters(const RtAudio::StreamParameters &parameters);

        void applyEffects(unsigned nFrames, float *buffer) const;

        void addEffect(std::unique_ptr<Effect> effect);

        RtAudio *getAudioInstance() { return &audio_; }

        void startStream(const std::function<void()> &wrapped_function);

        // void audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFrames,
        //                                   double, RtAudioStreamStatus status, void *audioData);

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
    };
}

#endif //AUDIO_PROCESSOR_H
