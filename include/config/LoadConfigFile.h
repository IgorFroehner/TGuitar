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
    RtAudio::StreamParameters findDeviceByName(const std::string &device_name, audio::DeviceType deviceType) {
        const auto processor = audio::AudioProcessor::GetInstance();
        const auto audio = processor->getAudioInstance();

        const auto device_ids = audio->getDeviceIds();

        RtAudio::StreamParameters deviceParams;
        deviceParams.firstChannel = 0;

        for (const auto &device_id: device_ids) {
            auto deviceInfo = audio->getDeviceInfo(device_id);

            if (deviceInfo.name == device_name) {
                if (deviceType == audio::INPUT && deviceInfo.inputChannels > 0) {
                    deviceParams.nChannels = 1;
                    deviceParams.deviceId = device_id;
                    return deviceParams;
                }

                if (deviceType == audio::OUTPUT && deviceInfo.outputChannels > 0) {
                    deviceParams.deviceId = device_id;
                    deviceParams.nChannels = deviceInfo.outputChannels;
                    return deviceParams;
                }
            }
        }

        throw std::runtime_error("Device " + device_name + " not found!");
    }

    bool load_config_from_file(const std::string &file_path) {
        const auto processor = audio::AudioProcessor::GetInstance();

        auto config = toml::parse_file(file_path);

        auto input_device_str = config["input_device"].value_or<std::string>("");
        auto output_device_str = config["output_device"].value_or<std::string>("");

        if (input_device_str.empty() || output_device_str.empty()) {
            std::cerr << "Invalid configuration file, missing input or output device.\n";
            return false;
        }

        auto input_device = findDeviceByName(input_device_str, audio::INPUT);
        auto output_device = findDeviceByName(output_device_str, audio::OUTPUT);

        processor->setInputStreamParameters(input_device);
        processor->setOutputStreamParameters(output_device);

        return true;
    }

    bool config_exists(const std::string &file_path) {
        struct stat buffer{};
        return stat(file_path.c_str(), &buffer) == 0;
    }
}

#endif //LOAD_CONFIG_FILE_H
