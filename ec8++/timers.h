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

#ifndef EC8_TIMERS_H
#define EC8_TIMERS_H

#include <utility>
#include <atomic>
#include <thread>
#include <mutex>
#include <chrono>
#include <cstdint>
#include <ratio>

#include "openal.h"

class Timers final {

public:

    typedef std::chrono::duration<std::int64_t, std::ratio<1, 60>> timer_value_t;

    static Timers &getInstance();

    Timers(const Timers &) = delete;
    Timers(const Timers &&) = delete;

    Timers& operator=(const Timers &) = delete;
    Timers& operator=(const Timers &&) = delete;

    void initAudioThread();
    void joinAudioThread();
    void closeAudioThread();

    std::size_t getDelayTimerValue() const;

    void setDelayTimer(std::size_t value);
    void setAudioTimer(std::size_t value);

private:

    std::chrono::time_point<std::chrono::high_resolution_clock> delayTimerFinalTime{};
    std::chrono::time_point<std::chrono::high_resolution_clock> audioTimerFinalTime{};
    std::mutex audioTimerMutex;

    std::thread audioPlayThread;
    volatile std::atomic_bool quit = false;
    std::mutex running;

    ALCdevice *device;
    ALCcontext *context;
    ALuint buffer = 0;
    ALuint source = 0;

    void audioLoop();
    void audioLoopInit();
    void audioLoopDestroy();

    bool playing = false;
    void startPlayBeep();
    void stopPlayBeep();

    Timers() = default;
    ~Timers() = default;

};


#endif //EC8_TIMERS_H
