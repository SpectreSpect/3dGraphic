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

    if(this->depthStencil)
        for(int x = 0; x < this->depthStencil->size.x; x++)
            for (int y = 0; y < this->depthStencil->size.y; y++)
                *(float*)(&(((char**)this->depthStencil->texture2D)[x][y * this->depthStencil->format])) = BIG_NUM;
}

void Device::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    this->indexBuffer = indexBuffer;
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


float4  Device::DrawTriangle(float2 pixelPos, float2 _PixelPos, int firstVertexID, void* localVertex)
{
    float** depthStencilTexture2D = (float**)depthStencil->texture2D;
    if (inputLayout)
    {
        float3 uvz = GetUVZ(_PixelPos, firstVertexID);
        float u = uvz.x;
        float v = uvz.y;
        float z = uvz.z;

        if ((u + v <= 1) && (u >= 0) && (u <= 1) && (v >= 0) && (v <= 1) && (1 - u - v >= 0) && (1 - u - v <= 1))
        {
            if (z < depthStencilTexture2D[(int)pixelPos.x][(int)pixelPos.y])
            {
                depthStencilTexture2D[(int)pixelPos.x][(int)pixelPos.y] = z;
                for (int i = 0; i < inputLayout->elementsCount; i++)
                {
                    int offset = inputLayout->attributeBuffer[i].offsetInVertex;
                    switch (inputLayout->attributeBuffer[i].format)
                    {
                    case DXGI_FORMAT_R32G32B32_FLOAT:
                    {
                        float3 elementVertex1 = *(float3*)((char*)changedVertexBuffer.buffer + offset + firstVertexID * changedVertexBuffer.vertexSize);
                        float3 elementVertex2 = *(float3*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
                        float3 elementVertex3 = *(float3*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 2) * changedVertexBuffer.vertexSize);
                        float3 element = elementVertex1 * (1 - u - v) + elementVertex2 * u + elementVertex3 * v;
                        memcpy((char*)localVertex + offset, &element, sizeof(float3));
                        break;
                    }
                    case DXGI_FORMAT_R32G32B32A32_FLOAT:
                    {
                        float4 elementVertex1 = *(float4*)((char*)changedVertexBuffer.buffer + offset + firstVertexID * changedVertexBuffer.vertexSize);
                        float4 elementVertex2 = *(float4*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
                        float4 elementVertex3 = *(float4*)((char*)changedVertexBuffer.buffer + offset + (firstVertexID + 2) * changedVertexBuffer.vertexSize);
                        float4 element = elementVertex1 * (1 - u - v) + elementVertex2 * u + elementVertex3 * v;
                        memcpy((char*)localVertex + offset, &element, sizeof(float4));
                        break;
                    }
                    }
                }

                if (firstVertexID == 0)
                    int pint = 5;

                float4 color = ps->Execute(localVertex);
                return color;
            }
        }
        else
            return float4{ 0, 0, 0, 0 };
    }
    else throw;
}

void Device::ClearBuffer(float4 color, Texture2D* texture)
{
    for (int x = 0; x < texture->size.x; x++)
        for (int y = 0; y < texture->size.y; y++)
            memcpy((char*)texture->texture2D[x] + y * texture->format, &color, sizeof(float4));
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

        int byteCountInVertex = inputLayout->attributeBuffer[inputLayout->elementsCount - 1].offsetInVertex + inputLayout->attributeBuffer[inputLayout->elementsCount - 1].format;
        void* vertex = new char[byteCountInVertex];

        for (int x = viewPort.left; x < viewPort.right; x++)
            for (int y = viewPort.top; y < viewPort.bottom; y++)
            {
                float transfromCoef = width / height;
                float2 _PixelPos = float2{ ((x / width) * 2 - 1) * transfromCoef, ((height - y) / height) * 2 - 1 };
                float2 pixelPos = float2{ (float)x, (float)y };
                float4 color{};
                for (int i = 0; i < verticesCount; i += 3)
                {
                    float4 tempColor = DrawTriangle(pixelPos, _PixelPos, i, vertex);
                    if(tempColor.w != 0)
                        color = tempColor;
                }
                    if (color.w != 0)
                        memcpy((char*)(*swapChain)->backBuffers[(*swapChain)->currentBackBufferId].texture2D[x] + (y * (*swapChain)->backBuffers->format), &color, (*swapChain)->backBuffers->format);
            }

    }
    else
        throw;
}

