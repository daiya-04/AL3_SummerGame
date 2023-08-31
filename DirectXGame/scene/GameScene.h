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
#include "EnemyBullet.h"
#include <list>
#include "Camera.h"

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

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void CheckAllCollision();

	void ToClearScene();

	void ToGameOverScene();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	XINPUT_STATE preJoyState{};
	
	enum class Scene {
		Title,
		InGame,
		Clear,
		GameOver,
	};

	Scene scene_ = Scene::Title;

	ViewProjection viewProjection_;
	

	//プレイヤー
	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerBodyModel_;
	std::unique_ptr<Model> playerHeadModel_;

	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;
	std::unique_ptr<Model> playerBulletModel_;

	//敵
	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> enemyBodyModel1_;
	std::unique_ptr<Model> enemyBodyModel2_;
	std::unique_ptr<Model> enemyBodyModel3_;
	std::unique_ptr<Model> enemyHeadModel_;

	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	std::unique_ptr<Model> enemyBulletModel_;

	//天球
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Skydome> skydome_;
	
	//地面
	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Ground> ground_;

	//カメラ
	std::unique_ptr<Camera> camera_;

	uint32_t titleTexture_ = 0u;
	uint32_t button_BTexture_ = 0u;
	uint32_t enemyHpTexture_ = 0u;
	uint32_t playerHpIconTexture_ = 0u;
	uint32_t gameOverTexture_ = 0u;
	uint32_t gameClearTexture_ = 0u;


	std::unique_ptr<Sprite> title_;
	std::unique_ptr<Sprite> button_B_;
	std::unique_ptr<Sprite> enemyHp_;
	std::unique_ptr<Sprite> player_hp_icon1_;
	std::unique_ptr<Sprite> player_hp_icon2_;
	std::unique_ptr<Sprite> player_hp_icon3_;
	std::unique_ptr<Sprite> player_hp_icon4_;
	std::unique_ptr<Sprite> player_hp_icon5_;
	std::unique_ptr<Sprite> gameOverText_;
	std::unique_ptr<Sprite> gameClearText_;

	Vector2 size_ = {800.0f, 25.0f};

	int count_ = 0;
};
