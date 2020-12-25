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

#import <AppKit/AppKit.h>

#import "dialog.h"

#warning "The macOS implementation of dialog.h isn't working at the moment. Will be fixed in future version"

const char *openFileDialog(void) {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:YES];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];

    if ([panel runModal] == NSModalResponseOK) {
        NSURL *URL = [panel URL];
        return [URL.absoluteString UTF8String];
    } else {
        errorMessageDialog("File wasn't selected!");
        return NULL;
    }
}

void errorMessageDialog(const char *msg) {
    NSString *description = [NSString stringWithUTF8String:msg];
    NSError *error = [NSError errorWithDomain:@"me.nikitaserba.ec8pp" code:1 userInfo:@{@"Error reason": description}];
    NSAlert *alert = [NSAlert alertWithError:error];
    [alert runModal];
}