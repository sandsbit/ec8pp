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

    game = static_cast<uint16_t *>(malloc(size));
    gamef.read(reinterpret_cast<char *>(game), size);
}

Emulator::~Emulator() {
    free(game);
    free(memory);
}
