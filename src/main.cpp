/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#include <iostream>
#include <rtaudio/RtAudio.h>

#include <ui/TGuitarUI.h>
#include <ui/InitialMenu.h>
#include <audio/AudioProcessor.h>
#include <audio/DelayEffect.h>

int main() {
    const auto processor = audio::AudioProcessor::GetInstance();
    const auto audio = processor->getAudioInstance();

    if (audio->getDeviceCount() < 1) {
        std::cout << "No audio devices found!\n";
        return EXIT_FAILURE;
    }

    ui::printDevicesAvailable(*audio);

    auto [input_params, output_params] = ui::selectDevices(*audio);

    processor->setInputStreamParameters(input_params);
    processor->setOutputStreamParameters(output_params);

    processor->setMetronome(true);
    processor->setBPM(120);

    auto delay = audio::DelayEffect(0.2, 0.2);
    processor->addEffect(std::make_unique<audio::DelayEffect>(delay));

    processor->runStream([] {
        const auto ui = new ui::TGuitarUI();
        ui->start();
    });

    return EXIT_SUCCESS;
}
