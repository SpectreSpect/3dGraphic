#include "Device.h"

Device::Device(size_t videoHashMemorySize)
{
    this->videoHashMemorySize = videoHashMemorySize;
    videoHashMemory = new char[this->videoHashMemorySize];
}

void Device::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
    if (this->vertexBuffer)
    {
        int dataSize = this->vertexBuffer->vertexSize * this->vertexBuffer->verticesCount;
        delete[dataSize](char*)changedVertexBuffer.buffer;
    }
    this->vertexBuffer = vertexBuffer;

    changedVertexBuffer = *vertexBuffer;
    void* verticesData = new char[vertexBuffer->vertexSize * vertexBuffer->verticesCount];
    changedVertexBuffer.buffer = verticesData;
}

void Device::SetVertexShader(VertexShader* vertexShader)
{
    vs = vertexShader;
}

void Device::SetViewPort(ViewPort viewPort)
{
	this->viewPort = viewPort;
}

float4 Device::DrawTriangle(float2 pixelPos, float3 A, float3 B, float3 C)
{
    float3 a = B - A;
    float3 b = C - A;
    float x = pixelPos.x;
    float y = pixelPos.y;

    float delta = -(a.x * b.y) - (a.z * b.x * y) - (a.y * b.z * x) + (x * b.y * a.z) + (b.z * y * a.x) + (a.y * b.x);

    if (delta == 0)
        return float4{ 0, 0, 0, 0 };

    float deltaU = (A.x * b.y) + (A.z * b.x * y) + (A.y * b.z * x) - (A.z * b.y * x) - (A.y * b.x) - (A.x * b.z * y);
    float deltaV = (a.x * A.y) + (A.x * y * a.z) + (a.y * A.z * x) - (a.z * A.y * x) - (a.y * A.x) - (A.z * y * a.x);
    float deltaZ = -(A.z * b.y * a.x) - (a.z * b.x * A.y) - (a.y * b.z * A.x) + (A.x * b.y * a.z) + (A.y * b.z * a.x) + (A.z * a.y * b.x);

    float u = deltaU / delta;
    float v = deltaV / delta;
    float z = deltaZ / delta;

    if ((u + v <= 1) && (u >= 0) && (u <= 1) && (v >= 0) && (v <= 1) && (1 - u - v >= 0) && (1 - u - v <= 1))
    {
        return float4{ u / z, v / z, (1.0f - u - v) / z, 1 };
    }
    return float4{ 0, 0, 0, 0 };
}

void Device::Draw(int verticesCount)
{
    if (vs)
        for (int i = 0; i < verticesCount; i++)
        {
            void* output = vs->Execute(vertexBuffer->buffer, i * vertexBuffer->vertexSize);
            int idInVertexBuffer = i * changedVertexBuffer.vertexSize;
            memcpy((char*)changedVertexBuffer.buffer + idInVertexBuffer, output, changedVertexBuffer.vertexSize);
        }
    else
        throw;

    for (int i = 0; i < verticesCount; i++)
    {
        float point1 = *(float*)((char*)changedVertexBuffer.buffer + (i * vertexBuffer->vertexSize));
        float point2 = *(float*)((char*)changedVertexBuffer.buffer + (i * vertexBuffer->vertexSize) + sizeof(float));
        float point3 = *(float*)((char*)changedVertexBuffer.buffer + (i * vertexBuffer->vertexSize) + 2 * sizeof(float));
        int k = 0;
    }

		for(float x = viewPort.left; x <= viewPort.right; x++)
			for (float y = viewPort.top; y <= viewPort.bottom; y++)
			{
                //DrawTriangle(float2{x, y}, );
			}
}
