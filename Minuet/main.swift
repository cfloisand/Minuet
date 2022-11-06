//
//  main.swift
//  Minuet
//
//  Created by Christian Floisand on 2022-10-16.
//

import Cocoa


autoreleasepool {
    
    let cocoaApp = FSCocoaApp.initApp()
    
    let window = MinuetWindow(title: "Minuet")
    
    window.startDisplayLink()
    window.makeKeyAndOrderFront(nil)

    var running = true
    while running {
        let input = cocoaApp.processEvents()
        if cocoaApp.isKeyPressed(keycode: Keycodes.vkEscape.rawValue) {
            running = false
        }
        
        window.update(input: input)
        cocoaApp.resetInput()
    }
    
    window.stopDisplayLink()

    cocoaApp.shutdown()
    
}
