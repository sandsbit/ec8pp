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

#include <thread>
#include <chrono>
#include <algorithm>

#include "openal.h"

#include "timers.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

Timers &Timers::getInstance() {
    static Timers instance;
    return instance;
}

void Timers::initAudioThread() {
    audioPlayThread = std::thread(&Timers::audioLoop, this);
}

void Timers::joinAudioThread() {
    audioPlayThread.join();
}

void Timers::closeAudioThread() {
    quit = true;
}

std::size_t Timers::getDelayTimerValue() const {
    auto tv = std::chrono::duration_cast<timer_value_t>(delayTimerFinalTime
                                                            - std::chrono::high_resolution_clock::now()).count();
    if (tv < 0)
        return 0;
    else
        return static_cast<std::size_t>(tv);
}

void Timers::setDelayTimer(std::size_t value) {
    auto newValue = std::max(std::chrono::high_resolution_clock::now(), delayTimerFinalTime)
                + std::chrono::duration_cast<std::chrono::nanoseconds>(timer_value_t(value));
    delayTimerFinalTime = newValue;
}

void Timers::setAudioTimer(std::size_t value) {
    std::lock_guard<std::mutex> lock(audioTimerMutex);
    auto newValue = std::max(std::chrono::high_resolution_clock::now(), audioTimerFinalTime)
                    + std::chrono::duration_cast<std::chrono::nanoseconds>(timer_value_t(value));
    audioTimerFinalTime = newValue;
}

void Timers::audioLoop() {
    audioLoopInit();
    while (!quit) {
        audioTimerMutex.lock();
        if (!playing && std::chrono::high_resolution_clock::now() <= audioTimerFinalTime)
            startPlayBeep();
        if (playing && std::chrono::high_resolution_clock::now() > audioTimerFinalTime)
            stopPlayBeep();
        audioTimerMutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
    audioLoopDestroy();
}

void Timers::audioLoopInit() {
    alutInit(nullptr, nullptr);

    alGenBuffers(1, &buffer);
    ALenum		format;
    ALsizei		size;
    ALsizei		freq;
    ALvoid*		data;
    alutLoadWAVFile(const_cast<char *>("assets/beep.wav"), &format, &data, &size, &freq);
    alBufferData(buffer, format, data, size, freq);

    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
}

void Timers::audioLoopDestroy() {
    alDeleteBuffers(1, &buffer);
    alDeleteSources(1, &source);
    alutExit();
}

void Timers::startPlayBeep() {
    playing = true;
    alSourcei(source, AL_LOOPING, AL_TRUE);
    alSourcePlay(source);
}

void Timers::stopPlayBeep() {
    playing = false;
    alSourcePause(source);
}

#pragma clang diagnostic pop