#include "Object3d.hlsli"


Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct Material{
    float32_t4 color;
    int32_t enableLighting;
};

struct TransformationMaterial
{
    float32_t4x4 WVP;
    float32_t4x4 World;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};


ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct PixelShaderOutput{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input){   
    
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    PixelShaderOutput output;
    output.color = gMaterial.color * textureColor;
    
    if (gMaterial.enableLighting != 0){
        
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
        
    } else {
        
        output.color = gMaterial.color * textureColor;
        
        }
    
    return output;
}



/*float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}*/