//
//  minuet_imgui.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-29.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

@protocol MTLDevice;
@protocol CAMetalDrawable;
@class NSView;

struct mnScene;
typedef struct mnScene mnScene;
struct mnPlatform;
typedef struct mnPlatform mnPlatform;

void mn_imgui_init(id<MTLDevice> device, NSView *view);
void mn_imgui_shutdown();
void mn_imgui_update(NSView *view, id<CAMetalDrawable> drawable, mnScene *scene, mnPlatform *platform);

#ifdef __cplusplus
}
#endif
