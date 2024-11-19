#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include "PhysicalBasedRendering.hlsli"

////////////////////////////////////////////////////////////////////////////////////////////
// Output structure
////////////////////////////////////////////////////////////////////////////////////////////

struct SystematicOutput {
	float4 albedo   : SV_Target0;
	float4 normal   : SV_Target1;
	float4 position : SV_Target2;
	float4 material : SV_Target3;
};

////////////////////////////////////////////////////////////////////////////////////////////
// methods
////////////////////////////////////////////////////////////////////////////////////////////

float4 ToAlbedo(float4 albedo) {
	return albedo;
}

float4 ToNormal(float3 normal) {
	float3 fixedNormal = normalize(normal) * 0.5f + 0.5f;
	return float4(fixedNormal, 1.0f);
}

float4 ToPosition(float3 position) {
	return float4(position, 1.0f);
}

float4 ToMaterial(PBRMaterial material) {
	return float4(material.roughness, material.metallic, 0.0f, 1.0f);
}