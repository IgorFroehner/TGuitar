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
