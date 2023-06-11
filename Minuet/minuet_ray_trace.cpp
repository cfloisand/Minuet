//
//  minuet_ray_trace.cpp
//  Minuet
//
//  Created by Christian Floisand on 2022-10-18.
//

#include "minuet_ray_trace.h"
#include "minuet_camera.h"
#include "minuet_renderer.h"
#include "minuet_platform.h"


#pragma mark - mnPlatform

// Platform stubs
static void platform_hide_cursor_stub() {}
static void platform_show_cursor_stub() {}
static void platform_quit_stub() {}

static mnPlatform platform;

void
mn_platform_initialize() {
    platform.hide_cursor = platform_hide_cursor_stub;
    platform.show_cursor = platform_show_cursor_stub;
    platform.quit = platform_quit_stub;
}

void
mn_platform_set_hide_cursor_func(void(*func)(void)) {
    platform.hide_cursor = func;
}

void
mn_platform_set_show_cursor_func(void(*func)(void)) {
    platform.show_cursor = func;
}

void
mn_platform_set_quit_func(void(*func)(void)) {
    platform.quit = func;
}

mnPlatform*
mn_platform_get() {
    return &platform;
}

fsTimingToken*
mn_platform_timing_start() {
    return fs_timing_start();
}

fsr32
mn_platform_timing_stop(fsTimingToken *token) {
    return (fsr32)fs_timing_stop(token);
}


#pragma mark - mnImage

fsi16
mn_image_get_width(mnImage *image) {
    return image->width;
}

fsi16
mn_image_get_height(mnImage *image) {
    return image->height;
}

fsu32*
mn_image_get_data(mnImage *image) {
    return image->pixelData;
}


#pragma mark - mnScene
struct mnScene;
typedef struct mnScene mnScene;

mnScene*
mn_make_scene() {
    mnScene *scene = new mnScene;
    
    mnMaterial pinkSphere;
    pinkSphere.albedo = {1.f, 0.f, 1.f};
    pinkSphere.roughness = 0.f;
    
    mnMaterial blueSphere;
    blueSphere.albedo = {0.2f, 0.3f, 1.f};
    blueSphere.roughness = 0.1f;
    
    mnMaterial orangeSphere;
    orangeSphere.albedo = {0.8f, 0.5f, 0.2f};
    orangeSphere.roughness = 0.1f;
    orangeSphere.emissionColor = orangeSphere.albedo;
    orangeSphere.emissionPower = 2.f;
    
    scene->materials.push_back(pinkSphere);
    scene->materials.push_back(blueSphere);
    scene->materials.push_back(orangeSphere);
    
    mnSphere sphere0;
    sphere0.position = {0.f, 0.f, 0.f};
    sphere0.radius = 1.f;
    sphere0.materialIndex = 0;
    scene->spheres.push_back(sphere0);
    
    mnSphere sphere1;
    sphere1.position = {2.f, 0.f, 0.f};
    sphere1.radius = 1.f;
    sphere1.materialIndex = 2;
    scene->spheres.push_back(sphere1);
    
    mnSphere sphere2;
    sphere2.position = {1.f, -101.f, 0.f};
    sphere2.radius = 100.f;
    sphere2.materialIndex = 1;
    scene->spheres.push_back(sphere2);
    
    return scene;
}


#pragma mark - mnRenderer
mnRenderer*
mn_make_renderer() {
    mnRenderer *renderer = new mnRenderer;
    return renderer;
}

mnImage*
mn_renderer_render(mnRenderer *renderer, mnScene *scene, mnCamera *camera) {
    return renderer->render(*scene, *camera);
}

void
mn_renderer_resize(mnRenderer *renderer, fsi16 width, fsi16 height) {
    renderer->resize(width, height);
}


#pragma mark - mnCamera

mnCamera*
mn_make_camera(fsr32 verticalFOV, fsr32 nearClip, fsr32 farClip) {
    mnCamera *camera = new mnCamera(verticalFOV, nearClip, farClip);
    return camera;
}

void
mn_camera_resize(mnCamera *camera, fsi16 width, fsi16 height) {
    camera->resize(width, height);
}

void
mn_camera_update(mnCamera *camera, mnRenderer *renderer, fsRawInput *input, fsr32 dt) {
    if (camera->update(input, &platform, dt)) {
        renderer->resetFrameIndex();
    }
}
