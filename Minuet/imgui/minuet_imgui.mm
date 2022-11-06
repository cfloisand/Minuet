//
//  minuet_imgui.cpp
//  Minuet
//
//  Created by Christian Floisand on 2022-10-29.
//

#include "minuet_imgui.h"
#include "imgui.h"
#include "imgui_impl_metal.h"
#include "imgui_impl_osx.h"

#include "minuet_scene.h"
#include "minuet_renderer.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>


static id<MTLCommandQueue> commandQueue;
static MTLRenderPassDescriptor *renderPassDescriptor;
static ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void
mn_imgui_init(id<MTLDevice> device, NSView *view) {
    commandQueue = device.newCommandQueue;
    renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    
    // Setup Dear ImGui context
    // FIXME: This example doesn't have proper cleanup...
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Renderer backend
    ImGui_ImplMetal_Init(device);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    ImGui_ImplOSX_Init(view);
}

void
mn_imgui_shutdown() {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImGui::DestroyContext();
}

void
mn_imgui_update(NSView *view, id<CAMetalDrawable> drawable, mnScene *scene, mnPlatform *platform) {
    id<MTLCommandBuffer> commandBuffer = commandQueue.commandBuffer;
    
    renderPassDescriptor.colorAttachments[0].texture = drawable.texture;

    // Start the Dear ImGui frame
    ImGui_ImplMetal_NewFrame(renderPassDescriptor);
    ImGui_ImplOSX_NewFrame(view);
    ImGui::NewFrame();

    {
        ImGui::Begin("Settings");
        ImGui::Text("Last render: %.3fms", mnRenderer::lastRenderTime);
        ImGui::Spacing();
        ImGui::Spacing();
        if (ImGui::Button("Quit")) {
            platform->quit();
        }
        ImGui::End();
        
        ImGui::Begin("Scene");
        for (int i = 0; i < scene->spheres.size(); ++i) {
            mnSphere& sphere = scene->spheres[i];
            
            ImGui::PushID(i);
            ImGui::DragFloat3("Position", sphere.position.e, 0.1f);
            ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
            ImGui::DragInt("Material", &sphere.materialIndex, 1.f, 0, (int)scene->materials.size() - 1);
            ImGui::Separator();
            ImGui::PopID();
        }
        
        for (int i = 0; i < scene->materials.size(); ++i) {
            mnMaterial& material = scene->materials[i];
            
            ImGui::PushID(i);
            ImGui::ColorEdit3("Albedo", material.albedo.e);
            ImGui::DragFloat("Roughness", &material.roughness, 0.05f, 0.f, 1.f);
            ImGui::DragFloat("Metallic", &material.metallic, 0.05f, 0.f, 1.f);
            ImGui::Separator();
            ImGui::PopID();
        }
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    [renderEncoder pushDebugGroup:@"Dear ImGui rendering"];
    ImGui_ImplMetal_RenderDrawData(draw_data, commandBuffer, renderEncoder);
    [renderEncoder popDebugGroup];
    [renderEncoder endEncoding];

    // Present
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}
