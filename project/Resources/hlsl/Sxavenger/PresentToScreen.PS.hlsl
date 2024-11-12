//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include "PresentToScreen.hlsli"

//=========================================================================================
// Buffer
//=========================================================================================

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler      : register(s0);

////////////////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////////////////
PSOutput main(VSOutput input) {

	PSOutput output = (PSOutput)0;

	output.color = gTexture.Sample(gSampler, input.texcoord);
	output.color.a = 1.0f;
	
	return output;
}