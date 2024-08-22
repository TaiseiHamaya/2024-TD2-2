#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* DxObjectCommon
#include <DxObjectCommon.h>

////////////////////////////////////////////////////////////////////////////////////////////
// DxObject
////////////////////////////////////////////////////////////////////////////////////////////
_DXOBJECT_NAMESPACE_BEGIN

//-----------------------------------------------------------------------------------------
// forward
//-----------------------------------------------------------------------------------------
class Devices;

////////////////////////////////////////////////////////////////////////////////////////////
// Fence class
////////////////////////////////////////////////////////////////////////////////////////////
class Fence {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	//! @brief コンストラクタ
	//! 
	//! @param[in] devices DxObject::Devices
	Fence(Devices* devices) { Init(devices); }

	//! @brief デストラクタ
	~Fence() { Term(); }

	//! @brief 初期化処理/
	//! 
	//! @param[in] devices DxObject::Devices
	void Init(Devices* devices);

	//! @brief 終了処理
	void Term();

	//! @brief fenceValueをインクリメント
	void AddFenceValue();

	//! @brief GPUのイベントを待つ
	void WaitGPU();

	//! @brief フェンスを取得
	//! 
	//! @return フェンスを返却
	ID3D12Fence* const GetFence() const { return fence_.Get(); }

	//! @brief fenceValueを取得
	//! 
	//! @return fenceValueを返却
	uint64_t GetFenceValue() const { return fenceValue_; }

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	ComPtr<ID3D12Fence> fence_;

	uint64_t fenceValue_;
	HANDLE   fenceEvent_;
};

_DXOBJECT_NAMESPACE_END