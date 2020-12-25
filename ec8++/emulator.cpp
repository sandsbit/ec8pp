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

#include "emulator.h"

#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <type_traits>
#include <sstream>
#include <random>

#define INVALID_INSTRUCTION "Invalid instruction at " + numberToHexString(2*(PC - game))

template <typename T>
typename std::enable_if<std::is_arithmetic_v<T>, std::string>::type numberToHexString(T x) {
    std::stringstream stream;
    stream << std::setfill ('0') << std::setw(sizeof(T)*2)
    << std::showbase << std::hex << x;
    return stream.str();
}

Emulator::Emulator(const std::filesystem::path &file) {
    loadGame(file);
    PC = game;

    memory = malloc(4 * 1024);
    loadFontInMemory();
}

void Emulator::loadFontInMemory() {
    std::ifstream font("assets/font.bin", std::ios::ate | std::ios::binary);
    std::streamsize size = font.tellg();
    font.seekg(0, std::ios::beg);

    font.read(static_cast<char *>(memory), size);
}

void Emulator::loadGame(const std::filesystem::path &file) {
    std::ifstream gamef(file, std::ios::ate | std::ios::binary);
    std::streamsize size = gamef.tellg();
    gamef.seekg(0, std::ios::beg);

    game = static_cast<uint16_t *>(malloc(static_cast<size_t>(size)));
    gameEnd = game + size/2;
    gamef.read(reinterpret_cast<char *>(game), size);
}

Emulator::~Emulator() {
    free(game);
    free(memory);
}

void Emulator::initEmulatorThread() {
    quit = false;
    emulatorThread = std::thread(&Emulator::loop, this);
}

void Emulator::joinEmulatorThread() {
    emulatorThread.join();
}

void Emulator::quitEmulatorThread() {
    quit = true;
}

void Emulator::SYS([[maybe_unused]] std::uint16_t addr) {}

void Emulator::CLS() {
    graphics->clearScreen();
}

#define NDEBUG

void Emulator::RET() {
#ifdef NDEBUG
    if (stack.empty())
        throw std::runtime_error(INVALID_INSTRUCTION + ": \"Cannot return: not in subroutine\"");
#endif
    PC = stack.top();
    stack.pop();
}

void Emulator::JP(std::uint16_t addr) {
#ifdef NDEBUG
    if (addr > 4*1024)
        throw std::runtime_error(INVALID_INSTRUCTION + ": \"Invalid address\"");
#endif
    PC = game + addr;
}

void Emulator::CALL(std::uint16_t addr) {
#ifdef NDEBUG
    if (addr > 4*1024)
        throw std::runtime_error(INVALID_INSTRUCTION + ": \"Invalid address\"");
#endif
    stack.push(PC + 1);
    PC = game + addr;
}

void Emulator::SE(std::uint8_t x, std::uint8_t byte) {
    assert(x <= 0xF);
    if (V[x] == byte)
        ++PC;
}

void Emulator::SNE(std::uint8_t x, std::uint8_t byte) {
    assert(x <= 0xF);
    if (V[x] != byte)
        ++PC;
}

void Emulator::SEXY(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    if (V[x] == V[y])
        ++PC;
}

void Emulator::LD(std::uint8_t x, std::uint8_t byte) {
    assert(x <= 0xF);
    V[x] = byte;
}

void Emulator::ADD(std::uint8_t x, std::uint8_t byte) {
    assert(x <= 0xF);
    V[x] += byte;
}

void Emulator::LDXY(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[x] = V[y];
}

void Emulator::OR(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[x] = V[x] | V[y];
}

void Emulator::AND(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[x] = V[x] & V[y];
}

void Emulator::XOR(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[x] = V[x] ^ V[y];
}

void Emulator::ADDXY(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[0xF] = __builtin_add_overflow(V[x], V[y], &V[x]);
}

void Emulator::SUB(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[0xF] = !__builtin_sub_overflow(V[x], V[y], &V[x]);
}

void Emulator::SHR(std::uint8_t x, [[maybe_unused]] std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[0xF] = x & 1;
    V[x] >>= 1;
}

void Emulator::SUBN(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[0xF] = !__builtin_sub_overflow(V[y], V[x], &V[x]);
}

void Emulator::SHL(std::uint8_t x, [[maybe_unused]] std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    V[0xF] = (x >> 7) & 1;
    V[x] <<= 1;
}

void Emulator::SNEXY(std::uint8_t x, std::uint8_t y) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    if (V[x] != V[y])
        ++PC;
}

void Emulator::LD(std::uint16_t addr) {
    I = static_cast<uint8_t *>(memory) + addr;
}

void Emulator::JPV0(std::uint16_t addr) {
    JP(addr + V[0]);
}

void Emulator::RND(std::uint8_t x, std::uint8_t byte) {
    assert(x <= 0xF);
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<std::uint8_t> uniform_dist(0, 255);

    V[x] = uniform_dist(e) & byte;
}

void Emulator::DRAW(std::uint8_t x, std::uint8_t y, std::uint8_t byte) {
    assert(x <= 0xF);
    assert(y <= 0xF);
    assert(byte <= 0xF);

    V[0xF] = graphics->drawSprite(V[x], V[y], byte, static_cast<void *>(I));
}

void Emulator::LDT(std::uint8_t x) {
    assert(x <= 0xF);
    V[x] = static_cast<std::uint8_t>(timers->getDelayTimerValue());
}

void Emulator::LDTSET(std::uint8_t x) {
    assert(x <= 0xF);
    timers->setDelayTimer(x);
}

void Emulator::LDATSET(std::uint8_t x) {
    assert(x <= 0xF);
    timers->setAudioTimer(x);
}

void Emulator::ADDI(std::uint8_t x) {
    assert(x <= 0xF);
    I += V[x];
}

void Emulator::LDISPR(std::uint8_t x) {
    assert(x <= 0xF);
    I = static_cast<uint8_t *>(memory) + V[x];
}

void Emulator::LDBCD(std::uint8_t x) {
    assert(x <= 0xF);
    *I = V[x] % 10;
    *(I+1) = (V[x] % 100 - *I) / 10;
    *(I+2) = (V[x] - *I - *(I + 1)) / 100;
}

void Emulator::LDREGMEM(std::uint8_t x) {
    assert(x <= 0xF);
    memcpy(I, V.data(), x + 1);
}

void Emulator::LDRREGMEM(std::uint8_t x) {
    assert(x <= 0xF);
    memcpy( V.data(), I, x + 1);
}

void Emulator::SKP(std::uint8_t x) {
    assert(x <= 0xF);
    if (input->isKeyPressed(V[x]))
        ++PC;
}

void Emulator::SKNP(std::uint8_t x) {
    assert(x <= 0xF);
    if (!input->isKeyPressed(V[x]))
        ++PC;
}

void Emulator::LDK(std::uint8_t x) {
    assert(x <= 0xF);
    V[x] = input->waitUntilKeyPress();
}

inline std::uint8_t majorFourBitsFromInstruction(std::uint16_t instruction) {
    return ((instruction) >> 12) & 0b1111;
}

inline std::uint16_t addressFromInstruction(std::uint16_t instruction) {
    return instruction & 0x0FFF;
}

inline std::uint8_t secondPositionRegisterFromInstruction(std::uint16_t instruction) {
    return (instruction >> 8) & 0b1111;
}

inline std::uint8_t thirdPositionRegisterFromInstruction(std::uint16_t instruction) {
    return (instruction >> 4) & 0b1111;
}

inline std::uint8_t byteFromInstructions(std::uint16_t instruction) {
    return instruction & 0xFF;
}

inline std::uint8_t nibbleFromInstructions(std::uint16_t instruction) {
    return instruction & 0xF;
}

void Emulator::loop() {
    while ((PC++) != gameEnd && !quit) {
        auto spr = secondPositionRegisterFromInstruction(*PC);
        auto tpr = thirdPositionRegisterFromInstruction(*PC);
        auto ni = nibbleFromInstructions(*PC);
        auto by = byteFromInstructions(*PC);
        switch (majorFourBitsFromInstruction(*PC)) {
            case 0x0:
                switch (*PC) {
                    case 0x00E0:
                        CLS();
                        break;
                    case 0x00EE:
                        RET();
                        break;
                    default:
                        SYS(*PC);
                }
                break;
            case 0x1:
                JP(addressFromInstruction(*PC));
                break;
            case 0x2:
                CALL(addressFromInstruction(*PC));
                break;
            case 0x3:
                SE(spr, by);
                break;
            case 0x4:
                SNE(spr, by);
                break;
            case 0x5:
                assert(ni == 0);  // TODO: throw
                SEXY(spr, tpr);
                break;
            case 0x6:
                LD(spr, by);
                break;
            case 0x7:
                ADD(spr, by);
                break;
            case 0x8:
                switch (ni) {
                    case 0x0:
                        LDXY(spr, tpr);
                        break;
                    case 0x1:
                        OR(spr, tpr);
                        break;
                    case 0x2:
                        AND(spr, tpr);
                        break;
                    case 0x3:
                        XOR(spr, tpr);
                        break;
                    case 0x4:
                        ADDXY(spr, tpr);
                        break;
                    case 0x5:
                        SUB(spr, tpr);
                        break;
                    case 0x6:
                        SHR(spr, tpr);
                        break;
                    case 0x7:
                        SUBN(spr, tpr);
                        break;
                    case 0xE:
                        SHL(spr, tpr);
                        break;
                    default:
                        throw std::runtime_error(INVALID_INSTRUCTION);
                }
                break;
            case 0x9:
                assert(ni == 0);  // TODO: throw
                SNEXY(spr, tpr);
                break;
            case 0xA:
                LD(addressFromInstruction(*PC));
                break;
            case 0xB:
                JPV0(addressFromInstruction(*PC));
                break;
            case 0xC:
                RND(spr, by);
                break;
            case 0xD:
                DRAW(spr, tpr, ni);
                break;
            case 0xE:
                switch (by) {
                    case 0x9E:
                        SKP(spr);
                        break;
                    case 0xA1:
                        SKNP(spr);
                        break;
                    default:
                        throw std::runtime_error(INVALID_INSTRUCTION);
                }
                break;
            case 0xF:
                switch (by) {
                    case 0x07:
                        LDT(spr);
                        break;
                    case 0x0A:
                        LDK(spr);
                        break;
                    case 0x15:
                        LDTSET(spr);
                        break;
                    case 0x18:
                        LDATSET(spr);
                        break;
                    case 0x1E:
                        ADDI(spr);
                        break;
                    case 0x29:
                        LDISPR(spr);
                        break;
                    case 0x33:
                        LDBCD(spr);
                        break;
                    case 0x55:
                        LDREGMEM(spr);
                        break;
                    case 0x65:
                        LDRREGMEM(spr);
                        break;
                    default:
                        throw std::runtime_error(INVALID_INSTRUCTION);
                }
                break;
        }
    }
    graphics->quitGraphics();
}
