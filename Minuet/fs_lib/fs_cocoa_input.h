//
//  fs_cocoa_input.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-25.
//

#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fsInputBuffer;
typedef struct fsInputBuffer fsInputBuffer;
struct fsButtonState;
typedef struct fsButtonState fsButtonState;
struct fsRawInput;
typedef struct fsRawInput fsRawInput;

union fsPoint {
    struct { float x, y; };
    struct { float dx, dy; };
};
typedef union fsPoint fsPoint;


/*! @brief Allocates a new instance of \c fsRawInput. The caller is responsible for calling \c fs_delete_raw_input when the instance is no longer needed. */
fsRawInput* fs_create_raw_input();

/*! @brief Deallocates the given \c fsRawInput instance. */
void fs_delete_raw_input(fsRawInput *rawInput);

/*! @brief Returns true if the given keyboard keycode was pressed. */
bool fs_input_key_pressed(fsRawInput *rawInput, uint16_t keycode);

/*! @brief Returns true if the given keyboard keycode is down. */
bool fs_input_key_is_down(fsRawInput *rawInput, uint16_t keycode);

/*! @brief Returns true if the given keyboard keycode went down. */
bool fs_input_key_went_down(fsRawInput *rawInput, uint16_t keycode);

/*! @brief Returns true if the given mouse keycode was pressed. */
bool fs_input_mouse_pressed(fsRawInput *rawInput, uint16_t keycode);

/*! @brief Returns true if the given mouse keycode is down. */
bool fs_input_mouse_is_down(fsRawInput *rawInput, uint16_t keycode);

/*! @brief Returns true if the given mouse keycode went down. */
bool fs_input_mouse_went_down(fsRawInput *rawInput, uint16_t keycode);

/*! @brief Returns the current mouse position. */
fsPoint fs_input_get_mouse_position(fsRawInput *rawInput);

/*! @brief Returns the current scroll wheel deltas. */
fsPoint fs_input_get_scroll_wheel_deltas(fsRawInput *rawInput);

/*! @brief Allocates an input buffer with the given size and associated button state. The size of the input buffer dictates how many simultaneous button states can exist before any additional states are discarded. */
fsInputBuffer* fs_create_input_buffer(int16_t size, fsButtonState *buttonState);

/*! @brief Deallocates the given \c fsInputBuffer instance. */
void fs_delete_input_buffer(fsInputBuffer *inputBuffer);

/*! @brief Returns the keyboard buttons state for the given raw input. */
fsButtonState* fs_get_keyboard_state(fsRawInput *rawInput);

/*! @brief Returns the mouse buttons state for the given raw input. */
fsButtonState* fs_get_mouse_state(fsRawInput *rawInput);

/*! @brief Process an input event with the given input buffer and button state. */
void fs_process_input_event(fsInputBuffer *inputBuffer, int16_t buttonIndex, bool isDown, bool isRepeat);

/*! @brief Updates the mouse position for the given raw input instance to the given x,y values. */
void fs_update_mouse_position(fsRawInput *rawInput, float x, float y);

/*! @brief Updates the mouse's scroll wheel deltas given dx,dy values. */
void fs_update_scroll_wheel_deltas(fsRawInput *rawInput, float dx, float dy);

/*! @brief Resets the given input buffer. */
void fs_reset_input_buffer(fsInputBuffer *inputBuffer);

/*! @brief Resets the given \c fsRawInput instance. */
void fs_reset_input(fsRawInput *input);

#ifdef __cplusplus
}
#endif
