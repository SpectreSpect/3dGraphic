#pragma once

struct float4
{
    float x;
    float y;
    float z;
    float w;

    float4 operator+ (float num)
    {
        float4 v{ x, y, z, w };

        v.x += num;
        v.y += num;
        v.z += num;
        v.w += num;

        return v;
    }

    float4 operator- (float4 vec)
    {
        float4 v{ x, y, z, w };

        v.x -= vec.x;
        v.y -= vec.y;
        v.z -= vec.z;
        v.w -= vec.w;

        return v;
    }

    float4 operator* (float num)
    {
        float4 v{ x, y, z, w };

        v.x *= num;
        v.y *= num;
        v.z *= num;
        v.w *= num;

        return v;
    }

    float4 operator/ (float num)
    {
        float4 v{ x, y, z, w };

        v.x /= num;
        v.y /= num;
        v.z /= num;
        v.w /= num;

        return v;
    }

    float4 operator+ (float4 inputVec)
    {
        float4 v{ x, y, z, w };

        v.x += inputVec.x;
        v.y += inputVec.y;
        v.z += inputVec.z;
        v.w += inputVec.w;

        return v;
    }
};