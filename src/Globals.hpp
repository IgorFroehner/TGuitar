/*
 * Copyright (c) 2025 Igor Froehner. All rights reserved.
 * Use of this source code is governed by the MIT license that can be found in
 * the LICENSE file.
 */

#ifndef GLOBAL_STATE_HPP
#define GLOBAL_STATE_HPP

#include <atomic>

namespace globals {
    inline std::atomic<float> gInputLevel{0.0};
}

#endif //GLOBAL_STATE_HPP
