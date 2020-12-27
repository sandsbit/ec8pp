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

#include <filesystem>
#include <cstring>

#include "dialog.h"
#include "emulator.h"
#include "graphics.h"
#include "timers.h"

int main(int argc, char **argv) {
    if (argc > 2)
        errorMessageDialog("Invalid number of arguments!");

    std::filesystem::path file;
    if (argc == 2)
        file = argv[1];
    else
        file = openFileDialog();

    Timers::getInstance().initAudioThread();

    Emulator emulator(file);

    bool fullscreen = (argc >= 2 && strcmp(argv[1], "-fs") == 0)
            || (argc >= 3 && (strcmp(argv[2], "-fs") == 0 || strcmp(argv[1], "-fs") == 0));

    Graphics &graphics = Graphics::getInstance();
    graphics.init(&emulator, fullscreen);

    emulator.initEmulatorThread();

    graphics.loop();

    return 0;
}
