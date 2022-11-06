//
//  MinuetView.swift
//  Minuet
//
//  Created by Christian Floisand on 2022-10-16.
//

import Cocoa
import MNRayTrace


class MinuetView: NSView {
    private var bitmapContext: CGContext?
    private let renderer: OpaquePointer
    private let camera: OpaquePointer
    
    struct Viewport {
        let width: Int
        let height: Int
    }
    private var viewport: Viewport
    
    private var lock = pthread_mutex_t()
    
    
    override init(frame frameRect: NSRect) {
        renderer = mn_make_renderer()
        camera = mn_make_camera(45, 0.1, 100)
        viewport = Viewport(width: Int(frameRect.width), height: Int(frameRect.height))
        
        super.init(frame: frameRect)
        configureBitmapContext(viewport: viewport)
        
        pthread_mutex_init(&lock, nil)
    }
    
    convenience init() {
        self.init(frame: .zero)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    deinit {
        pthread_mutex_destroy(&lock)
    }
    
    override var acceptsFirstResponder: Bool { return true }
    
    override var isFlipped: Bool { return true }
    
    override var frame: NSRect {
        get { return super.frame }
        set {
            super.frame = newValue
            let width = Int(newValue.width)
            let height = Int(newValue.height)
            if width != viewport.width || height != viewport.height {
                pthread_mutex_lock(&lock)
                viewport = Viewport(width: width, height: height)
                pthread_mutex_unlock(&lock)
            }
        }
    }
    
    override func draw(_ dirtyRect: NSRect) {
        guard let context = NSGraphicsContext.current?.cgContext else {
            return
        }
        
        context.setFillColor(red: 0, green: 0, blue: 0, alpha: 1)
        context.fill(bounds)
        
        guard let bitmap = bitmapContext?.makeImage() else {
            return
        }
        
//        context.saveGState()
//        context.setBlendMode(.sourceAtop)
        context.draw(bitmap, in: bounds)
//        context.restoreGState()
    }
    
    func update(input: OpaquePointer, scene: OpaquePointer, dt: Float) {
        pthread_mutex_lock(&lock)
        let currentViewport = viewport
        pthread_mutex_unlock(&lock)
        
        if bitmapContext == nil || (currentViewport.width != bitmapContext!.width || currentViewport.height != bitmapContext!.height) {
            configureBitmapContext(viewport: currentViewport)
        }
        
        guard let context = bitmapContext, let contextData = context.data else {
            return
        }
        
        let viewportWidth = Int16(currentViewport.width)
        let viewportHeight = Int16(currentViewport.height)
        mn_camera_resize(camera, viewportWidth, viewportHeight)
        mn_camera_update(camera, input, dt)
        mn_renderer_resize(renderer, viewportWidth, viewportHeight)
        
        if let image = mn_renderer_render(renderer, scene, camera), let imageData = mn_image_get_data(image) {
            let imageWidth = mn_image_get_width(image)
            let imageHeight = mn_image_get_height(image)
            let pixelCount = Int(imageWidth) * Int(imageHeight)
            contextData.copyMemory(from: imageData, byteCount: pixelCount * context.bitsPerPixel/8)
        }
    }
    
    private func configureBitmapContext(viewport: Viewport) {
        let bitsPerComponent = 8
        let bytesPerPixel = 4
        let alphaInfo = CGImageAlphaInfo.noneSkipLast
        let bitmapInfo = CGBitmapInfo.byteOrder32Little
        guard let colorSpace = CGColorSpace(name: CGColorSpace.sRGB) else {
            fatalError("Could not initialize RGB color space.")
        }
        
        bitmapContext = CGContext(data: nil,
                                  width: viewport.width,
                                  height: viewport.height,
                                  bitsPerComponent: bitsPerComponent,
                                  bytesPerRow: viewport.width * bytesPerPixel,
                                  space: colorSpace,
                                  bitmapInfo: alphaInfo.rawValue | bitmapInfo.rawValue)
    }
}
