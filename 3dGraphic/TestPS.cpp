#include "TestPS.h"

float4 TestPS::Execute(void* input)
{
    return main(*(Input*)input);
}

float4 TestPS::main(Input input)
{
    float4 sky = float4{ 117.0f / 255, 165.0f / 255, 231.0f / 255, 1 } + 0.7f;
    //float4 sunLight = float4{ 255 / 255.0f, 254 / 255.0f, 227 / 255.0f, 1 / 1.2f} *1.2f;
    float4 sunLight = float4{ 1, 1, 1, 1} * 1.5f;
    float3 sunDir = {-0.8f, 1, -0.7};
    if (input.normal.x == 1)
        int point = 0;

    float cosDecrease = dot(input.normal, normalize(sunDir));

    if (cosDecrease < 0)
        cosDecrease = 0;

    //float4 color = float4{ (input.color.x * cosDecrease) * sunLight.x, (input.color.y * cosDecrease) * sunLight.y, (input.color.z * cosDecrease) * sunLight.z, 1.0f } + sky * 0.1f;
    float4 color = float4{ (input.color.x * cosDecrease), (input.color.y * cosDecrease), (input.color.z * cosDecrease), 1.0f } + sky * 0.4f;


    return color;
}
