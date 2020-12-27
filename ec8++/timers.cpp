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

#include <mutex>
#include <utility>
#include <thread>
#include <chrono>
#include <algorithm>

#include "openal.h"

#include "timers.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

static std::string describeAlError(ALenum error_code) {
    switch (error_code) {
        case AL_INVALID_NAME:
            return "a bad name (ID) was passed to an OpenAL function (AL_INVALID_NAME)";
        case AL_INVALID_ENUM:
            return "an invalid enum value was passed to an OpenAL function (AL_INVALID_ENUM)";
        case AL_INVALID_VALUE:
            return "an invalid value was passed to an OpenAL function (AL_INVALID_VALUE)";
        case AL_INVALID_OPERATION:
            return "the requested operation is not valid (AL_INVALID_OPERATION)";
        case AL_OUT_OF_MEMORY:
            return "the requested operation resulted in OpenAL running out of memory (AL_OUT_OF_MEMORY)";
        default:
            return "invalid error code - " + std::to_string(error_code) + "; please, contact LEngine"
                                                                          "developer and left a bug"
                                                                          "report";
    }
}

static void checkAlError(const std::string &msg) {
#ifndef NDEBUG
    ALenum error_code;
    if ((error_code = alGetError()) != AL_NO_ERROR) {
        std::string error_message = msg + ": " + describeAlError(error_code);
        throw std::runtime_error(error_message);
    }
#endif
}

std::string describeAlcError(ALCenum error_code) {
    switch (error_code) {
        case ALC_INVALID_DEVICE:
            return "a bad device was passed to an OpenAL function (ALC_INVALID_DEVICE)";
        case ALC_INVALID_CONTEXT:
            return "a bad context was passed to an OpenAL function (ALC_INVALID_CONTEXT)";
        case ALC_INVALID_ENUM:
            return "an unknown enum value was passed to an OpenAL function (ALC_INVALID_ENUM)";
        case ALC_INVALID_VALUE:
            return "an invalid value was passed to an OpenAL function (ALC_INVALID_VALUE)";
        case ALC_OUT_OF_MEMORY:
            return "the requested operation resulted in OpenAL running out of memory (ALC_OUT_OF_MEMORY)";
        default:
            return "invalid alc error code - " + std::to_string(error_code) + "; please, contact LEngine"
                                                                              "developer and left a bug"
                                                                              "report";
    }
}

void checkAlcError(ALCdevice *device, const std::string &msg) {
#ifndef NDEBUG
    ALCenum error_code;
    if ((error_code = alcGetError(device)) != ALC_NO_ERROR) {
        std::string error_message = msg + ": " + describeAlcError(error_code);
        throw std::runtime_error(error_message);
    }
#endif
}


Timers &Timers::getInstance() {
    static Timers instance;
    return instance;
}

void Timers::initAudioThread() {
    audioPlayThread = std::thread(&Timers::audioLoop, this);
    audioPlayThread.detach();
}

void Timers::joinAudioThread() {
    running.lock();
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
    std::lock_guard<std::mutex> lock(running);
    audioLoopInit();
    while (!quit) {
        std::unique_lock<std::mutex> auMutex(audioTimerMutex);
        if (!playing && std::chrono::high_resolution_clock::now() <= audioTimerFinalTime)
            startPlayBeep();
        if (playing && std::chrono::high_resolution_clock::now() > audioTimerFinalTime)
            stopPlayBeep();
        auMutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
    audioLoopDestroy();
}

void Timers::audioLoopInit() {
    device = alcOpenDevice(NULL);
    if (!device) {
        throw std::runtime_error("Error while opening alc device");
    }

    context = alcCreateContext(device, NULL);
    checkAlcError(device, "Cannot create new context");

    alcMakeContextCurrent(context);
    checkAlcError(device, "Cannot make new context current");

    alGenBuffers(1, &buffer);
    checkAlError("Cannot generate buffer");
    ALenum		format;
    ALsizei		size;
    ALsizei		freq;
    ALvoid*		data;
    alutLoadWAVFile(const_cast<char *>("assets/beep.wav"), &format, &data, &size, &freq);
    alBufferData(buffer, format, data, size, freq);
    checkAlError("Cannot load data to buffer");

    alGenSources(1, &source);
    checkAlError("Cannot generate source");
    alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
    checkAlError("Cannot link source to buffer");
    alSourcei(source, AL_LOOPING, AL_TRUE);
    checkAlError("Cannot set source looping to AL_TRUE");
}

void Timers::audioLoopDestroy() {
    alDeleteSources(1, &source);
    checkAlError("Cannot delete source");
    alDeleteBuffers(1, &buffer);
    checkAlError("Cannot delete buffer");
    alcMakeContextCurrent(NULL);
    checkAlcError(device, "Cannot remove current context");
    alcDestroyContext(context);
    checkAlcError(device, "Cannot destroy context");
    alcCloseDevice(device);
    checkAlcError(device, "Cannot close device");
}

void Timers::startPlayBeep() {
    playing = true;
    alSourcePlay(source);
    checkAlError("Cannot play source");
}

void Timers::stopPlayBeep() {
    playing = false;
    alSourcePause(source);
    checkAlError("Cannot pause source");
}

#pragma clang diagnostic pop