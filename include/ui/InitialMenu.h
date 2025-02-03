
#ifndef INITIAL_MENU_H
#define INITIAL_MENU_H

#include <iostream>
#include <rtaudio/RtAudio.h>

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


inline RtAudio::StreamParameters selectDevice(const unsigned int deviceId, const int nChannels,
                                               std::vector<unsigned int> const &deviceIds) {
    if (std::find(deviceIds.begin(), deviceIds.end(), deviceId) == deviceIds.end()) {
        throw std::runtime_error("Device not found!");
    }

    RtAudio::StreamParameters deviceParams;
    deviceParams.deviceId = deviceId;
    deviceParams.nChannels = nChannels;
    deviceParams.firstChannel = 0;

    return deviceParams;
}

}

#endif //INITIALMENU_H
