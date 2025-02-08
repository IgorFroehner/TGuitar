/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef INITIAL_MENU_H
#define INITIAL_MENU_H

#include <iostream>
#include <rtaudio/RtAudio.h>

enum DeviceType {
    INPUT,
    OUTPUT,
};

namespace ui {
    inline void printDevicesAvailable(RtAudio &audio) {
        std::cout << "Devices count: " << audio.getDeviceCount() << std::endl;

        std::cout << "Default input device: " << audio.getDefaultInputDevice() << std::endl;
        std::cout << "Default output device: " << audio.getDefaultOutputDevice() << std::endl << std::endl;

        std::cout << "Devices connected: " << std::endl;

        const auto deviceIds = audio.getDeviceIds();

        for (const auto &device_id: deviceIds) {
            auto deviceInfo = audio.getDeviceInfo(device_id);

            std::cout << deviceInfo.ID << "  " << deviceInfo.name << std::endl;
        }
        std::cout << std::endl;
    }

    static RtAudio::StreamParameters selectDevice(const unsigned int deviceId, RtAudio &audio,
                                                  const DeviceType deviceType) {
        if (const auto deviceIds = audio.getDeviceIds();
            std::find(deviceIds.begin(), deviceIds.end(), deviceId) == deviceIds.end()) {
            throw std::runtime_error("Device not found!");
        }

        const auto deviceInfo = audio.getDeviceInfo(deviceId);

        RtAudio::StreamParameters deviceParams;

        deviceParams.deviceId = deviceId;
        switch (deviceType) {
            case INPUT:
                if (deviceInfo.inputChannels <= 0) {
                    throw std::runtime_error("Device selected has no input channels specified!");
                }

                deviceParams.nChannels = 1; // TODO: change this when it handles stereo input
                break;
            case OUTPUT:
                if (deviceInfo.outputChannels <= 0) {
                    throw std::runtime_error("Device selected has no output channels specified!");
                }

                deviceParams.nChannels = deviceInfo.outputChannels;
                break;
        }

        deviceParams.firstChannel = 0;

        return deviceParams;
    }

    inline std::pair<RtAudio::StreamParameters, RtAudio::StreamParameters> selectDevices(RtAudio &audio) {
        unsigned int inputDeviceId, outputDeviceId;

        std::cout << "Input device id: ";
        std::cin >> inputDeviceId;
        auto inputParams = selectDevice(inputDeviceId, audio, INPUT);

        std::cout << "Output device id: ";
        std::cin >> outputDeviceId;
        auto outputParams = selectDevice(outputDeviceId, audio, OUTPUT);

        return std::make_pair(inputParams, outputParams);
    }
}

#endif //INITIAL_MENU_H
