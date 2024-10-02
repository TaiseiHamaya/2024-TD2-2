#include "InputMesh.h"
_DXOBJECT_USING

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
// meshoptimizer
#include <meshoptimizer.h>

////////////////////////////////////////////////////////////////////////////////////////////
// InputMesh class methods
////////////////////////////////////////////////////////////////////////////////////////////

void InputMesh::CreateMeshlet() {

	//!< 最適化は除外
	//!< bufferが増えるので
	
	//!< bufferで使う構造体
	// meshlet情報
	std::vector<DirectX::Meshlet>         bufferMeshlets;
	std::vector<uint32_t>                 bufferUniqueVertexIndices;
	std::vector<DirectX::MeshletTriangle> bufferPrimitiveIndices;

	// cullData情報
	std::vector<DirectX::CullData>        bufferCullDatas;

	// positionを別構造体へ
	std::vector<DirectX::XMFLOAT3> positions;
	positions.reserve(vertices_->GetIndexSize());

	for (uint32_t i = 0; i < vertices_->GetIndexSize(); ++i) {
		DirectX::XMFLOAT3 element = {};

		element.x = (*vertices_)[i].position.x;
		element.y = (*vertices_)[i].position.y;
		element.z = (*vertices_)[i].position.z;

		positions.emplace_back(element);
	}

	// meshletの情報の生成
	{

		std::vector<uint8_t> uniqueVertexIB;

		auto hr = DirectX::ComputeMeshlets(
			indices_->GetData(), indices_->GetIndexSize() / 3,
			positions.data(), positions.size(),
			nullptr,
			bufferMeshlets,
			uniqueVertexIB,
			bufferPrimitiveIndices,
			kMaxVertices_,
			kMaxPrimitives_
		);

		Assert(SUCCEEDED(hr));

		// uint8_tから梱包してtriangles(uint32_t)に変換
		bufferUniqueVertexIndices.reserve(uniqueVertexIB.size() / sizeof(uint32_t));

		const uint32_t* datas = reinterpret_cast<const uint32_t*>(uniqueVertexIB.data());
		size_t count = uniqueVertexIB.size() / sizeof(uint32_t);

		for (size_t i = 0; i < count; ++i) {
			bufferUniqueVertexIndices.push_back(datas[i]);
		}
	}

	// cullData情報の生成
	{

		// メモリの確保
		bufferCullDatas.resize(bufferMeshlets.size());

		auto hr = DirectX::ComputeCullData(
			positions.data(), positions.size(),
			bufferMeshlets.data(), bufferMeshlets.size(),
			bufferUniqueVertexIndices.data(), bufferUniqueVertexIndices.size(),
			bufferPrimitiveIndices.data(), bufferPrimitiveIndices.size(),
			bufferCullDatas.data()
		);

		Assert(SUCCEEDED(hr));
	}

	//!< meshletBufferの生成
	{

		// _DXOBJECT Deviceの取り出し
		auto deviceObj = Sxavenger::GetDevicesObj();

		// uniqueVertexIndicesの初期化
		uniqueVertexIndices_ = std::make_unique<BufferResource<uint32_t>>(deviceObj, static_cast<uint32_t>(bufferUniqueVertexIndices.size()));
		uniqueVertexIndices_->Memcpy(bufferUniqueVertexIndices.data());

		// primitiveIndicesの初期化
		primitiveIndices_ = std::make_unique<BufferResource<DirectX::MeshletTriangle>>(deviceObj, static_cast<uint32_t>(bufferPrimitiveIndices.size()));
		primitiveIndices_->Memcpy(bufferPrimitiveIndices.data());

		// meshletsの初期化
		meshlets_ = std::make_unique<BufferResource<DirectX::Meshlet>>(deviceObj, static_cast<uint32_t>(bufferMeshlets.size()));
		meshlets_->Memcpy(bufferMeshlets.data());

		// cullDataの初期化
		cullDatas_ = std::make_unique<BufferResource<DirectX::CullData>>(deviceObj, static_cast<uint32_t>(bufferCullDatas.size()));
		cullDatas_->Memcpy(bufferCullDatas.data());

		// mesh情報を初期化
		meshInfo_ = std::make_unique<BufferResource<MeshInfo>>(Sxavenger::GetDevicesObj(), 1);
		(*meshInfo_)[0].meshletCount = static_cast<uint32_t>(bufferMeshlets.size());
	}

	isCreateMeshlet_ = true;
}

//void InputMesh::CreateMeshlet() {
//
//	//!< 最適化は除外
//	//!< bufferが増えるので
//	
//	//!< bufferで使う構造体
//	// meshlet情報
//	std::vector<DirectX::Meshlet>         bufferMeshlets;
//	std::vector<uint32_t>                 bufferUniqueVertexIndices;
//	std::vector<DirectX::MeshletTriangle> bufferPrimitiveIndices;
//
//	// cullData情報
//	std::vector<DirectX::CullData>        bufferCullDatas;
//
//
//	// meshletの情報の生成
//	{
//
//		std::vector<meshopt_Meshlet> meshlets(
//			meshopt_buildMeshletsBound(
//				indices_->GetIndexSize(),
//				kMaxVertices_,
//				kMaxPrimitives_
//			)
//		);
//
//		meshlets.resize(
//			meshopt_buildMeshlets(
//				meshlets.data(),
//				indices_->GetData(),
//				indices_->GetIndexSize(),
//				vertices_->GetIndexSize(),
//				kMaxVertices_,
//				kMaxPrimitives_
//			)
//		);
//
//		// 最大値でのメモリ予約
//		bufferMeshlets.reserve(meshlets.size());
//		bufferUniqueVertexIndices.reserve(meshlets.size() * kMaxVertices_);
//		bufferPrimitiveIndices.reserve(meshlets.size() * kMaxPrimitives_);
//
//		for (const auto& meshlet : meshlets) {
//			auto vertexOffset    = static_cast<uint32_t>(bufferUniqueVertexIndices.size());
//			auto primitiveOffset = static_cast<uint32_t>(bufferPrimitiveIndices.size());
//
//			for (auto i = 0u; i < meshlet.vertex_count; ++i) {
//				bufferUniqueVertexIndices.emplace_back(meshlet.vertices[i]);
//			}
//
//			for (auto i = 0; i < meshlet.triangle_count; ++i) {
//				DirectX::MeshletTriangle tris = {};
//				tris.i0 = meshlet.indices[i][0];
//				tris.i1 = meshlet.indices[i][1];
//				tris.i2 = meshlet.indices[i][2];
//				bufferPrimitiveIndices.emplace_back(tris);
//			}
//
//			// メッシュレットデータの設定
//			DirectX::Meshlet m = {};
//			m.VertCount  = meshlet.vertex_count;
//			m.VertOffset = vertexOffset;
//			m.PrimCount  = meshlet.triangle_count;
//			m.PrimOffset = primitiveOffset;
//
//			bufferMeshlets.emplace_back(m);
//		}
//		
//		// サイズの最適化
//		bufferUniqueVertexIndices.shrink_to_fit();
//		bufferPrimitiveIndices.shrink_to_fit();
//		bufferMeshlets.shrink_to_fit();
//	}
//
//	// cullData情報の生成
//	{
//		
//		// DirectX::XMFLOAT3に情報を移す
//		std::vector<DirectX::XMFLOAT3> vertexPostions;
//		vertexPostions.reserve(vertices_->GetIndexSize());
//
//		for (uint32_t i = 0; i < vertices_->GetIndexSize(); ++i) {
//			DirectX::XMFLOAT3 element = {};
//
//			element.x = (*vertices_)[i].position.x;
//			element.y = (*vertices_)[i].position.y;
//			element.z = (*vertices_)[i].position.z;
//
//			vertexPostions.emplace_back(element);
//		}
//
//		// メモリの確保
//		bufferCullDatas.resize(bufferMeshlets.size());
//
//		auto hr = DirectX::ComputeCullData(
//			vertexPostions.data(), vertexPostions.size(),
//			bufferMeshlets.data(), bufferMeshlets.size(),
//			bufferUniqueVertexIndices.data(), bufferUniqueVertexIndices.size(),
//			bufferPrimitiveIndices.data(), bufferPrimitiveIndices.size(),
//			bufferCullDatas.data()
//		);
//
//		Assert(SUCCEEDED(hr));
//	}
//
//	//!< meshletBufferの生成
//	{
//
//		// _DXOBJECT Deviceの取り出し
//		auto deviceObj = Sxavenger::GetDevicesObj();
//
//		// uniqueVertexIndicesの初期化
//		uniqueVertexIndices_ = std::make_unique<BufferResource<uint32_t>>(deviceObj, static_cast<uint32_t>(bufferUniqueVertexIndices.size()));
//		uniqueVertexIndices_->Memcpy(bufferUniqueVertexIndices.data());
//
//		// primitiveIndicesの初期化
//		primitiveIndices_ = std::make_unique<BufferResource<DirectX::MeshletTriangle>>(deviceObj, static_cast<uint32_t>(bufferPrimitiveIndices.size()));
//		primitiveIndices_->Memcpy(bufferPrimitiveIndices.data());
//
//		// meshletsの初期化
//		meshlets_ = std::make_unique<BufferResource<DirectX::Meshlet>>(deviceObj, static_cast<uint32_t>(bufferMeshlets.size()));
//		meshlets_->Memcpy(bufferMeshlets.data());
//
//		// cullDataの初期化
//		cullDatas_ = std::make_unique<BufferResource<DirectX::CullData>>(deviceObj, static_cast<uint32_t>(bufferCullDatas.size()));
//		cullDatas_->Memcpy(bufferCullDatas.data());
//
//		// mesh情報を初期化
//		meshInfo_ = std::make_unique<BufferResource<MeshInfo>>(Sxavenger::GetDevicesObj(), 1);
//		(*meshInfo_)[0].meshletCount = static_cast<uint32_t>(bufferMeshlets.size());
//	}
//
//	isCreateMeshlet_ = true;
//}

void InputMesh::Dispatch(UINT verticesParam, UINT uniqueVertexIndicesParam, UINT meshletsParam, UINT primitiveIndices, UINT cullDataParam, UINT meshInfoParam) const {

	Assert(isCreateMeshlet_, "Not create meshlets.");

	// commandListの取得
	auto commandList = Sxavenger::GetCommandList();

	// descriptorの設定
	commandList->SetGraphicsRootShaderResourceView(verticesParam, vertices_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootShaderResourceView(uniqueVertexIndicesParam, uniqueVertexIndices_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootShaderResourceView(meshletsParam, meshlets_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootShaderResourceView(primitiveIndices, primitiveIndices_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootShaderResourceView(cullDataParam, cullDatas_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(meshInfoParam, meshInfo_->GetGPUVirtualAddress());

	commandList->DispatchMesh(RoundUp(static_cast<UINT>(meshlets_->GetIndexSize()), kAmplificationNumthread_), 1, 1);

}