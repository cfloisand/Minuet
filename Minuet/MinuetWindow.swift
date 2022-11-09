//
//  MinuetWindow.swift
//  Minuet
//
//  Created by Christian Floisand on 2022-10-22.
//

import Cocoa
import MNRayTrace


class MinuetWindow: FSWindow {
    private var displayLink: CVDisplayLink!
    private var frameLock = pthread_mutex_t()
    private var frameEnd = pthread_cond_t()
    private var timingToken: OpaquePointer?
    
    private let metalDevice: MTLDevice
    
    private weak var minuetView: MinuetView?
    private weak var uiView: MinuetUIView?
    
    private let scene: OpaquePointer
    
    
    convenience init(title: String?) {
        let windowRect = NSMakeRect(200, 600, 1260, 780);
        let windowStyle: NSWindow.StyleMask = [.titled, .closable, .miniaturizable, .resizable]
        self.init(title: title, contentRect: windowRect, styleMask: windowStyle, delegate: MinuetWindowDelegate())
    }
    
    override init(title: String?, contentRect: NSRect, styleMask: NSWindow.StyleMask, delegate: FSWindowDelegate?) {
        guard let device = MTLCreateSystemDefaultDevice() else {
            fatalError("Failed to create default Metal device!")
        }
        metalDevice = device
        scene = mn_make_scene()
        
        super.init(title: title, contentRect: contentRect, styleMask: styleMask, delegate: MinuetWindowDelegate())
        
        var contentRect = self.contentRect(forFrameRect: frame)
        contentRect.origin = .zero
        
        let contentView = NSView(frame: contentRect)
        self.contentView = contentView
        self.makeFirstResponder(contentView)
        
        let minuetView = MinuetView()
        let uiView = MinuetUIView(metalDevice: metalDevice)
        contentView.addSubview(minuetView)
        contentView.addSubview(uiView)
        
        self.minuetView = minuetView
        self.uiView = uiView
        layoutSubviews(withContentSize: contentRect.size)
        
        let mainDisplay = CGMainDisplayID()
        let result = CVDisplayLinkCreateWithCGDisplay(mainDisplay, &displayLink)
        guard result == kCVReturnSuccess else {
            fatalError("Failed to create display link for main display!")
        }
        
        mn_platform_initialize()
        mn_platform_set_hide_cursor_func(platformHideCursor)
        mn_platform_set_show_cursor_func(platformShowCursor)
        mn_platform_set_quit_func(platformQuit)
    }
    
    func startDisplayLink() {
        pthread_mutex_init(&frameLock, nil)
        pthread_cond_init(&frameEnd, nil)
        
        CVDisplayLinkSetOutputHandler(displayLink) { [weak self] (_, _, _, _, _) -> CVReturn in
            return self?.vsyncSignal() ?? kCVReturnError
        }
        
        let result = CVDisplayLinkStart(displayLink)
        if result != kCVReturnSuccess {
            fatalError("Failed to start display link!")
        }
    }
    
    func stopDisplayLink() {
        let result = CVDisplayLinkStop(displayLink)
        if result != kCVReturnSuccess {
            debugPrint("Failed to stop display link!")
        }
        
        pthread_mutex_destroy(&frameLock)
        pthread_cond_destroy(&frameEnd)
    }
    
    private func vsyncSignal() -> CVReturn {
        // NOTE(christian): Synchronize with thread waiting on condition variable.
        pthread_mutex_lock(&frameLock)
        pthread_cond_signal(&frameEnd)
        pthread_mutex_unlock(&frameLock)
        
        return kCVReturnSuccess
    }
    
    func update(input: OpaquePointer) {
        assert(Thread.isMainThread, "update must be called from the main thread.")
        
        let dt: Float
        if let token = timingToken {
            dt = mn_platform_timing_stop(token)
        } else {
            dt = 0
        }
        timingToken = mn_platform_timing_start()
        
        minuetView?.update(input: input, scene: scene, dt: Float(dt/1000))
        uiView?.update(scene: scene, platform: mn_platform_get())
        
        // NOTE(christian): Synchronize with display link thread to keep this thread running at the monitor's refresh rate.
        pthread_mutex_lock(&frameLock)
        pthread_cond_wait(&frameEnd, &frameLock)
        pthread_mutex_unlock(&frameLock)
        
        minuetView?.setNeedsDisplay(minuetView?.bounds ?? .zero)
    }
    
    func layoutSubviews(withContentSize contentSize: NSSize) {
        guard let minuetView = minuetView, let uiView = uiView else {
            fatalError("Missing subviews for layout.")
        }
        
        let uiViewWidth: CGFloat = 300
        uiView.frame = NSRect(x: contentSize.width - uiViewWidth, y: 0, width: uiViewWidth, height: contentSize.height)
        minuetView.frame = NSRect(x: 0, y: 0, width: contentSize.width - uiViewWidth, height: contentSize.height)
    }
    
}


//# MARK: - MinuetWindowDelegate

private class MinuetWindowDelegate: FSWindowDelegate {
    
    override func windowDidResize(_ notification: Notification) {
        if let window = notification.object as? MinuetWindow, let contentView = window.contentView {
            window.layoutSubviews(withContentSize: contentView.bounds.size)
            window.update(input: FSCocoaApp.shared.getInput())
        }
    }
    
}


//# MARK: - Platform functions

fileprivate var cursorShowing = true
fileprivate func platformHideCursor() {
    if cursorShowing {
        FSCocoaApp.hideCursor()
        cursorShowing = false
    }
}

fileprivate func platformShowCursor() {
    if !cursorShowing {
        FSCocoaApp.showCursor()
        cursorShowing = true
    }
}

fileprivate func platformQuit() {
    FSCocoaApp.shared.shutdown()
}
