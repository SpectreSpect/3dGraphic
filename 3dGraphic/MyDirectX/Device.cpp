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

void Device::SetPixelShader(PixelShader* pixelShader)
{
    ps = pixelShader;
}

void Device::SetViewPort(ViewPort viewPort)
{
	this->viewPort = viewPort;
}

void Device::SetInputLayout(Input_Layout* inputLayout)
{
    this->inputLayout = inputLayout;

    Attribute test = inputLayout->attributeBuffer[2];
    int sdfsdf = 0;
    //if (this->inputLayout->attributeBuffer[0].semantic == "POSITION")
    //    throw;

    int point = 0;
}

void Device::CreateSwapChain(SwapChain** swapChain, int backBuffersCount, float2 backBuffersSize, DXGI_FORMAT format)
{
    *swapChain = new SwapChain();

    (*swapChain)->CreateBackBuffers(backBuffersSize, format, backBuffersCount);

    this->swapChain = swapChain;
}

void Device::SetDepthStencil(Texture2D* depthStencil)
{
    this->depthStencil = depthStencil;
}

float3 Device::GetUVZ(float2 pixelPos, int firstVertexID)
{

    int posID = NONE;
    for (int i = 0; i < inputLayout->elementsCount; i++)
        if (inputLayout->attributeBuffer[i].semantic == "POSITION")
        {
            posID = i;
            int kek = 0;
        }


    if (posID == NONE)
        throw;

    float3 A = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + firstVertexID * changedVertexBuffer.vertexSize);
    float3 B = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
    float3 C = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + (firstVertexID + 2) * changedVertexBuffer.vertexSize);

    float3 a = B - A;
    float3 b = C - A;
    float x = pixelPos.x;
    float y = pixelPos.y;

    float delta = -(a.x * b.y) - (a.z * b.x * y) - (a.y * b.z * x) + (x * b.y * a.z) + (b.z * y * a.x) + (a.y * b.x);

    if (delta == 0)
        return float3{ -1, -1, -1};

    float deltaU = (A.x * b.y) + (A.z * b.x * y) + (A.y * b.z * x) - (A.z * b.y * x) - (A.y * b.x) - (A.x * b.z * y);
    float deltaV = (a.x * A.y) + (A.x * y * a.z) + (a.y * A.z * x) - (a.z * A.y * x) - (a.y * A.x) - (A.z * y * a.x);
    float deltaZ = -(A.z * b.y * a.x) - (a.z * b.x * A.y) - (a.y * b.z * A.x) + (A.x * b.y * a.z) + (A.y * b.z * a.x) + (A.z * a.y * b.x);

    float u = deltaU / delta;
    float v = deltaV / delta;
    float z = deltaZ / delta;

    return float3{ u, v, z };
}


float4 Device::DrawTriangle(float2 pixelPos, int firstVertexID)
{
    if (inputLayout)
    {
        float3 uvz = GetUVZ(pixelPos, firstVertexID);
        float u = uvz.x;
        float v = uvz.y;
        float z = uvz.z;

        if ((u + v <= 1) && (u >= 0) && (u <= 1) && (v >= 0) && (v <= 1) && (1 - u - v >= 0) && (1 - u - v <= 1))
        {
            int byteCountInVertex = inputLayout->attributeBuffer[inputLayout->elementsCount - 1].offsetInVertex + inputLayout->attributeBuffer[inputLayout->elementsCount - 1].format;
            void* vertex = new char[byteCountInVertex];

            for (int i = 0; i < inputLayout->elementsCount; i++)
            {
                int offset = inputLayout->attributeBuffer[i].offsetInVertex;
                if (inputLayout->attributeBuffer[i].format == DXGI_FORMAT_R32G32B32_FLOAT)
                {
                    float3 elementVertex1 = *(float3*)((char*)changedVertexBuffer.buffer + offset + firstVertexID * changedVertexBuffer.vertexSize);
                    float3 elementVertex2 = *(float3*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
                    float3 elementVertex3 = *(float3*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 2) * changedVertexBuffer.vertexSize);
                    float3 element = elementVertex1 * (1 - u - v) + elementVertex2 * u + elementVertex3 * v;
                    memcpy((char*)vertex + offset, &element, sizeof(float3));
                }
                else if (inputLayout->attributeBuffer[i].format == DXGI_FORMAT_R32G32B32A32_FLOAT)
                {
                    float4 elementVertex1 = *(float4*)((char*)changedVertexBuffer.buffer + offset + firstVertexID * changedVertexBuffer.vertexSize);
                    float4 elementVertex2 = *(float4*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
                    float4 elementVertex3 = *(float4*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 2) * changedVertexBuffer.vertexSize);
                    float4 element = elementVertex1 * (1 - u - v) + elementVertex2 * u + elementVertex3 * v;
                    memcpy((char*)vertex + offset, &element, sizeof(float4));
                }
            }
            //float4 color = float4{ u / z, v / z, (1.0f - u - v) / z, 1 };
            float4 color = ps->Execute(vertex);
            return color;
        }
        return float4{ 0, 0, 0, 0 };
    }
    else throw; 
}

float4 Device::DrawTriangle(float2 pixelPos, int firstVertexID, float3 uvz)
{
    if (inputLayout)
    {
        float u = uvz.x;
        float v = uvz.y;
        float z = uvz.z;

        if ((u + v <= 1) && (u >= 0) && (u <= 1) && (v >= 0) && (v <= 1) && (1 - u - v >= 0) && (1 - u - v <= 1))
        {
            int byteCountInVertex = inputLayout->attributeBuffer[inputLayout->elementsCount - 1].offsetInVertex + inputLayout->attributeBuffer[inputLayout->elementsCount - 1].format;

            if (firstVertexID == 3)
                int point = 1;

            void* vertex = new char[byteCountInVertex];


            //float3 pos = { 0, 0, 4 };
            //VERTEX verticesPos[] =
            //{
            //    //bottom
            //    -1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,
            //    -1 + pos.x, -1 + pos.y, 1 + pos.z,  1, 1, 1, 1,
            //    1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,
            // 
            //    1 + pos.x, -1 + pos.y, -1 + pos.z,  1, 1, 1, 1,
            //    1 + pos.x, -1 + pos.y, 1 + pos.z, 1, 1, 1, 1,
            //   -1 + pos.x, -1 + pos.y, 1 + pos.z, 1, 1, 1, 1,
            //}
            for (int i = 0; i < inputLayout->elementsCount; i++)
            {
                int offset = inputLayout->attributeBuffer[i].offsetInVertex;
                if (inputLayout->attributeBuffer[i].format == DXGI_FORMAT_R32G32B32_FLOAT)
                {
                    float3 elementVertex1 = *(float3*)((char*)changedVertexBuffer.buffer + offset + firstVertexID * changedVertexBuffer.vertexSize);
                    float3 elementVertex2 = *(float3*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
                    float3 elementVertex3 = *(float3*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 2) * changedVertexBuffer.vertexSize);
                    float3 element = elementVertex1 * (1 - u - v) + elementVertex2 * u + elementVertex3 * v;
                    memcpy((char*)vertex + offset, &element, sizeof(float3));
                }
                else if (inputLayout->attributeBuffer[i].format == DXGI_FORMAT_R32G32B32A32_FLOAT)
                {
                    float4 elementVertex1 = *(float4*)((char*)changedVertexBuffer.buffer + offset + firstVertexID * changedVertexBuffer.vertexSize);
                    float4 elementVertex2 = *(float4*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
                    float4 elementVertex3 = *(float4*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 2) * changedVertexBuffer.vertexSize);
                    float4 element = elementVertex1 * (1 - u - v) + elementVertex2 * u + elementVertex3 * v;
                    memcpy((char*)vertex + offset, &element, sizeof(float4));
                }
            }

            //VERTEX vertex = *(VERTEX*)vertex;
            //float4 color = float4{ u / z, v / z, (1.0f - u - v) / z, 1 };
            float4 color = ps->Execute(vertex);
            return color;
        }
        return float4{ 0, 0, 0, 0 };
    }
    else throw;
}

void Device::FillDepthStencil()
{

}

void Device::ClearBuffer(float4 color, Texture2D* texture)
{
    for (int x = 0; x < texture->size.x; x++)
        for (int y = 0; y < texture->size.y; y++)
            memcpy((char*)texture->texture2D[x] + y * texture->format, &color, sizeof(float4));

    float4 point = *(float4*)((char*)texture->texture2D[0] + (int)texture->size.y * texture->format);
}


void Device::Draw(int verticesCount)
{
    if (swapChain)
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



        float width = viewPort.right - viewPort.left;
        float height = viewPort.bottom - viewPort.top;

        if (depthStencil)
        {

            for (int x = viewPort.left; x < viewPort.right; x++)
                for (int y = viewPort.top; y < viewPort.bottom; y++)
                {
                    float kek = height / width;
                    float2 pixelPos = float2{ ((x / width) * 2 - 1) / kek , ((height - y) / height) * 2 - 1 };
                    float3 uvzWIthMinZ = { BIG_NUM, BIG_NUM, BIG_NUM };
                    float minDepth = 0;
                    float triangleWithMinDepthID = 0;
                    float depth = BIG_NUM;
                    for (int i = 0; i < verticesCount; i += 3)
                    {
                        float3 uvz = GetUVZ(pixelPos, i);
                        //depth = (*(float2*)((char*)depthStencil->texture2D[x] + y * depthStencil->format)).x;
                        if ((uvz.x + uvz.y <= 1) && (uvz.x >= 0) && (uvz.x <= 1) && (uvz.y >= 0) && (uvz.y <= 1) && (1 - uvz.x - uvz.y >= 0) && (1 - uvz.x - uvz.y <= 1))
                        {
                            if (x == 357 && y == 520)
                                int kek = 0;
                            if (uvzWIthMinZ.z > uvz.z)
                            {
                                //{x=-0.498595476 y=-0.498559058 }
                                //{x=-0.498595476 y=-0.498559058 }
                                uvzWIthMinZ = uvz;
                                triangleWithMinDepthID = i / 3;
                            }
                        }
                    }
                    float4 result = { uvzWIthMinZ.x, uvzWIthMinZ.y, uvzWIthMinZ.z, triangleWithMinDepthID };
                    memcpy((char*)depthStencil->texture2D[x] + y * depthStencil->format, &result, depthStencil->format);

                    float4 test = *(float4*)((char*)depthStencil->texture2D[x] + (y * depthStencil->format));
                    float id = *(float*)((char*)depthStencil->texture2D[x] + (y * depthStencil->format) + sizeof(float3));
                    int point = 0;
                }

        }

        if (!depthStencil)
        {
            for (int i = 0; i < verticesCount; i += 3)
                for (int x = viewPort.left; x < viewPort.right; x++)
                    for (int y = viewPort.top; y < viewPort.bottom; y++)
                    {

                        float2 pixelPos = float2{ (x / width) * 2 - 1, ((height - y) / height) * 2 - 1 };

                        float4 color = DrawTriangle(pixelPos, i);

                        if (color.w != 0)
                            memcpy((char*)(*swapChain)->backBuffers[(*swapChain)->currentBackBufferId].texture2D[x] + (y * (*swapChain)->backBuffers->format), &color, (*swapChain)->backBuffers->format);
                    }
        }
        else
        {
            for (int x = viewPort.left; x < viewPort.right; x++)
                for (int y = viewPort.top; y < viewPort.bottom; y++)
                {
                    int triangleID = *(float*)((char*)depthStencil->texture2D[x] + (y * depthStencil->format) + sizeof(float3));
                    if (triangleID != BIG_NUM)
                    {
                        float kek = height / width;
                        float2 pixelPos = float2{ ((x / width) * 2 - 1) * 100, (((height - y) / height) * 2 - 1) };
                        float3 uvz = *(float3*)((char*)depthStencil->texture2D[x] + (y * depthStencil->format));
                        float4 color = DrawTriangle(pixelPos, triangleID * 3, uvz);

                        if (color.w != 0)
                            memcpy((char*)(*swapChain)->backBuffers[(*swapChain)->currentBackBufferId].texture2D[x] + (y * (*swapChain)->backBuffers->format), &color, (*swapChain)->backBuffers->format);
                    }
                }
        }

    }
    else
        throw;
}

