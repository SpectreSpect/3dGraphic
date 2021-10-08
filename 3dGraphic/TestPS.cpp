#include "TestPS.h"

float4 TestPS::Execute(void* input)
{
    return main(*(Input*)input);
}

float4 TestPS::main(Input input)
{
    //float4 color = float4{ input.pos.x, input.pos.y, input.pos.z, 1.0f };
    float4 color = float4{ input.color.x / input.pos.z, input.color.y / input.pos.z, input.color.z / input.pos.z, 1.0f };
    //float4 color = float4{ 0, 1, 1, 1.0f };
    return color;
}
