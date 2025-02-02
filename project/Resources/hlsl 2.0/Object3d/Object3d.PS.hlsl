//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include "Object3d.hlsli"

//=========================================================================================
// Output
//=========================================================================================
struct PSOutput {
	float4 color : SV_Target0;
};

//=========================================================================================
// Buffer
//=========================================================================================
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////
PSOutput main(VSOutput input) {
	
	PSOutput output;
	
	output.color = gTexture.Sample(gSampler, input.texcoord);
	
	return output;
	
}