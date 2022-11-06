//
//  minuet_camera.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-18.
//

#pragma once
#include "minuet_platform.h"
#include <vector>


struct fsRawInput;
struct mnPlatform;

struct mnCamera {
    mnCamera(fsr32 verticalFOV, fsr32 nearClip, fsr32 farClip);
    
    void update(fsRawInput *input, mnPlatform *platform, fsr32 dt);
    void resize(fsi16 width, fsi16 height);
    
    const fsmat4f& getProjection() const { return _projection; }
    const fsmat4f& getInverseProjection() const { return _inverseProjection; }
    const fsmat4f& getView() const { return _view; }
    const fsmat4f& getInverseView() const { return _inverseView; }
    
    const fsv3f& getPosition() const { return _position; }
    const fsv3f& getDirection() const { return _forwardDirection; }
    
    const std::vector<fsv3f>& getRayDirections() const { return _rayDirections; }
    
    fsr32 getRotationSpeed();
    
private:
    void recalculateProjection();
    void recalculateView();
    void recalculateRayDirections();
    
private:
    fsmat4f _projection;
    fsmat4f _view;
    fsmat4f _inverseProjection;
    fsmat4f _inverseView;
    
    fsr32 _verticalFOV = 45.f;
    fsr32 _nearClip = 0.1f;
    fsr32 _farClip = 100.f;
    
    fsv3f _position = {0.f, 0.f, 0.f};
    fsv3f _forwardDirection = {0.f, 0.f, 0.f};
    
    std::vector<fsv3f> _rayDirections;
    
    fsv2f _lastMousePosition = {0.f, 0.f};
    
    fsi16 _viewportWidth = 0;
    fsi16 _viewportHeight = 0;
};
