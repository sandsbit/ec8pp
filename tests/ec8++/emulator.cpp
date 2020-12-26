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

#include "doctest.h"

#include "emulator_functions.h"

TEST_CASE("Test parsing singe instruction bits") {
    // they are randomly generated, hope they are all different
    std::uint16_t ex1 = 0b1011011011011011;
    std::uint16_t ex2 = 0b0001001001001111;
    std::uint16_t ex3 = 0b1011001100101001;
    std::uint16_t ex4 = 0b1111011001011101;
    std::uint16_t ex5 = 0b1110011010100111;
    std::uint16_t ex6 = 0b0000101000110110;
    std::uint16_t ex7 = 0b1111110111010010;
    std::uint16_t ex8 = 0b1101101001101110;
    std::uint16_t ex9 = 0b0101110100011100;

    SUBCASE("test getting major 4 bits") {
        CHECK_EQ(majorFourBitsFromInstruction(ex1), 0b1011);
        CHECK_EQ(majorFourBitsFromInstruction(ex2), 0b0001);
        CHECK_EQ(majorFourBitsFromInstruction(ex3), 0b1011);
        CHECK_EQ(majorFourBitsFromInstruction(ex4), 0b1111);
        CHECK_EQ(majorFourBitsFromInstruction(ex5), 0b1110);
        CHECK_EQ(majorFourBitsFromInstruction(ex6), 0b0000);
        CHECK_EQ(majorFourBitsFromInstruction(ex7), 0b1111);
        CHECK_EQ(majorFourBitsFromInstruction(ex8), 0b1101);
        CHECK_EQ(majorFourBitsFromInstruction(ex9), 0b0101);
    }

    SUBCASE("test getting address from instruction") {
        CHECK_EQ(addressFromInstruction(ex1), 0b0000011011011011);
        CHECK_EQ(addressFromInstruction(ex2), 0b0000001001001111);
        CHECK_EQ(addressFromInstruction(ex3), 0b0000001100101001);
        CHECK_EQ(addressFromInstruction(ex4), 0b0000011001011101);
        CHECK_EQ(addressFromInstruction(ex5), 0b0000011010100111);
        CHECK_EQ(addressFromInstruction(ex6), 0b0000101000110110);
        CHECK_EQ(addressFromInstruction(ex7), 0b0000110111010010);
        CHECK_EQ(addressFromInstruction(ex8), 0b0000101001101110);
        CHECK_EQ(addressFromInstruction(ex9), 0b0000110100011100);
    }

    SUBCASE("test getting X register number from instruction") {
        CHECK_EQ(secondPositionRegisterFromInstruction(ex1), 0b0110);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex2), 0b0010);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex3), 0b0011);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex4), 0b0110);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex5), 0b0110);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex6), 0b1010);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex7), 0b1101);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex8), 0b1010);
        CHECK_EQ(secondPositionRegisterFromInstruction(ex9), 0b1101);
    }

    SUBCASE("test getting Y register number from instruction") {
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex1), 0b1101);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex2), 0b0100);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex3), 0b0010);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex4), 0b0101);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex5), 0b1010);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex6), 0b0011);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex7), 0b1101);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex8), 0b0110);
        CHECK_EQ(thirdPositionRegisterFromInstruction(ex9), 0b0001);
    }

    SUBCASE("test getting last byte from instruction") {
        CHECK_EQ(byteFromInstructions(ex1), 0b11011011);
        CHECK_EQ(byteFromInstructions(ex2), 0b01001111);
        CHECK_EQ(byteFromInstructions(ex3), 0b00101001);
        CHECK_EQ(byteFromInstructions(ex4), 0b01011101);
        CHECK_EQ(byteFromInstructions(ex5), 0b10100111);
        CHECK_EQ(byteFromInstructions(ex6), 0b00110110);
        CHECK_EQ(byteFromInstructions(ex7), 0b11010010);
        CHECK_EQ(byteFromInstructions(ex8), 0b01101110);
        CHECK_EQ(byteFromInstructions(ex9), 0b00011100);
    }

    SUBCASE("test getting nibble (last 4 bits) from instruction") {
        CHECK_EQ(nibbleFromInstructions(ex1), 0b1011);
        CHECK_EQ(nibbleFromInstructions(ex2), 0b1111);
        CHECK_EQ(nibbleFromInstructions(ex3), 0b1001);
        CHECK_EQ(nibbleFromInstructions(ex4), 0b1101);
        CHECK_EQ(nibbleFromInstructions(ex5), 0b0111);
        CHECK_EQ(nibbleFromInstructions(ex6), 0b0110);
        CHECK_EQ(nibbleFromInstructions(ex7), 0b0010);
        CHECK_EQ(nibbleFromInstructions(ex8), 0b1110);
        CHECK_EQ(nibbleFromInstructions(ex9), 0b1100);
    }
}
