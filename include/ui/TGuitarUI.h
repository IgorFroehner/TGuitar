/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef TGUITAR_UI_H
#define TGUITAR_UI_H

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui {
    class Graph {
    public:
        Graph() = default;

        std::vector<int> operator()(const int width, const int height) const {
            std::vector output(width, 0);
            if (data.empty() || max == 0.0f) return output;

            const int band_width = width / data.size();
            int remaining = width % data.size();

            const auto max = std::max(0.05f, this->max);

            int index = 0;
            for (const float band : data) {
                const int fill_width = band_width + (remaining > 0 ? 1 : 0);
                remaining--;

                const int value = static_cast<int>(band / max * height);

                for (int j = 0; j < fill_width && index < width; ++j) {
                    output[index++] = value;
                }
            }

            return output;
        }

        std::vector<float> data;
        float max = 0.0f;
    };

    class TGuitarUI {
    public:
        TGuitarUI();

        virtual ~TGuitarUI();

        void start();

    private:
        std::atomic<bool> running;
        ftxui::ScreenInteractive screen;

        ftxui::Element Header() const;

        ftxui::Element Body() const;

        static ftxui::Element Footer();

        void computeFFT(const std::vector<float> &samplesBlock);

        ftxui::Element theGraph() const;

        void UpdateLoop();

        Graph frequency_graph_;
    };
}

#endif //TGUITAR_UI_H
