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

#ifndef EC8_INPUT_H
#define EC8_INPUT_H

#include <cstdint>

#ifdef __cpp_consteval
consteval
#else
constexpr
#endif
inline auto getKeyBinding(std::uint8_t key);

class Input final {

public:

    static Input& getInstance();

    Input(const Input &) = delete;
    Input(Input &&) = delete;

    Input& operator=(const Input &) = delete;
    Input& operator=(Input &&) = delete;

    [[nodiscard]] bool isKeyPressed(std::uint8_t key) const;
    [[nodiscard]] std::uint8_t waitUntilKeyPress() const;

private:

    Input();
    ~Input();

};


#endif //EC8_INPUT_H
