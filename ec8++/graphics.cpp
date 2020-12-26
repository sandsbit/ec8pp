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

#include "graphics.h"

#include <iostream>
#include <stdexcept>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "emulator.h"
#include "timers.h"
#include "input.h"

static void errorCallback([[maybe_unused]] int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

Graphics &Graphics::getInstance() {
    static Graphics instance;
    return instance;
}

Graphics::Graphics() {
    if (!glfwInit())
        throw std::runtime_error("Could not init graphics!");

    glfwSetErrorCallback(errorCallback);
}

Graphics::~Graphics() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Graphics::init(Emulator *em, bool fullscreen, int width) {
    WIDTH = width;
    emulator = em;

    if (width % 64 != 0)
        throw std::runtime_error("Invalid resolution: width should be multiple of 64.");
    HEIGHT = width/2;
    PIXEL_WIDTH = WIDTH / 64;

    GLFWmonitor *monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, monitor, nullptr);
    if (!window)
        throw std::runtime_error("Could not create window!");

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        throw std::runtime_error("Could not initialize GLAD!");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-compare"
void Graphics::loop() {
    auto lastFPSCountTime = glfwGetTime();
    std::size_t frames = 0;

    while (!glfwWindowShouldClose(window) && !quit) {
        glfwPollEvents();

        ++frames;
        auto time = glfwGetTime();
        if (time - lastFPSCountTime >= 1.0)  {
            frames = static_cast<std::size_t>(static_cast<double>(frames) / (time - lastFPSCountTime));
            glfwSetWindowTitle(window, (std::string(WINDOW_TITLE) + " - " + std::to_string(frames) + " fps").c_str());

            frames = 0;
            lastFPSCountTime = time;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.2f, 0.3f, 0.3f);
        int i = -1;
        screenMutex.lock();
        for (const auto &line : screen) {
            int yOffset = (++i) * PIXEL_WIDTH;

            for (int j = 0; j < line.size(); ++j) {
                int xOffset = j * PIXEL_WIDTH;

                if (line[static_cast<size_t>(j)])
                    glRecti(xOffset, yOffset, xOffset + PIXEL_WIDTH, yOffset + PIXEL_WIDTH);
            }
        }
        screenMutex.unlock();

        glfwSwapBuffers(window);
    }

    emulator->quitEmulatorThread();
    emulator->joinEmulatorThread();

    Timers::getInstance().closeAudioThread();
    Timers::getInstance().joinAudioThread();

    Input::getInstance().quitInputThread();
    Input::getInstance().joinInputThread();
}
#pragma clang diagnostic pop

void Graphics::clearScreen() {
    screenMutex.lock();
    for (auto &line : screen)
        line.reset();
    screenMutex.unlock();
}

std::uint8_t Graphics::drawSprite(std::uint8_t x, std::uint8_t y, std::uint8_t height, void *sprite) {
    screenMutex.lock();
    bool collision;
    for (auto i = y; i < y + height; ++i) {
        std::uint8_t line = *(static_cast<std::uint8_t *>(sprite) + (i - y));
        for (auto j = x; j < x + 8; ++j) {
            std::uint8_t pixel = (line >> (7-(j - x))) & 0b1;
            collision = collision || (screen[i][j % 64] & pixel);
            screen[i][j % 64] = screen[i][j] ^ pixel;
        }
    }
    screenMutex.unlock();
    return collision;
}

void Graphics::quitGraphics() {
    quit = true;
}
