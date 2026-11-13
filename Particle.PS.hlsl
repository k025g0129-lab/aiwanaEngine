#include "Particle.hlsli"


Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct Material{
    float4 color;
    int lightngType;
    float4x4 uvTransform;
};

struct TransformationMaterial{
    float4x4 WVP;
    float4x4 World;
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};


//ConstantBuffer<Material> gMaterial : register(b0);
//ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

cbuffer MaterialBuffer : register(b0)
{
    Material gMaterial;
};

cbuffer DirectionalLightBuffer : register(b1)
{
    DirectionalLight gDirectionalLight;
};


struct PixelShaderOutput{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input){   
    
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    output.color = gMaterial.color * textureColor;

    if (output.color.a == 0.0){
        discard;
    }
    
    return output;
}



/*float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}*/