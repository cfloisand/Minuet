//
//  minuet_ray.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-18.
//  Adapted from https://github.com/TheCherno/RayTracing, by Cherno
//

#pragma once
#include "minuet_platform.h"

struct mnRay {
    fsv3f origin;
    fsv3f direction;
};
