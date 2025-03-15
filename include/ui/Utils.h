/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <iomanip>
#include <sstream>

namespace ui {
    inline std::string floatToString(const float value) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << value;
        return stream.str();
    }
}

#endif //UI_UTILS_H
