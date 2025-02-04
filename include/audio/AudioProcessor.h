/*
* Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

namespace audio {

class AudioProcessor final {
  public:
    AudioProcessor();
    ~AudioProcessor();

    void helloWorld();
    void process(int nFrames, float *input, float *output);
};

}

#endif //AUDIO_PROCESSOR_H
