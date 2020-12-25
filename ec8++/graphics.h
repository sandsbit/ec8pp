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

#ifndef EC8_GRAPHICS_H
#define EC8_GRAPHICS_H

#include <utility>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <bitset>
#include <array>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Emulator;

/**
 * Render graphics using OpenGL.
 */
class Graphics final {

public:

    static constexpr const char *WINDOW_TITLE = "ec8++ - CHIP-8 emulator";
    int WIDTH;
    int HEIGHT;

    static Graphics& getInstance();

    Graphics(const Graphics &) = delete;
    Graphics(Graphics &&) = delete;

    Graphics& operator=(const Graphics &) = delete;
    Graphics& operator=(Graphics &&) = delete;

    void quitGraphics();

    /**
     * Init window.
     *
     * Should be called only once!
     */
    void init(Emulator *emulator, bool fullscreen = true, int width = 640);
    void loop();

    void clearScreen();
    std::uint8_t drawSprite(std::uint8_t x, std::uint8_t y, std::uint8_t height, void *sprite);


private:

    int PIXEL_WIDTH;

    GLFWwindow *window;
    std::mutex screenMutex;
    std::array<std::bitset<32>, 64> screen;

    Emulator *emulator;

    std::atomic_bool quit = false;

    Graphics();
    virtual ~Graphics();

};


#endif //EC8_GRAPHICS_H
