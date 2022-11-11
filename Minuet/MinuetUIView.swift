//
//  MinuetUIView.swift
//  Minuet
//
//  Created by Christian Floisand on 2022-10-28.
//

import Cocoa
import MNImGui


class MinuetUIView: NSView {
    private let metalLayer = CAMetalLayer()
    
    
    init(metalDevice: MTLDevice) {
        super.init(frame: .zero)
        metalLayer.framebufferOnly = true
        metalLayer.pixelFormat = .bgra8Unorm
        metalLayer.device = metalDevice
        layer = metalLayer
        wantsLayer = true
        
        mn_imgui_init(metalDevice, self)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override var frame: NSRect {
        get { return super.frame }
        set {
            super.frame = newValue
            metalLayer.drawableSize = newValue.size
        }
    }
    
    deinit {
        mn_imgui_shutdown()
    }
    
    func update(scene: OpaquePointer, renderer: OpaquePointer, platform: OpaquePointer) {
        guard let drawable = metalLayer.nextDrawable() else {
            return
        }
        
        mn_imgui_update(self, drawable, scene, renderer, platform)
    }
    
}
