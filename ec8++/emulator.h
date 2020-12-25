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

#ifndef EC8_EMULATOR_H
#define EC8_EMULATOR_H

#include <array>
#include <cstdint>
#include <stack>
#include <filesystem>
#include <thread>
#include <atomic>

#include "graphics.h"
#include "timers.h"

#define ADDRESS std::uint16_t addr
#define REGISTER_AND_VALUE std::uint8_t x, std::uint8_t byte
#define REGISTER std::uint8_t x
#define TWO_REGISTERS std::uint8_t x, std::uint8_t y
#define TWO_REGISTERS_AND_VALUE std::uint8_t x, std::uint8_t y, std::uint8_t byte
#define VOID void

class Emulator final {

public:

    explicit Emulator(const std::filesystem::path &file);
    ~Emulator();

    Emulator(const Emulator &) = delete;
    Emulator(Emulator &&) = delete;

    Emulator& operator=(const Emulator &) = delete;
    Emulator& operator=(Emulator &&) = delete;

    void initEmulatorThread();
    void joinEmulatorThread();
    void quitEmulatorThread();

    void SYS([[maybe_unused]] ADDRESS);
    void CLS(VOID);
    void RET(VOID);
    void JP(ADDRESS);
    void CALL(ADDRESS);
    void SE(REGISTER_AND_VALUE);
    void SNE(REGISTER_AND_VALUE);
    void SEXY(TWO_REGISTERS);
    void LD(REGISTER_AND_VALUE);
    void ADD(REGISTER_AND_VALUE);
    void LDXY(TWO_REGISTERS);
    void OR(TWO_REGISTERS);
    void AND(TWO_REGISTERS);
    void XOR(TWO_REGISTERS);
    void ADDXY(TWO_REGISTERS);
    void SUB(TWO_REGISTERS);
    void SHR(TWO_REGISTERS);
    void SUBN(TWO_REGISTERS);
    void SHL(TWO_REGISTERS);
    void SNEXY(TWO_REGISTERS);
    void LD(ADDRESS);
    void JPV0(ADDRESS);
    void RND(REGISTER_AND_VALUE);
    void DRAW(TWO_REGISTERS_AND_VALUE);
    void SKP(REGISTER);
    void SKNP(REGISTER);
    void LDT(REGISTER);
    void LDK(REGISTER);
    void LDTSET(REGISTER);
    void LDATSET(REGISTER);
    void ADDI(REGISTER);
    void LDISPR(REGISTER);
    void LDBCD(REGISTER);
    void LDREGMEM(REGISTER);
    void LDRREGMEM(REGISTER);

private:

    std::uint16_t *game;

    std::stack<void *> stack;
    std::uint16_t *PC;

    void *memory;
    std::array<std::uint8_t, 16> V;
    std::uint16_t I;

    Graphics * const graphics = &Graphics::getInstance();
    Timers * const timers = &Timers::getInstance();

    std::thread emulatorThread;
    std::atomic_bool quit;

    void loadFontInMemory();
    void loadGame(const std::filesystem::path &file);

    void loop();

};


#endif //EC8_EMULATOR_H
