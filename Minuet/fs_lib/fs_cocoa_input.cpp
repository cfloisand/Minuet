//
//  fs_cocoa_input.cpp
//  Minuet
//
//  Created by Christian Floisand on 2022-10-25.
//

#include "fs_cocoa_input.h"
#include "fs_input.h"
#include "fs_lib.h"


fsRawInput*
fs_create_raw_input() {
    fsRawInput *rawInput = new fsRawInput;
    fs_memclear(rawInput->keyboardButtons, sizeof(rawInput->keyboardButtons));
    fs_memclear(rawInput->mouseButtons, sizeof(rawInput->mouseButtons));
    return rawInput;
}

void fs_delete_raw_input(fsRawInput *rawInput) {
    delete rawInput;
}

bool
fs_input_key_pressed(fsRawInput *rawInput, uint16_t keycode) {
    fsAssert(keycode < fsArrayCount(rawInput->keyboardButtons));
    return rawInput->keyboardButtons[keycode].pressed;
}

bool
fs_input_key_is_down(fsRawInput *rawInput, uint16_t keycode) {
    fsAssert(keycode < fsArrayCount(rawInput->keyboardButtons));
    return rawInput->keyboardButtons[keycode].isDown;
}

bool
fs_input_key_went_down(fsRawInput *rawInput, uint16_t keycode) {
    fsAssert(keycode < fsArrayCount(rawInput->keyboardButtons));
    return rawInput->keyboardButtons[keycode].wentDown;
}

bool
fs_input_mouse_pressed(fsRawInput *rawInput, uint16_t keycode) {
    fsAssert(keycode < fsArrayCount(rawInput->mouseButtons));
    return rawInput->mouseButtons[keycode].pressed;
}

bool
fs_input_mouse_is_down(fsRawInput *rawInput, uint16_t keycode) {
    fsAssert(keycode < fsArrayCount(rawInput->mouseButtons));
    return rawInput->mouseButtons[keycode].isDown;
}

bool
fs_input_mouse_went_down(fsRawInput *rawInput, uint16_t keycode) {
    fsAssert(keycode < fsArrayCount(rawInput->mouseButtons));
    return rawInput->mouseButtons[keycode].wentDown;
}

fsPoint
fs_input_get_mouse_position(fsRawInput *rawInput) {
    fsPoint pos = { rawInput->mouseX, rawInput->mouseY };
    return pos;
}

fsPoint
fs_input_get_scroll_wheel_deltas(fsRawInput *rawInput) {
    fsPoint deltas = { rawInput->scrollWheelDeltaX, rawInput->scrollWheelDeltaY };
    return deltas;
}

fsInputBuffer*
fs_create_input_buffer(int16_t size, fsButtonState *buttonState) {
    fsInputBuffer *buffer = new fsInputBuffer(size, buttonState);
    return buffer;
}

void
fs_delete_input_buffer(fsInputBuffer *inputBuffer) {
    delete inputBuffer;
}

fsButtonState*
fs_get_keyboard_state(fsRawInput *rawInput) {
    return rawInput->keyboardButtons;
}

fsButtonState*
fs_get_mouse_state(fsRawInput *rawInput) {
    return rawInput->mouseButtons;
}

void
fs_process_input_event(fsInputBuffer *inputBuffer, int16_t buttonIndex, bool isDown, bool isRepeat) {
    inputBuffer->process(buttonIndex, isDown, isRepeat);
}

void
fs_update_mouse_position(fsRawInput *rawInput, float x, float y) {
    rawInput->mouseX = x;
    rawInput->mouseY = y;
}

void
fs_update_scroll_wheel_deltas(fsRawInput *rawInput, float dx, float dy) {
    rawInput->scrollWheelDeltaX = dx;
    rawInput->scrollWheelDeltaY = dy;
}

void
fs_reset_input_buffer(fsInputBuffer *inputBuffer) {
    inputBuffer->reset();
}

void
fs_reset_input(fsRawInput *rawInput) {
    rawInput->scrollWheelDeltaX = 0.f;
    rawInput->scrollWheelDeltaY = 0.f;
}
