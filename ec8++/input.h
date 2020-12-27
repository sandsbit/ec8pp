/**
 *  _____ _____  _____
 * |  ___/  __ \|  _  | _     _
 * | |__ | /  \/ \ V /_| |_ _| |_
 * |  __|| |     / _ \_   _|_   _|
 * | |___| \__/\| |_| ||_|   |_|
 * \____/ \____/\_____/
 *
 * Simple C++ CHIP-8 emulator (Windows/macOS/Linux/Unix).
 * Copyright (C) 2020 Nikita Serba. All rights reserved
 * https://github.com/sandsbit/ec8pp/
 *
 *  This file is part of ec8pp.
 *
 *  ec8pp is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  ec8pp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  and GNU Lesser General Public License along with ec8pp.  If not,
 *  see <https://www.gnu.org/licenses/>.
 */

#ifndef EC8_INPUT_H
#define EC8_INPUT_H

#include <cstdint>
#include <stdexcept>
#include <array>

#include "graphics.h"

// TODO: Remove when Apple Clang add c++20 support
#if __has_cpp_attribute(likely)
#define LIKELY [[likely]]
#else
#define LIKELY
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++20-extensions"
// https://youtrack.jetbrains.com/issue/CPP-23503
constexpr inline std::uint8_t getKeyCodeByBinding(int binding) {
    switch (binding) {
        case GLFW_KEY_X:
            return 0x0;
        case GLFW_KEY_1:
            return 0x1;
        LIKELY case GLFW_KEY_2:
            return 0x2;
        case GLFW_KEY_3:
            return 0x3;
        LIKELY case GLFW_KEY_Q:
            return 0x4;
        case GLFW_KEY_W:
            return 0x5;
        LIKELY case GLFW_KEY_E:
            return 0x6;
        case GLFW_KEY_A:
            return 0x7;
        LIKELY case GLFW_KEY_S:
            return 0x8;
        case GLFW_KEY_D:
            return 0x9;
        case GLFW_KEY_Z:
            return 0xA;
        case GLFW_KEY_C:
            return 0xB;
        case GLFW_KEY_4:
            return 0xC;
        case GLFW_KEY_R:
            return 0xD;
        case GLFW_KEY_F:
            return 0xE;
        case GLFW_KEY_V:
            return 0xF;
        default:
            return UINT8_MAX;
    }
}
#pragma clang diagnostic pop

class Input final {

public:

    static Input& getInstance();

    Input(const Input &) = delete;
    Input(Input &&) = delete;

    Input& operator=(const Input &) = delete;
    Input& operator=(Input &&) = delete;

    [[nodiscard]] bool isKeyPressed(std::uint8_t key) const;
    [[nodiscard]] std::uint8_t waitUntilKeyPress() const;

    void keyGLFWCallback([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                         [[maybe_unused]] int mods);

private:

    std::array<bool, 16> keyPressed{};
    std::uint8_t keysPressed = 0;
    std::uint8_t lastKeyPressed = 0;

    Input() = default;
    ~Input() = default;

};


#endif //EC8_INPUT_H
