//
//  fs_cocoa.swift
//  Minuet
//
//  Created by Christian Floisand on 2022-10-15.
//

import Cocoa
import FSCocoaInput


//# MARK: - Application

class FSCocoaApp {
    static private(set) var shared: FSCocoaApp!
    
    private var appDelegate: FSAppDelegate
    
    private var rawInput: OpaquePointer
    private var keyboardBuffer: OpaquePointer
    private var mouseBuffer: OpaquePointer
    
    
    private init(delegate: FSAppDelegate?) {
        assert(Thread.isMainThread, "Shared application must be initialized on main thread.")
        guard FSCocoaApp.shared == nil else {
            fatalError("Shared application can only be initialized once.")
        }
        
        appDelegate = delegate ?? FSAppDelegate()
        rawInput = fs_create_raw_input()
        let keyboardState = fs_get_keyboard_state(rawInput)
        let mouseState = fs_get_mouse_state(rawInput)
        keyboardBuffer = fs_create_input_buffer(16, keyboardState)
        mouseBuffer = fs_create_input_buffer(4, mouseState)
        FSCocoaApp.shared = self
    }
    
    /// Initializes and activates the shared application instance.
    /// - Returns: The shared application instance.
    static func initApp(delegate: FSAppDelegate? = nil) -> FSCocoaApp {
        let cocoaApp = FSCocoaApp(delegate: delegate)
        let app = NSApplication.shared
        if !app.setActivationPolicy(.regular) {
            debugPrint("WARNING: Failed to set activation policy.")
        }
        
        app.delegate = cocoaApp.appDelegate
        app.presentationOptions = []
        app.activate(ignoringOtherApps: true)
        app.finishLaunching()
        
        return cocoaApp
    }
    
    /// Terminates the shared application.
    func shutdown() {
        NSApp.terminate(nil)
    }
    
    /// Toggles full screen mode for the application's key window.
    func toggleFullScreen() {
        if let keyWindow = NSApp.keyWindow {
            keyWindow.toggleFullScreen(nil)
        } else {
            debugPrint("Cannot toggle full-screen; no key window.")
        }
    }
    
    /// Processes all current events for the shared application, including input.  This method should be called at the top of the main thread's loop.
    /// - Returns: A pointer to the raw input that represents a snapshot of the input state after processing all events in the queue.
    @discardableResult
    func processEvents() -> OpaquePointer {
        assert(Thread.isMainThread, "Processing application events must occur on main thread.")
        
        while true {
            guard let event = NSApp.nextEvent(matching: .any, until: nil, inMode: .default, dequeue: true) else {
                break
            }
            processEvent(event)
            NSApp.sendEvent(event)
            NSApp.updateWindows()
        }
        
        if let mainWindow = NSApp.mainWindow {
            let mouseLoc = mainWindow.mouseLocationOutsideOfEventStream
            fs_update_mouse_position(rawInput, Float(mouseLoc.x), Float(mouseLoc.y))
        }
        
        return rawInput
    }
    
    /// Resets the input state. This method should be called at the end of the main thread's loop.
    func resetInput() {
        assert(Thread.isMainThread, "Reset input must be called on main thread.")
        fs_reset_input_buffer(keyboardBuffer)
        fs_reset_input_buffer(mouseBuffer)
        fs_reset_input(rawInput)
    }
    
    /// Returns the pointer to the raw input instance, which represents a snapshot of the input state for a given frame.
    func getInput() -> OpaquePointer {
        return rawInput
    }
    
    /// Returns true/false based on whether the given keycode was pressed in the given frame.
    func isKeyPressed(keycode: UInt16) -> Bool {
        return fs_input_key_pressed(rawInput, keycode)
    }
    
    /// Returns true/false based on whether the given keycode is in the down state in the given frame.
    func isKeyDown(keycode: UInt16) -> Bool {
        return fs_input_key_is_down(rawInput, keycode)
    }
    
    /// Returns true/false based on whether the given keycode went down in the given frame.
    func keyWentDown(keycode: UInt16) -> Bool {
        return fs_input_key_went_down(rawInput, keycode)
    }
    
    /// Returns true/false based on whether the given mouse button was pressed in the given frame.
    func isMousePressed(button: MouseButtons) -> Bool {
        return fs_input_mouse_pressed(rawInput, button.rawValue)
    }
    
    /// Returns true/false based on whether the given mouse button is in the down state in the given frame.
    func isMouseDown(button: MouseButtons) -> Bool {
        return fs_input_mouse_is_down(rawInput, button.rawValue)
    }
    
    /// Returns true/false based on whether the given mouse button went down in the given frame.
    func mouseWentDown(button: MouseButtons) -> Bool {
        return fs_input_mouse_went_down(rawInput, button.rawValue)
    }
    
    /// Returns the current mouse position in the coordinate system of the application's main window.
    func getMousePosition() -> fsPoint {
        return fs_input_get_mouse_position(rawInput)
    }
    
    /// Returns the current scroll wheel deltas of the mouse.
    func getScrollWheelDeltas() -> fsPoint {
        return fs_input_get_scroll_wheel_deltas(rawInput)
    }
    
    private func processEvent(_ event: NSEvent) {
        var isMouseDownEvent = false
        var isKeyDownEvent = false
        
        switch event.type {
        case .leftMouseDown, .rightMouseDown, .otherMouseDown:
            isMouseDownEvent = true
            fallthrough
        case .leftMouseUp, .rightMouseUp, .otherMouseUp:
            fs_process_input_event(mouseBuffer, Int16(event.buttonNumber), isMouseDownEvent, false)
            
        case .scrollWheel:
            fs_update_scroll_wheel_deltas(rawInput, Float(event.scrollingDeltaX), Float(event.scrollingDeltaY))
        
        case .keyDown:
            isKeyDownEvent = true
            fallthrough
        case .keyUp:
            fs_process_input_event(keyboardBuffer, Int16(event.keyCode), isKeyDownEvent, event.isARepeat)
            
        case .flagsChanged:
            var isModifierDown = false
            if let keycode = Keycodes(rawValue: event.keyCode) {
                var mask: NSEvent.ModifierFlags
                switch keycode {
                case .vkShift, .vkRightShift:
                    mask = .shift
                case .vkControl, .vkRightControl:
                    mask = .control
                case .vkOption, .vkRightOption:
                    mask = .option
                case .vkCommand, .vkRightCommand:
                    mask = .command
                case .vkCapsLock:
                    mask = .capsLock
                default:
                    mask = []
                }
                
                isModifierDown = (NSEvent.modifierFlags.rawValue & mask.rawValue) == mask.rawValue
            }
            
            fs_process_input_event(keyboardBuffer, Int16(event.keyCode), isModifierDown, false)
        
        default:
            break
        }
    }
    
    /// Hides the cursor. Each call to this method must be balanced with a call to showCursor in order to make the cursor visible again.
    class func hideCursor() {
        NSCursor.hide()
    }
    
    /// Shows the cursor. Each call to this method must be balanced with calls to hideCursor in order for the cursor to correctly be shown/hidden.
    class func showCursor() {
        NSCursor.unhide()
    }
}

class FSAppDelegate: NSObject, NSApplicationDelegate {
    
    func applicationWillFinishLaunching(_ notification: Notification) {
        debugPrint("FSAppDelegate: will finish launching")
    }
    
    func applicationDidFinishLaunching(_ notification: Notification) {
        debugPrint("FSAppDelegate: did finish launching")
    }
    
    func applicationWillResignActive(_ notification: Notification) {
        debugPrint("FSAppDelegate: will resign active")
    }
    
    func applicationDidResignActive(_ notification: Notification) {
        debugPrint("FSAppDelegate: did resign active")
    }
    
    func applicationWillBecomeActive(_ notification: Notification) {
        debugPrint("FSAppDelegate: will become active")
    }
    
    func applicationDidBecomeActive(_ notification: Notification) {
        debugPrint("FSAppDelegate: did become active")
    }
    
    func applicationWillTerminate(_ notification: Notification) {
        debugPrint("FSAppDelegate: will terminate")
    }
    
}


//# MARK: - Window

class FSWindow: NSWindow {
    private let windowDelegate: FSWindowDelegate
    
    init(title: String?, contentRect: NSRect, styleMask: NSWindow.StyleMask, delegate: FSWindowDelegate?) {
        windowDelegate = delegate ?? FSWindowDelegate()
        super.init(contentRect: contentRect, styleMask: styleMask, backing: .buffered, defer: false)

        self.title = title ?? ""
        self.backgroundColor = .white
        self.acceptsMouseMovedEvents = true
        self.collectionBehavior = .fullScreenPrimary
        self.delegate = windowDelegate
    }
    
    override func keyDown(with event: NSEvent) {}
    override func keyUp(with event: NSEvent) {}
    
    override var acceptsFirstResponder: Bool {
        return true
    }
    
    override var canBecomeKey: Bool {
        return true
    }
    
    override var canBecomeMain: Bool {
        return true
    }
}

class FSWindowDelegate: NSObject, NSWindowDelegate {
    private var previousRect: NSRect?
    
    func windowDidBecomeKey(_ notification: Notification) {
        debugPrint("FSWindow: did become key")
    }
    
    func windowDidBecomeMain(_ notification: Notification) {
        debugPrint("FSWindow: did become main")
    }
    
    func windowDidResignKey(_ notification: Notification) {
        debugPrint("FSWindow: did resign key")
    }
    
    func windowDidResignMain(_ notification: Notification) {
        debugPrint("FSWindow: did resign main")
    }
    
    func windowWillEnterFullScreen(_ notification: Notification) {
        guard let window = notification.object as? FSWindow, let screenRect = NSScreen.main?.frame else {
            return
        }
        
        let size = NSSize(width: window.frame.size.width, height: window.frame.size.height)
        previousRect = NSRect(origin: window.frame.origin, size: size)
        window.setFrame(screenRect, display: true)
    }
    
    func windowDidEnterFullScreen(_ notification: Notification) {
        NSApp.presentationOptions = [.fullScreen, .hideDock, .hideMenuBar]
    }
    
    func windowWillExitFullScreen(_ notification: Notification) {
        guard let previousRect = previousRect else {
            return
        }
        
        let window = notification.object as? FSWindow
        window?.setFrame(previousRect, display: true)
    }
    
    func windowDidExitFullScreen(_ notification: Notification) {
        NSApp.presentationOptions = []
    }
    
    func windowWillStartLiveResize(_ notification: Notification) {
        
    }
    
    func windowDidEndLiveResize(_ notification: Notification) {
        
    }
    
    func windowWillResize(_ sender: NSWindow, to frameSize: NSSize) -> NSSize {
        return frameSize
    }
    
    func windowDidResize(_ notification: Notification) {
        
    }
    
    func windowWillClose(_ notification: Notification) {
        guard let window = notification.object as? FSWindow else {
            return
        }
        
        if window.isMainWindow {
            FSCocoaApp.shared.shutdown()
        }
    }
}


//# MARK: - Keycodes

/*
 *  Virtual keycodes
 *
 *  Discussion:
 *    These constants are the virtual keycodes defined originally in
 *    Inside Mac Volume V, pg. V-191. They identify physical keys on a
 *    keyboard. Those constants with "ANSI" in the name are labeled
 *    according to the key position on an ANSI-standard US keyboard.
 *    For example, vkA indicates the virtual keycode for the key
 *    with the letter 'A' in the US keyboard layout. Other keyboard
 *    layouts may have the 'A' key label on a different physical key;
 *    in this case, pressing 'A' will generate a different virtual
 *    keycode.
 */
enum ANSIKeycodes: UInt16 {
    case vkA = 0x00
    case vkS = 0x01
    case vkD = 0x02
    case vkF = 0x03
    case vkH = 0x04
    case vkG = 0x05
    case vkZ = 0x06
    case vkX = 0x07
    case vkC = 0x08
    case vkV = 0x09
    case vkB = 0x0B
    case vkQ = 0x0C
    case vkW = 0x0D
    case vkE = 0x0E
    case vkR = 0x0F
    case vkY = 0x10
    case vkT = 0x11
    case vk1 = 0x12
    case vk2 = 0x13
    case vk3 = 0x14
    case vk4 = 0x15
    case vk6 = 0x16
    case vk5 = 0x17
    case vkEqual = 0x18
    case vk9 = 0x19
    case vk7 = 0x1A
    case vkMinus = 0x1B
    case vk8 = 0x1C
    case vk0 = 0x1D
    case vkRightBracket = 0x1E
    case vkO = 0x1F
    case vkU = 0x20
    case vkLeftBracket = 0x21
    case vkI = 0x22
    case vkP = 0x23
    case vkL = 0x25
    case vkJ = 0x26
    case vkQuote = 0x27
    case vkK = 0x28
    case vkSemicolon = 0x29
    case vkBackslash = 0x2A
    case vkComma = 0x2B
    case vkSlash = 0x2C
    case vkN = 0x2D
    case vkM = 0x2E
    case vkPeriod = 0x2F
    case vkGrave = 0x32
    case vkKeypadDecimal = 0x41
    case vkKeypadMultiply = 0x43
    case vkKeypadPlus = 0x45
    case vkKeypadClear = 0x47
    case vkKeypadDivide = 0x4B
    case vkKeypadEnter = 0x4C
    case vkKeypadMinus = 0x4E
    case vkKeypadEquals = 0x51
    case vkKeypad0 = 0x52
    case vkKeypad1 = 0x53
    case vkKeypad2 = 0x54
    case vkKeypad3 = 0x55
    case vkKeypad4 = 0x56
    case vkKeypad5 = 0x57
    case vkKeypad6 = 0x58
    case vkKeypad7 = 0x59
    case vkKeypad8 = 0x5B
    case vkKeypad9 = 0x5C
}

/* keycodes for keys that are independent of keyboard layout*/
enum Keycodes: UInt16 {
    case vkReturn = 0x24
    case vkTab = 0x30
    case vkSpace = 0x31
    case vkDelete = 0x33
    case vkEscape = 0x35
    case vkRightCommand = 0x36
    case vkCommand = 0x37
    case vkShift = 0x38
    case vkCapsLock = 0x39
    case vkOption = 0x3A
    case vkControl = 0x3B
    case vkRightShift = 0x3C
    case vkRightOption = 0x3D
    case vkRightControl = 0x3E
    case vkFunction = 0x3F
    case vkF17 = 0x40
    case vkVolumeUp = 0x48
    case vkVolumeDown = 0x49
    case vkMute = 0x4A
    case vkF18 = 0x4F
    case vkF19 = 0x50
    case vkF20 = 0x5A
    case vkF5 = 0x60
    case vkF6 = 0x61
    case vkF7 = 0x62
    case vkF3 = 0x63
    case vkF8 = 0x64
    case vkF9 = 0x65
    case vkF11 = 0x67
    case vkF13 = 0x69
    case vkF16 = 0x6A
    case vkF14 = 0x6B
    case vkF10 = 0x6D
    case vkF12 = 0x6F
    case vkF15 = 0x71
    case vkHelp = 0x72
    case vkHome = 0x73
    case vkPageUp = 0x74
    case vkForwardDelete = 0x75
    case vkF4 = 0x76
    case vkEnd = 0x77
    case vkF2 = 0x78
    case vkPageDown = 0x79
    case vkF1 = 0x7A
    case vkLeftArrow = 0x7B
    case vkRightArrow = 0x7C
    case vkDownArrow = 0x7D
    case vkUpArrow = 0x7E
}

/* ISO keyboards only*/
enum ISOKeycodes: UInt16 {
    case vkSection = 0x0A
}

/* JIS keyboards only*/
enum JISKeycodes: UInt16 {
    case vkYen = 0x5D
    case vkUnderscore = 0x5E
    case vkKeypadComma = 0x5F
    case vkEisu = 0x66
    case vkKana = 0x68
}

/* Enum cases map to mouseButtons in fsRawInput struct. */
enum MouseButtons: UInt16 {
    case left = 0
    case right = 1
    case middle = 2
    case other = 3
}
