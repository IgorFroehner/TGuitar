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


    inline RtAudio::StreamParameters selectDevice(const unsigned int deviceId, RtAudio &audio, DeviceType deviceType) {
        const auto deviceIds = audio.getDeviceIds();

        if (std::find(deviceIds.begin(), deviceIds.end(), deviceId) == deviceIds.end()) {
            throw std::runtime_error("Device not found!");
        }

        auto deviceInfo = audio.getDeviceInfo(deviceId);

        RtAudio::StreamParameters deviceParams;

        deviceParams.deviceId = deviceId;
        if (deviceType == INPUT) {
            deviceParams.nChannels = deviceInfo.inputChannels;
        } else if (deviceType == OUTPUT) {
            deviceParams.nChannels = deviceInfo.outputChannels;
        }

        deviceParams.firstChannel = 0;

        return deviceParams;
    }
}

#endif //INITIALMENU_H
