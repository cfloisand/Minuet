//
//  minuet_platform.h
//  Minuet
//
//  Created by Christian Floisand on 2022-10-26.
//

#pragma once
#include "fs_lib.h"
#include "fs_vector.h"
#include "fs_matrix.h"
#include "fs_quaternion.h"

typedef fsVec<fsr32, 2> fsv2f;
typedef fsVec<fsr32, 3> fsv3f;
typedef fsVec<fsr32, 4> fsv4f;

typedef fsMat<fsr32, 4, 4> fsmat4f;

typedef fsQuat<fsr32> fsquatf;

typedef void(*platform_call_f)(void);

struct mnPlatform {
    platform_call_f hide_cursor;
    platform_call_f show_cursor;
    platform_call_f quit;
};
