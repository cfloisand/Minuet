//
//  minuet_renderer.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-16.
//  Adapted from https://github.com/TheCherno/RayTracing, by Cherno
//

#pragma once
#include "minuet_platform.h"
#include "minuet_camera.h"
#include "minuet_ray.h"
#include "minuet_scene.h"


#pragma mark - mnImage

struct mnImage {
    mnImage(fsi16 width, fsi16 height);
    ~mnImage();
    
    void resize(fsi16 width, fsi16 height);
    
public:
    fsi16 width;
    fsi16 height;
    fsu32 *pixelData;
};


#pragma mark - mnRenderer

struct mnRenderer {
    
    struct Settings {
        bool accumulate = true;
    };
    
    mnRenderer() = default;
    
    mnImage* render(const mnScene& scene, const mnCamera& camera);
    void resize(fsi16 width, fsi16 height);
    
    void resetFrameIndex() { _frameIndex = 1; }
    
    Settings& getSettings() { return _settings; }
    
public:
    fsr64 lastRenderTime;
    
private:
    struct HitPayload {
        fsv3f worldPosition;
        fsv3f worldNormal;
        fsr32 hitDistance;
        fsi32 objectIndex;
    };
    
    fsv4f perPixel(fsu32 x, fsu32 y);
    HitPayload traceRay(const mnRay& ray);
    HitPayload closestHit(const mnRay& ray, fsr32 hitDistance, fsi32 objectIndex);
    HitPayload miss(const mnRay& ray);
    
private:
    Settings _settings;
    
    std::vector<fsu32> _imageHorizontalIter, _imageVerticalIter;
    
    mnImage * _image = nullptr;
    fsv4f * _accumulationData = nullptr;
    fsu32 _frameIndex = 1;
    
    const mnScene * _activeScene;
    const mnCamera * _activeCamera;
};
