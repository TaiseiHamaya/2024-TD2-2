#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
#include <object.h>

#include <Model.h>

////////////////////////////////////////////////////////////////////////////////////////////
// MultiMesh class
////////////////////////////////////////////////////////////////////////////////////////////
class MultiMesh
	: public Object {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	//! @breif コンストラクタ
	MultiMesh() { Init(); }

	//! @brief デストラクタ
	~MultiMesh() { Term(); }

	//! @brief 初期化処理
	void Init() override;

	//! @brief ImGui
	void EditorImGui(int identifier);

	//! @brief 描画処理
	//! 
	//! @param[in] commandList
	void Draw(ID3D12GraphicsCommandList* commandList, Directional* directionalLight) override;

	//! @brief 終了処理
	void Term() override;

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	Object_Ptr<Model> model_;

};