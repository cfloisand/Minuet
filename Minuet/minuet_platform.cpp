//
//  minuet_platform.cpp
//  Minuet
//
//  Created by Christian Floisand on 2022-10-26.
//

#include "minuet_platform.h"

fsv3f
fsv3f_random(fsr32 min, fsr32 max) {
    fsv3f vec = { fs_random01() * (max - min) + min, fs_random01() * (max - min) + min, fs_random01() * (max - min) + min };
    return vec;
}
