#pragma once
struct float2
{
    float x;
    float y;

    float2 operator* (float num)
    {
        float2 v{ x, y };

        v.x *= num;
        v.y *= num;

        return v;
    }

    float2 operator+ (float2 vecArg)
    {
        float2 v{ x, y };

        v.x += vecArg.x;
        v.y += vecArg.y;

        return v;
    }
};