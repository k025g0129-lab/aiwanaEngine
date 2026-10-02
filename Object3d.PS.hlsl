#include "Object3d.hlsli"


Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct Material{
    float32_t4 color;
    int32_t lightngType;
    float32_t4x4 uvTransform;
};

struct TransformationMaterial{
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
    
    
    float4 transformedUV = mul(float32_t4(input.texcoord,0.0f,1.0f),gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    PixelShaderOutput output;
    output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
    output.color.a = gMaterial.color.a * textureColor.a;
    
    
    
    if (gMaterial.lightngType == 0){
    // None
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else if (gMaterial.lightngType == 1)
    {
    // Lambert

        float NdotL =
        saturate(dot(normalize(input.normal),
                     -gDirectionalLight.direction));

        output.color.rgb =
        gMaterial.color.rgb *
        textureColor.rgb *
        gDirectionalLight.color.rgb *
        NdotL *
        gDirectionalLight.intensity;
        
        output.color.a =
        gMaterial.color.a *
        textureColor.a;

    }
    else if (gMaterial.lightngType == 2)
    {
    // Half Lambert

        float NdotL =
        dot(normalize(input.normal),
            -gDirectionalLight.direction);

        float diffuse =
        pow(NdotL * 0.5f + 0.5f, 2.0f);

        output.color.rgb =
        gMaterial.color.rgb *
        textureColor.rgb *
        gDirectionalLight.color.rgb *
        diffuse *
        gDirectionalLight.intensity;
        
        output.color.a =
        gMaterial.color.a *
        textureColor.a;

    }
    
    

    
    return output;
}



/*float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}*/