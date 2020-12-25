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
#include <thread>
#include <mutex>
#include <array>
#include <atomic>

#include "SDL2/SDL.h"

// TODO: Remove when Apple Clang add c++20 support
#if __has_cpp_attribute(likely)
#define LIKELY [[likely]]
#else
#define LIKELY
#endif

constexpr inline std::uint8_t getKeyCodeByBinding(SDL_KeyCode binding) {
    switch (binding) {
        case SDLK_KP_0:
            return 0x0;
        case SDLK_KP_1:
            return 0x7;
        LIKELY case SDLK_KP_2:
            return 0x8;
        case SDLK_KP_3:
            return 0x9;
        LIKELY case SDLK_KP_4:
            return 0x4;
        case SDLK_KP_5:
            return 0x5;
        LIKELY case SDLK_KP_6:
            return 0x6;
        case SDLK_KP_7:
            return 0x3;
        LIKELY case SDLK_KP_8:
            return 0x2;
        case SDLK_KP_9:
            return 0x1;
        case SDLK_KP_PERIOD:
            return 0xA;
        case SDLK_KP_ENTER:
            return 0xB;
        case SDLK_KP_DIVIDE:
            return 0xC;
        case SDLK_KP_MULTIPLY:
            return 0xD;
        case SDLK_KP_MINUS:
            return 0xE;
        case SDLK_KP_PLUS:
            return 0xF;
        default:
            return UINT8_MAX;
    }
}

class Input final {

public:

    static Input& getInstance();

    Input(const Input &) = delete;
    Input(Input &&) = delete;

    Input& operator=(const Input &) = delete;
    Input& operator=(Input &&) = delete;

    void initInputThread();
    void quitInputThread();
    void joinInputThread();

    [[nodiscard]] bool isKeyPressed(std::uint8_t key) const;
    [[nodiscard]] std::uint8_t waitUntilKeyPress() const;

private:

    void loop();

    std::thread inputThread;
    std::atomic_bool quit = false;

    std::array<bool, 16> keyPressed{};
    mutable std::mutex keyPressedMutex;
    std::atomic_uint8_t keysPressed = 0;
    std::atomic_uint8_t lastKeyPressed = 0;

    Input() = default;
    ~Input() = default;

};


#endif //EC8_INPUT_H
