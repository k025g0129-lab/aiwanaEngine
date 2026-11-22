#include "Particle.hlsli"   



struct TransformationMatrix{
    float4x4 WVP;
    float4x4 World;
    
};
StructuredBuffer<TransformationMatrix> gTransformationMatrix : register(t0);



struct VertexShaderInput{   
    
    float4 pos : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;    
};

VertexShaderOutput main(VertexShaderInput input,uint instanceId : SV_InstanceID){
    VertexShaderOutput output;  
    output.pos = mul(input.pos, gTransformationMatrix[instanceId].WVP);
    output.texcoord = input.texcoord;
      output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix[instanceId].World));
    
    return output;  
}




/*float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}*/

 