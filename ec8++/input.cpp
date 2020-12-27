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

#include <cstdint>
#include <mutex>
#include <thread>

#include "input.h"

#include "SDL2/SDL.h"

Input &Input::getInstance() {
    static Input instance;
    return instance;
}

bool Input::isKeyPressed(std::uint8_t key) const {
    return keyPressed[key];
}

std::uint8_t Input::waitUntilKeyPress() const {
    while (keysPressed == 0);
    return lastKeyPressed;
}

void Input::keyGLFWCallback([[maybe_unused]] GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
                            [[maybe_unused]] int mods) {
    auto code = getKeyCodeByBinding(key);
    if (code == UINT8_MAX)
        return;
    if (action == GLFW_PRESS) {
        ++keysPressed;
        keyPressed[code] = true;
        lastKeyPressed = code;
    } else if (action == GLFW_RELEASE) {
        --keysPressed;
        keyPressed[code] = false;
    }
}
