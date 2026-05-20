
struct VertexShaderOutput{
    
    float32_t4 pos : SV_POSITION;
	
}; 

struct VertexShaderInput{
    
    float32_t4 pos : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input){
    VertexShaderOutput output;
    output.pos = input.pos;
    return output;
}




/*float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}*/

 