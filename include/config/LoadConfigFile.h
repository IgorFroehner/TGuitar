/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef LOAD_CONFIG_FILE_H
#define LOAD_CONFIG_FILE_H

#include <rtaudio/RtAudio.h>
#include "toml.h"
#include <audio/AudioProcessor.h>
#include <audio/Utils.h>
#include <sys/stat.h>

namespace config {
    inline RtAudio::StreamParameters findDeviceByName(const std::string &device_name, const audio::DeviceType deviceType) {
        const auto processor = audio::AudioProcessor::GetInstance();
        const auto audio = processor->getAudioInstance();

        const auto device_ids = audio->getDeviceIds();

        RtAudio::StreamParameters deviceParams;
        deviceParams.firstChannel = 0;

        for (const auto &device_id: device_ids) {
            auto device_info = audio->getDeviceInfo(device_id);

            if (device_info.name == device_name) {
                if (deviceType == audio::INPUT && device_info.inputChannels > 0) {
                    deviceParams.nChannels = 1;
                    deviceParams.deviceId = device_id;
                    return deviceParams;
                }

                if (deviceType == audio::OUTPUT && device_info.outputChannels > 0) {
                    deviceParams.deviceId = device_id;
                    deviceParams.nChannels = device_info.outputChannels;
                    return deviceParams;
                }
            }
        }

        throw std::runtime_error("Device " + device_name + " not found!");
    }

    inline bool config_exists(const std::string &file_path) {
        struct stat buffer{};
        return stat(file_path.c_str(), &buffer) == 0;
    }

    inline void loadDevices(audio::AudioProcessor *processor, const toml::table &config) {
        const auto input_device_str = config["input_device"].value_or<std::string>("");
        const auto output_device_str = config["output_device"].value_or<std::string>("");

        if (input_device_str.empty() || output_device_str.empty()) {
            std::cerr << "Invalid configuration file, missing input or output device.\n";
            return;
        }

        const auto input_device = findDeviceByName(input_device_str, audio::INPUT);
        const auto output_device = findDeviceByName(output_device_str, audio::OUTPUT);

        processor->setInputStreamParameters(input_device);
        processor->setOutputStreamParameters(output_device);
    }

    inline void loadMetronome(audio::AudioProcessor *processor, const toml::table &config) {
        const auto metronome_bpm = config["metronome"]["bpm"].value_or<unsigned>(80);
        const auto metronome_active = config["metronome"]["active"].value_or<bool>(false);

        processor->setMetronome(metronome_active);
        processor->setBPM(metronome_bpm);
    }

    inline bool load_config_from_file(const std::string &file_path) {
        if (!config_exists(file_path)) {
            return false;
        }
        std::cout << "Found configuration file tguitar.toml, trying to load config from it.\n";

        const auto processor = audio::AudioProcessor::GetInstance();

        const auto config = toml::parse_file(file_path);

        loadDevices(processor, config);
        loadMetronome(processor, config);

        return true;
    }
}

#endif //LOAD_CONFIG_FILE_H
