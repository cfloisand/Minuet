//
//  minuet_platform.cpp
//  Minuet
//
//  Created by Christian Floisand on 2022-10-26.
//

#include "minuet_platform.h"

fsu32
pcgHash(fsu32 input) {
    fsu32 state = input * 747796405u + 2891336453u;
    fsu32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

fsr32
randomFloat(fsu32& seed) {
    seed = pcgHash(seed);
    return (fsr32)seed / (fsr32)std::numeric_limits<fsu32>::max();
}

fsv3f
fsv3f_random(fsr32 min, fsr32 max) {
    fsv3f vec = { fs_random01() * (max - min) + min, fs_random01() * (max - min) + min, fs_random01() * (max - min) + min };
    return vec;
}

fsv3f
fsv3f_random(fsu32& seed) {
    fsv3f vec = { randomFloat(seed) * 2.f - 1.f, randomFloat(seed) * 2.f - 1.f, randomFloat(seed) * 2.f - 1.f };
    return vec;
}
