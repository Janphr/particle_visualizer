//
// Created by janphr on 08.09.20.
//
#pragma once

static float sqrt_fast(const float &n)
{
    static union{int i; float f;} u;
    u.i = 0x5F375A86 - (*(int*)&n >> 1);
    return (int(3) - n * u.f * u.f) * n * u.f * 0.5f;
}
