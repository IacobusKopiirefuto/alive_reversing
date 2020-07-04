#pragma once

#include "FunctionFwd.hpp"
#include "FixedPoint.hpp"

START_NS_AO

BYTE Math_NextRandom();

EXPORT short CC Math_RandomRange_450F20(signed short min, signed short max);

EXPORT FP CC Math_Sine_451110(BYTE v);

EXPORT FP CC Math_Cosine_4510A0(BYTE v);

ALIVE_ARY_EXTERN(unsigned char, 256, sRandomBytes_4BBE30);

END_NS_AO
