//
//  minuet_ray_trace.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-18.
//

#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma mark - mnPlatform

struct mnPlatform;
typedef struct mnPlatform mnPlatform;
struct fsTimingToken;
typedef struct fsTimingToken fsTimingToken;

void mn_platform_initialize();

void mn_platform_set_hide_cursor_func(void(*func)(void));

void mn_platform_set_show_cursor_func(void(*func)(void));

void mn_platform_set_quit_func(void(*func)(void));

mnPlatform* mn_platform_get();

fsTimingToken* mn_platform_timing_start();

float mn_platform_timing_stop(fsTimingToken *token);

#pragma mark - mnImage

struct mnImage;
typedef struct mnImage mnImage;

int16_t mn_image_get_width(mnImage *image);

int16_t mn_image_get_height(mnImage *image);

uint32_t* mn_image_get_data(mnImage *image);

#pragma mark - mnScene
struct mnScene;
typedef struct mnScene mnScene;

mnScene* mn_make_scene();

#pragma mark - mnRenderer
struct mnRenderer;
typedef struct mnRenderer mnRenderer;
typedef struct mnCamera mnCamera;

mnRenderer* mn_make_renderer();

mnImage* mn_renderer_render(mnRenderer *renderer, mnScene *scene, mnCamera *camera);

void mn_renderer_resize(mnRenderer *renderer, int16_t width, int16_t height);

#pragma mark - mnCamera
struct mnCamera;
typedef struct mnCamera mnCamera;
struct fsRawInput;
typedef struct fsRawInput fsRawInput;

mnCamera* mn_make_camera(float verticalFOV, float nearClip, float farClip);

void mn_camera_resize(mnCamera *camera, int16_t width, int16_t height);

void mn_camera_update(mnCamera *camera, mnRenderer *renderer, fsRawInput *input, float dt);

#ifdef __cplusplus
}
#endif
