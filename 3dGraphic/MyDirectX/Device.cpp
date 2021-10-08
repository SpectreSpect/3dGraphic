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

float4 Device::DrawTriangle(float2 pixelPos, int firstVertexID)
{
    if (inputLayout)
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


        //float3 A = *(float3*)((char*)changedVertexBuffer.buffer + firstVertexID * changedVertexBuffer.vertexSize);
        //float3 B = *(float3*)((char*)changedVertexBuffer.buffer + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
        //float3 C = *(float3*)((char*)changedVertexBuffer.buffer + (firstVertexID + 2) * changedVertexBuffer.vertexSize);

        //float3 A = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + firstVertexID * changedVertexBuffer.vertexSize);
        //float3 B = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
        //float3 C = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + (firstVertexID + 2) * changedVertexBuffer.vertexSize);


        //float3 verticesPos[] =
        //{
        //    -1, -1, 1,  1, 0, 0, 1,
        //    0, 1, 1,    0, 1, 0, 1,
        //    1, -1, 1,   0, 0, 1, 1
        //};

        float3 A = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex) + firstVertexID * changedVertexBuffer.vertexSize;
        float3 B = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + (firstVertexID + 1) * changedVertexBuffer.vertexSize);
        float3 C = *(float3*)((char*)changedVertexBuffer.buffer + inputLayout->attributeBuffer[posID].offsetInVertex + (firstVertexID + 2) * changedVertexBuffer.vertexSize);

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
            int byteCountInVertex = inputLayout->attributeBuffer[inputLayout->elementsCount - 1].offsetInVertex + inputLayout->attributeBuffer[inputLayout->elementsCount - 1].format;
            void* vertex = new char[byteCountInVertex];

            for (int i = 0; i < inputLayout->elementsCount; i++)
            {
                int offset = inputLayout->attributeBuffer[i].offsetInVertex;
                if (inputLayout->attributeBuffer[i].format == DXGI_FORMAT_R32G32B32_FLOAT)
                {
                    //float3 float3Element = *(float3*)((char*)changedVertexBuffer.buffer + offset);
                    //float3Element = float3{ float3Element.x * u, float3Element.y * v, float3Element.z * (1 - u - v) };

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
            //float4 color = float4{ 1, 1, 1, 1 };
            float4 color = ps->Execute(vertex);

            return color;
            //return ps->Execute();
        }
        return float4{ 0, 0, 0, 0 };
    }
    else throw; 
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

        float3* kek = new float3;

        for (int i = 0; i < verticesCount; i += 3)
            for (int x = viewPort.left; x < viewPort.right; x++)
                for (int y = viewPort.top; y < viewPort.bottom; y++)
                {
                    float2 pixelPos = float2{ (x / width) * 2 - 1, ((height - y) / height) * 2 - 1 };

                    float4 color = DrawTriangle(pixelPos, i);

                    memcpy((char*)(*swapChain)->backBuffers[(*swapChain)->currentBackBufferId].texture2D[x] + (y * (*swapChain)->backBuffers->format), &color, (*swapChain)->backBuffers->format);
                }
    }
    else
        throw;
}
