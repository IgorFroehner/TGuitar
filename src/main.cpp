/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <fstream>
#include <iostream>
#include <map>
#include <regex>

#include <rtaudio/RtAudio.h>

#include <ui/TGuitarUI.h>
#include <ui/InitialMenu.h>
#include <audio/AudioProcessor.h>
#include <audio/DelayEffect.h>
#include <audio/DistortionEffect.h>
#include <config/LoadConfigFile.h>

int main() {
    const auto processor = audio::AudioProcessor::GetInstance();
    const auto audio = processor->getAudioInstance();

    if (audio->getDeviceCount() < 1) {
        std::cout << "No audio devices found!\n";
        return EXIT_FAILURE;
    }

    const auto file_path = "/Users/igor/development/audio/tguitar/tguitar.toml";

    if (!config::load_config_from_file(file_path)) {
        ui::get_config_input();
    }

    // processor->setMetronome(false);
    // processor->setBPM(80);

    auto distortion = audio::DistortionEffect("Distortion", 1.0);
    processor->addEffect(std::make_unique<audio::DistortionEffect>(distortion));

    auto delay = audio::DelayEffect("Delay", 0.2, 0.2);
    processor->addEffect(std::make_unique<audio::DelayEffect>(delay));

    processor->runStream([] {
        const auto ui = new ui::TGuitarUI();
        ui->start();
    });

    return EXIT_SUCCESS;
}
