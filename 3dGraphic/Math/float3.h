#pragma once
struct float3
{
    float x;
    float y;
    float z;

    float3 operator+ (float num)
    {
        float3 v{ x, y, z };

        v.x += num;
        v.y += num;
        v.z += num;

        return v;
    }

    float3 operator- (float3 vec)
    {
        float3 v{ x, y, z };

        v.x -= vec.x;
        v.y -= vec.y;
        v.z -= vec.z;

        return v;
    }

    float3 operator* (float num)
    {
        float3 v{ x, y, z };

        v.x *= num;
        v.y *= num;
        v.z *= num;

        return v;
    }

    float3 operator/ (float num)
    {
        float3 v{ x, y, z };

        v.x /= num;
        v.y /= num;
        v.z /= num;

        return v;
    }

    float3 operator+ (float3 inputVec)
    {
        float3 v{ x, y, z };

        v.x += inputVec.x;
        v.y += inputVec.y;
        v.z += inputVec.z;
        return v;
    }
};