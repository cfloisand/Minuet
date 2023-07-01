//
//  minuet_renderer.cpp
//  Minuet
//
//  Created by Christian Floisand on 2022-10-16.
//  Adapted from https://github.com/TheCherno/RayTracing, by Cherno
//

#include "minuet_renderer.h"
#include <dispatch/dispatch.h>


#pragma mark - mnImage

mnImage::mnImage(fsi16 width, fsi16 height)
    : width(width)
    , height(height)
{
    pixelData = new fsu32[width * height];
}

mnImage::~mnImage() {
    delete [] pixelData;
}

void
mnImage::resize(fsi16 w, fsi16 h) {
    if (width != w || height != h) {
        delete [] pixelData;
        width = w;
        height = h;
        pixelData = new fsu32[width * height];
    }
}


#pragma mark - mnRenderer

static fsu32
convertToRGBA(const fsv4f& color) {
    fsu8 r = (fsu8)(color.r * 255.f);
    fsu8 g = (fsu8)(color.g * 255.f);
    fsu8 b = (fsu8)(color.b * 255.f);
    fsu8 a = (fsu8)(color.a * 255.f);
    
    fsu32 result = (r << 24) | (g << 16) | (b << 8) | a;
    return result;
}

static fsv3f
randomInUnitSphere(fsu32& seed) {
    return fs_vnormalize(fsv3f_random(seed));
}

mnImage*
mnRenderer::render(const mnScene& scene, const mnCamera& camera) {
    fsTimingToken *start = fs_timing_start();
    if (_image) {
        _activeScene = &scene;
        _activeCamera = &camera;
        
        if (_frameIndex == 1) {
            fs_memclear(_accumulationData, _image->width * _image->height * sizeof(fsv4f));
        }
        
        // NOTE(christian): TheCherno uses a parallel for_each loop here, which requires C++17. I am on an older iMac, however, which does not allow me to update to the newest Xcode, and my current version
        // of Xcode (12) does not have support for C++17. As a replacement, I'm using Apple's GCD library to multithread the following code with very good, comparable results to what TheCherno achieved
        // with parallel for_each.
        dispatch_queue_t queue = dispatch_get_global_queue(QOS_CLASS_USER_INITIATED, 0);
        dispatch_group_t group = dispatch_group_create();
        
        for (fsu32 y = 0; y < _image->height; ++y) {
            dispatch_group_async(group, queue, ^{
                for (fsu32 x = 0; x < _image->width; ++x) {
                    fsv4f color = perPixel(x, y);
                    _accumulationData[x + (y * _image->width)] += color;

                    fsv4f accumulatedColor = _accumulationData[x + (y * _image->width)];
                    accumulatedColor /= (fsr32)_frameIndex;

                    accumulatedColor = fs_vclamp01(accumulatedColor);
                    _image->pixelData[x + (y * _image->width)] = convertToRGBA(accumulatedColor);
                }
            });
        }
        
        dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
    }
    
    if (_settings.accumulate) {
        _frameIndex++;
    } else {
        _frameIndex = 1;
    }
    
    lastRenderTime = fs_timing_stop(start);
    
    return _image;
}

void
mnRenderer::resize(fsi16 width, fsi16 height) {
    if (_image) {
        if (_image->width == width && _image->height == height) {
            return;
        }
        
        _image->resize(width, height);
    } else {
        _image = new mnImage(width, height);
    }
    
    delete [] _accumulationData;
    _accumulationData = new fsv4f[width * height];
    
    _imageHorizontalIter.resize(width);
    _imageVerticalIter.resize(height);
    for (fsu32 i = 0; i < width; ++i) {
        _imageHorizontalIter[i] = i;
    }
    for (fsu32 i = 0; i < height; ++i) {
        _imageVerticalIter[i] = i;
    }
}

fsv4f
mnRenderer::perPixel(fsu32 x, fsu32 y) {
    mnRay ray;
    ray.origin = _activeCamera->getPosition();
    ray.direction = _activeCamera->getRayDirections()[x + y * _image->width];
    
    fsv3f light = {};
    fsv3f contribution = {1.f, 1.f, 1.f};
    
    fsu32 seed = x + y * _image->width;
    seed *= _frameIndex;
    
    int bounces = 5;
    for (int i = 0; i < bounces; ++i) {
        seed += i;
        mnRenderer::HitPayload payload = traceRay(ray);
        if (payload.hitDistance < 0.f) {
            fsv3f skyColor = {0.6f, 0.7f, 0.9f};
//            light += fs_vhadamard(skyColor, contribution);
            break;
        }
        
        const mnSphere& sphere = _activeScene->spheres[payload.objectIndex];
        const mnMaterial& material = _activeScene->materials[sphere.materialIndex];
        
        contribution = fs_vhadamard(contribution, material.albedo);
        light += material.getEmission();
        
        ray.origin = payload.worldPosition + payload.worldNormal * 0.0001f;
        ray.direction = fs_vnormalize(payload.worldNormal + randomInUnitSphere(seed));
    }
    
    return {light.r, light.g, light.b, 1.f};
}

mnRenderer::HitPayload
mnRenderer::traceRay(const mnRay& ray) {
    int closestSphere = -1;
    fsr32 hitDistance = FLT_MAX;
    for (size_t i = 0; i < _activeScene->spheres.size(); ++i) {
        const mnSphere& sphere = _activeScene->spheres[i];
        fsv3f origin = ray.origin - sphere.position;
        
        fsr32 a = fs_vdot(ray.direction, ray.direction);
        fsr32 b = 2.f * fs_vdot(origin, ray.direction);
        fsr32 c = fs_vdot(origin, origin) - (sphere.radius * sphere.radius);
        
        fsr32 discriminant = (b * b) - (4.f * a * c);
        if (discriminant < 0.f) {
            continue;
        }
        
        fsr32 closestT = (-b - sqrtf(discriminant)) / (2.f * a);
        if (closestT > 0.f && closestT < hitDistance) {
            hitDistance = closestT;
            closestSphere = (int)i;
        }
    }
    
    if (closestSphere < 0) {
        return miss(ray);
    }
    
    return closestHit(ray, hitDistance, closestSphere);
}

mnRenderer::HitPayload
mnRenderer::closestHit(const mnRay& ray, fsr32 hitDistance, fsi32 objectIndex) {
    mnRenderer::HitPayload payload;
    payload.hitDistance = hitDistance;
    payload.objectIndex = objectIndex;
    
    const mnSphere& closestSphere = _activeScene->spheres[objectIndex];
    fsv3f origin = ray.origin - closestSphere.position;
    payload.worldPosition = origin + ray.direction * hitDistance;
    payload.worldNormal = fs_vnormalize(payload.worldPosition);
    payload.worldPosition += closestSphere.position;
    
    return payload;
}

mnRenderer::HitPayload
mnRenderer::miss(const mnRay& ray) {
    mnRenderer::HitPayload payload;
    payload.hitDistance = -1.f;
    return payload;
}
