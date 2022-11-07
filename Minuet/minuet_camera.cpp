//
//  minuet_camera.cpp
//  Minuet
//
//  Created by Christian Floisand on 2022-10-18.
//  Adapted from https://github.com/TheCherno/RayTracing, by Cherno
//

#include "minuet_camera.h"
#include "fs_input.h"


mnCamera::mnCamera(fsr32 verticalFOV, fsr32 nearClip, fsr32 farClip)
    : _verticalFOV(verticalFOV)
    , _nearClip(nearClip)
    , _farClip(farClip)
{
    _forwardDirection = {0.f, 0.f, -1.f};
    _position = {0.f, 0.f, 20.f};
    _projection = fs_matrix_identity<fsr32, 4>();
    _view = fs_matrix_identity<fsr32, 4>();
    _inverseProjection = fs_matrix_identity<fsr32, 4>();
    _inverseView = fs_matrix_identity<fsr32, 4>();
}

void
mnCamera::update(fsRawInput *input, mnPlatform *platform, fsr32 dt) {
    fsv2f mousePos = {input->mouseX, input->mouseY};
    fsv2f delta = (mousePos - _lastMousePosition) * 0.002f;
    _lastMousePosition = mousePos;
    
    if (!input->mouseRight.isDown) {
        platform->show_cursor();
        return;
    }
    
    platform->hide_cursor();
    
    bool moved = false;
    
    fsv3f upDirection = {0.f, 1.f, 0.f};
    fsv3f rightDirection = fs_vcross(_forwardDirection, upDirection);
    
    fsr32 speed = 5.f;
    
    if (input->W.isDown) {
        _position += _forwardDirection * speed * dt;
        moved = true;
    } else if (input->S.isDown) {
        _position -= _forwardDirection * speed * dt;
        moved = true;
    }
    
    if (input->A.isDown) {
        _position -= rightDirection * speed * dt;
        moved = true;
    } else if (input->D.isDown) {
        _position += rightDirection * speed * dt;
        moved = true;
    }
    
    if (input->Q.isDown) {
        _position -= upDirection * speed * dt;
        moved = true;
    } else if (input->E.isDown) {
        _position += upDirection * speed * dt;
        moved = true;
    }
    
    if (delta.x != 0.f || delta.y != 0.f) {
        fsr32 pitchDelta = delta.y * getRotationSpeed();
        fsr32 yawDelta = delta.x * getRotationSpeed();

        fsquatf q = fs_quat_unit_normal(-pitchDelta, rightDirection) * fs_quat_unit_normal(-yawDelta, upDirection);
        _forwardDirection = fs_quat_rotate(_forwardDirection, fs_quat_normalize(q));

        moved = true;
    }
    
    if (moved) {
        recalculateView();
        recalculateRayDirections();
    }
}

void
mnCamera::resize(fsi16 width, fsi16 height) {
    if (width == _viewportWidth && height == _viewportHeight) {
        return;
    }
    
    _viewportWidth = width;
    _viewportHeight = height;
    
    recalculateProjection();
    recalculateRayDirections();
}

fsr32
mnCamera::getRotationSpeed() {
    return 0.3f;
}

void
mnCamera::recalculateProjection() {
    fsr32 fovRadian = _verticalFOV * fsDeg2Rad;
    _projection = fs_matrix_perspective_fov<fsr32>(fovRadian, _viewportWidth, _viewportHeight, _nearClip, _farClip);
    _inverseProjection = fs_matrix_inverse(_projection);
}

void
mnCamera::recalculateView() {
    _view = fs_matrix_look_at(_position, _position + _forwardDirection, (fsv3f){0.f, 1.f, 0.f});
    _inverseView = fs_matrix_inverse(_view);
}

void
mnCamera::recalculateRayDirections() {
    _rayDirections.resize(_viewportWidth * _viewportHeight);
    
    for (fsu32 y = 0; y < _viewportHeight; ++y) {
        for (fsu32 x = 0; x < _viewportWidth; ++x) {
            fsv2f coord = { (fsr32)x / _viewportWidth, (fsr32)y / _viewportHeight };
            coord = coord * 2.f - (fsv2f){1.f, 1.f}; // -1 -> 1
            
            fsv4f target = _inverseProjection * (fsv4f){coord.x, coord.y, 1.f, 1.f};
            fsv3f tn = fs_vnormalize(target.xyz / target.w);
            fsv4f rayDirection = _inverseView * (fsv4f){tn.x, tn.y, tn.z, 0.f};
            _rayDirections[x + y * _viewportWidth] = rayDirection.xyz;
        }
    }
}
