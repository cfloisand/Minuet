/*  fs_input.h - Flyingsand input
 *  v. 0.1
 */

#pragma once
#pragma warning(disable : 4068) // NOTE(christian): Unknown pragma.

#include <stdint.h>


#pragma mark - Keyboard & mouse

struct fsButtonState {
    bool isDown;    // True while button is being held down.
    bool wentDown;  // True only on the first frame the button goes down.
    bool pressed;   // True only on the first frame the button is released.
};

struct fsRawInput {
    union {
        // NOTE: Indexes/keys map to virtual key codes.
        
#if defined(__APPLE__) && (__MACH__)
        fsButtonState keyboardButtons[118];
        struct {
            fsButtonState A;
            fsButtonState S;
            fsButtonState D;
            fsButtonState F;
            fsButtonState H;
            fsButtonState G;
            fsButtonState Z;
            fsButtonState X;
            fsButtonState C;
            fsButtonState V;
            
            fsButtonState ISO;
            
            fsButtonState B;
            fsButtonState Q;
            fsButtonState W;
            fsButtonState E;
            fsButtonState R;
            fsButtonState Y;
            fsButtonState T;
            fsButtonState Num1;
            fsButtonState Num2;
            fsButtonState Num3;
            fsButtonState Num4;
            fsButtonState Num6;
            fsButtonState Num5;
            fsButtonState Equal;
            fsButtonState Num9;
            fsButtonState Num7;
            fsButtonState Minus;
            fsButtonState Num8;
            fsButtonState Num0;
            fsButtonState RBracket;
            fsButtonState O;
            fsButtonState U;
            fsButtonState LBracket;
            fsButtonState I;
            fsButtonState P;
            
            fsButtonState Return;
            
            fsButtonState L;
            fsButtonState J;
            fsButtonState Quote;
            fsButtonState K;
            fsButtonState Semicolon;
            fsButtonState Backslash;
            fsButtonState Comma;
            fsButtonState Slash;
            fsButtonState N;
            fsButtonState M;
            fsButtonState Period;
            fsButtonState Tab;
            fsButtonState Space;
            fsButtonState Grave;
            fsButtonState Delete;
            
            fsButtonState _unused0;
            
            fsButtonState Escape;
            fsButtonState RightCommand;
            fsButtonState Command;
            fsButtonState Shift;
            fsButtonState CapsLock;
            fsButtonState Option;
            fsButtonState Control;
            fsButtonState RShift;
            fsButtonState ROption;
            fsButtonState RControl;
            fsButtonState Function;
            fsButtonState F17;
            fsButtonState KeypadDecimal;
            
            fsButtonState _unused2;
            
            fsButtonState KeypadMultiply;
            
            fsButtonState _unused3;
            
            fsButtonState KeypadPlus;
            
            fsButtonState _unused4;
            
            fsButtonState KeypadClear;
            fsButtonState VolumeUp;
            fsButtonState VolumeDown;
            fsButtonState Mute;
            fsButtonState KeypadDivide;
            fsButtonState KeypadEnter;
            
            fsButtonState _unused5;
            
            fsButtonState KeypadMinus;
            fsButtonState F18;
            fsButtonState F19;
            fsButtonState KeypadEquals;
            fsButtonState Keypad0;
            fsButtonState Keypad1;
            fsButtonState Keypad2;
            fsButtonState Keypad3;
            fsButtonState Keypad4;
            fsButtonState Keypad5;
            fsButtonState Keypad6;
            fsButtonState Keypad7;
            fsButtonState F20;
            fsButtonState Keypad8;
            fsButtonState Keypad9;
            
            fsButtonState JIS_Yen;
            fsButtonState JIS_Underscore;
            fsButtonState JIS_KeypadComma;
            
            fsButtonState F5;
            fsButtonState F6;
            fsButtonState F7;
            fsButtonState F3;
            fsButtonState F8;
            fsButtonState F9;
            
            fsButtonState JIS_Eisu;
            
            fsButtonState F11;
            
            fsButtonState JIS_Kana;
            
            fsButtonState F13;
            fsButtonState F16;
            fsButtonState F14;
            
            fsButtonState _unused6;
            
            fsButtonState F10;
            
            fsButtonState _unused7;
            
            fsButtonState F12;
            
            fsButtonState _unused8;
            
            fsButtonState F15;
            fsButtonState Help;
            fsButtonState Home;
            fsButtonState PageUp;
            fsButtonState ForwardDelete;
            fsButtonState F4;
            fsButtonState End;
            fsButtonState F2;
            fsButtonState PageDown;
            fsButtonState F1;
            fsButtonState LeftArrow;
            fsButtonState RightArrow;
            fsButtonState DownArrow;
            fsButtonState UpArrow;
        };
        
#elif defined(_WIN32) || defined(_WIN64)
        fsButtonState keyboardButtons[215];
        struct {
            fsButtonState Backspace;
            fsButtonState Tab;
            
            fsButtonState _reserved0[2];
            
            fsButtonState Clear;
            fsButtonState Enter;
            
            fsButtonState _undefined0[2];
            
            fsButtonState Shift;
            fsButtonState Control;
            fsButtonState Alt;
            fsButtonState Pause;
            fsButtonState CapsLock;
            
            fsButtonState IMEHangul;
            
            fsButtonState _undefined1;
            
            fsButtonState IMEJunja;
            fsButtonState IMEFinal;
            fsButtonState IMEKanji;
            
            fsButtonState _undefined2;
            
            fsButtonState Esc;
            
            fsButtonState IMEConvert;
            fsButtonState IMENonConvert;
            fsButtonState IMEAccept;
            fsButtonState IMEChange;
            
            fsButtonState Space;
            fsButtonState PageUp;
            fsButtonState PageDown;
            fsButtonState End;
            fsButtonState Home;
            fsButtonState LeftArrow;
            fsButtonState UpArrow;
            fsButtonState RightArrow;
            fsButtonState DownArrow;
            fsButtonState Select;
            fsButtonState Print;
            fsButtonState Execute;
            fsButtonState PrintScreen;
            fsButtonState Insert;
            fsButtonState Delete;
            fsButtonState Help;
            
            fsButtonState Num0;
            fsButtonState Num1;
            fsButtonState Num2;
            fsButtonState Num3;
            fsButtonState Num4;
            fsButtonState Num5;
            fsButtonState Num6;
            fsButtonState Num7;
            fsButtonState Num8;
            fsButtonState Num9;
            
            fsButtonState _undefined3[7];
            
            fsButtonState A;
            fsButtonState B;
            fsButtonState C;
            fsButtonState D;
            fsButtonState E;
            fsButtonState F;
            fsButtonState G;
            fsButtonState H;
            fsButtonState I;
            fsButtonState J;
            fsButtonState K;
            fsButtonState L;
            fsButtonState M;
            fsButtonState N;
            fsButtonState O;
            fsButtonState P;
            fsButtonState Q;
            fsButtonState R;
            fsButtonState S;
            fsButtonState T;
            fsButtonState U;
            fsButtonState V;
            fsButtonState W;
            fsButtonState X;
            fsButtonState Y;
            fsButtonState Z;
            
            fsButtonState LWindows;
            fsButtonState RWindows;
            fsButtonState Applications;
            
            fsButtonState _reserved1;
            
            fsButtonState Sleep;
            
            fsButtonState Keypad0;
            fsButtonState Keypad1;
            fsButtonState Keypad2;
            fsButtonState Keypad3;
            fsButtonState Keypad4;
            fsButtonState Keypad5;
            fsButtonState Keypad6;
            fsButtonState Keypad7;
            fsButtonState Keypad8;
            fsButtonState Keypad9;
            
            fsButtonState Multiply;
            fsButtonState Add;
            fsButtonState Separator;
            fsButtonState Subract;
            fsButtonState Decimal;
            fsButtonState Divide;
            
            fsButtonState F1;
            fsButtonState F2;
            fsButtonState F3;
            fsButtonState F4;
            fsButtonState F5;
            fsButtonState F6;
            fsButtonState F7;
            fsButtonState F8;
            fsButtonState F9;
            fsButtonState F10;
            fsButtonState F11;
            fsButtonState F12;
            fsButtonState F13;
            fsButtonState F14;
            fsButtonState F15;
            fsButtonState F16;
            fsButtonState F17;
            fsButtonState F18;
            fsButtonState F19;
            fsButtonState F20;
            fsButtonState F21;
            fsButtonState F22;
            fsButtonState F23;
            fsButtonState F24;
            
            fsButtonState _unassigned0[8];
            
            fsButtonState NumLock;
            fsButtonState ScrollLock;
            
            fsButtonState OEM[5];
            
            fsButtonState _unassigned1[9];
            
            fsButtonState LShift;
            fsButtonState RShift;
            fsButtonState LControl;
            fsButtonState RControl;
            fsButtonState LMenu;
            fsButtonState RMenu;
            fsButtonState BrowserBack;
            fsButtonState BrowserForward;
            fsButtonState BrowserRefresh;
            fsButtonState BrowserStop;
            fsButtonState BrowserSearch;
            fsButtonState BrowserFavorites;
            fsButtonState BrowserStart;
            fsButtonState VolumeMute;
            fsButtonState VolumeDown;
            fsButtonState VolumeUp;
            fsButtonState NextTrack;
            fsButtonState PreviousTrack;
            fsButtonState StopMedia;
            fsButtonState PlayMedia;
            fsButtonState StartMail;
            fsButtonState SelectMedia;
            fsButtonState StartApplication1;
            fsButtonState StartApplication2;
            
            fsButtonState _reserved2[2];
            
            fsButtonState Semicolon;
            fsButtonState KeypadPlus;
            fsButtonState Comma;
            fsButtonState KeypadMinus;
            fsButtonState Period;
            fsButtonState Slash;
            fsButtonState Grave;
            
            fsButtonState _reserved3[23];
            fsButtonState _unassigned2[3];
            
            fsButtonState LBracket;
            fsButtonState Backslash;
            fsButtonState RBracket;
            fsButtonState Quote;
        };
#endif
        
    };
    
    union {
        fsButtonState mouseButtons[4];
        struct {
            fsButtonState mouseLeft;
            fsButtonState mouseRight;
            fsButtonState mouseMiddle;
            fsButtonState mouseOther;
        };
    };
    
    float mouseX;
    float mouseY;
    float scrollWheelDeltaX;
    float scrollWheelDeltaY;
};


/* NOTE:
    An input buffer remembers up to a specified length of button states (e.g. pressed, wentDown) that are reset at the end of each frame.
    This could be thought of as the number of simultaneous button states that can exist for one frame.
    The button state 'isDown' is not managed by an input buffer since it's state is trivially handled by down/up events.
 */

struct fsInputBuffer {
    fsInputBuffer(int16_t size, fsButtonState *buttonState);
    ~fsInputBuffer();
    
    void process(int16_t buttonIndex, bool isDown, bool isRepeat);
    void reset();
    
private:
    const int16_t _length;
    int8_t _index;
    int16_t * _buffer;
    fsButtonState * _buttonState;
};
