//
//  minuet_scene.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-22.
//  Adapted from https://github.com/TheCherno/RayTracing, by Cherno
//

#pragma once
#include "minuet_platform.h"
#include <vector>


struct mnMaterial {
    fsv3f albedo = {1.f, 1.f, 1.f};
    fsr32 roughness = 1.f;
    fsr32 metallic = 0.f;
    fsv3f emissionColor = {};
    fsr32 emissionPower = 0.f;
    
    fsv3f getEmission() const { return emissionColor * emissionPower; }
};

struct mnSphere {
    fsv3f position = {};
    fsr32 radius = 0.5f;
    fsi32 materialIndex = 0;
};

struct mnScene {
    std::vector<mnSphere> spheres;
    std::vector<mnMaterial> materials;
};
