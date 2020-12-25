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
    const std::lock_guard<std::mutex> lock(keyPressedMutex);
    return keyPressed[key];
}

void Input::initInputThread() {
    inputThread = std::thread(&Input::loop, this);
}

void Input::quitInputThread() {
    quit = true;
}

void Input::joinInputThread() {
    inputThread.join();
}

std::uint8_t Input::waitUntilKeyPress() const {
    while (keysPressed == 0);
    return lastKeyPressed;
}

void Input::loop() {
    SDL_Init(SDL_INIT_EVENTS);
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (quit)
                goto quit;  // OH NO! What a bad developer. We'd better use java :/
            std::uint8_t key;
            switch (event.type) {
                case SDL_KEYDOWN:
                    key = getKeyCodeByBinding(static_cast<SDL_KeyCode>(event.key.keysym.sym));
                    if (key != UINT8_MAX) {
                        ++keysPressed;
                        lastKeyPressed = key;
                        keyPressedMutex.lock();
                        keyPressed[key] = true;
                        keyPressedMutex.unlock();
                    }
                    break;
                case SDL_KEYUP:
                    key = getKeyCodeByBinding(static_cast<SDL_KeyCode>(event.key.keysym.sym));
                    if (key != UINT8_MAX) {
                        --keysPressed;
                        keyPressedMutex.lock();
                        keyPressed[key] = false;
                        keyPressedMutex.unlock();
                    }
                    break;
            }
        }
    }
    quit:
    SDL_Quit();
}
