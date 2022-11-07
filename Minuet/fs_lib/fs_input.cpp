/*  fs_input.cpp - Flyingsand input
 *  v. 0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include "fs_input.h"


fsInputBuffer::fsInputBuffer(int16_t size, fsButtonState *buttonState)
    : _length(size)
    , _index(0)
    , _buttonState(buttonState)
{
    _buffer = new int16_t[size];
}

fsInputBuffer::~fsInputBuffer() {
    delete [] _buffer;
}

void
fsInputBuffer::process(int16_t buttonIndex, bool isDown, bool isRepeat) {
    _buttonState[buttonIndex].isDown = isDown;
    if (_index < _length - 1) {
        if (!isRepeat) {
            _buttonState[buttonIndex].wentDown = isDown;
            _buttonState[buttonIndex].pressed = !isDown;
        }
        _buffer[_index] = buttonIndex;
        _index++;
    }
}

void
fsInputBuffer::reset() {
    for (int8_t bufferIndex = 0; bufferIndex < _index; ++bufferIndex) {
        int16_t buttonIndex = _buffer[bufferIndex];
        _buttonState[buttonIndex].wentDown = false;
        _buttonState[buttonIndex].pressed = false;
    }
    _index = 0;
}
