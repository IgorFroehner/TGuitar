
#include "audio/AudioProcessor.h"
#include <iostream>

namespace audio {

// Constructor implementation
AudioProcessor::AudioProcessor() {
    // Constructor code here, if needed.
    std::cout << "AudioProcessor created." << std::endl;
}

// Destructor implementation
AudioProcessor::~AudioProcessor() {
    // Destructor code here, if needed.
    std::cout << "AudioProcessor destroyed." << std::endl;
}


void AudioProcessor::process(int nFrames, float *input, float *output) {
    for (int i = 0; i < nFrames; i++) {
        output[i] = input[i] + input[i + nFrames];
    }
}

void AudioProcessor::helloWorld() {
  std::cout << "Hello World!" << std::endl;
}

}
