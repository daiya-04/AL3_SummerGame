#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Vec3.h"
#include "Matrix44.h"
#include <memory>
#include <vector>
#include "Skydome.h"
#include "Ground.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include <list>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void ModelSet();

	void AddPlayerBullet(PlayerBullet* playerBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	ViewProjection viewProjection_;

	//プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerBodyModel_;
	std::unique_ptr<Model> playerHeadModel_;
	std::unique_ptr<Model> playerL_armModel_;
	std::unique_ptr<Model> playerR_armModel_;

	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;

	//敵
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> enemyBodyModel_;

	//天球
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Skydome> skydome_;
	
	//地面
	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Ground> ground_;


};
