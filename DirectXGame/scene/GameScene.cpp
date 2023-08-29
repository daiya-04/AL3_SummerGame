#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Hit.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.Initialize();


	ModelSet();
	
	player_ = std::make_unique<Player>();
	std::vector<Model*> playerModels = {
	    playerBodyModel_.get(),playerHeadModel_.get(),
		playerBulletModel_.get()
	};
	player_->Initialize(playerModels);
	player_->SetGameScene(this);

	enemy_ = std::make_unique<Enemy>();
	std::vector<Model*> enemyModels = {
		enemyBodyModel_.get(),enemyBodyModel2_.get(),
		enemyBodyModel3_.get(),enemyHeadModel_.get(),
		enemyBulletModel_.get()
	};
	enemy_->Initialize(enemyModels);
	enemy_->SetGameScene(this);

	player_->SetEnemy(enemy_.get());

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
	camera_->SetTarget(&player_->GetWorldTransformBase());
	
	player_->SetViewProjection(&camera_->GetViewProjection());

}

void GameScene::Update() {

	playerBullets_.remove_if([](const std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->isDead()) {
			return true;
		}
		return false;
	});

	enemyBullets_.remove_if([](const std::unique_ptr<EnemyBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
	});

	player_->Update();
	for (const auto& bullet : playerBullets_) {
		bullet->Update();
	}
	for (const auto& bullet : enemyBullets_) {
		bullet->Update();
	}
	
	enemy_->Update();
	skydome_->Update();
	ground_->Update();
	camera_->Update();

	viewProjection_.matView = camera_->GetViewProjection().matView;
	viewProjection_.matProjection = camera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);
	for (const auto& bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}
	for (const auto& bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

	CheckAllCollision();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::ModelSet() {

	skydomeModel_.reset(Model::CreateFromOBJ("skydome", true));
	groundModel_.reset(Model::CreateFromOBJ("ground", true));

	playerBodyModel_.reset(Model::CreateFromOBJ("float_Body", true));
	playerHeadModel_.reset(Model::CreateFromOBJ("float_Head", true));

	playerBulletModel_.reset(Model::CreateFromOBJ("bullet", true));

	enemyBodyModel_.reset(Model::CreateFromOBJ("EnemyBody", true));
	enemyBodyModel2_.reset(Model::CreateFromOBJ("EnemyBody", true));
	enemyBodyModel3_.reset(Model::CreateFromOBJ("EnemyBody", true));
	enemyHeadModel_.reset(Model::CreateFromOBJ("EnemyBody", true));

	enemyBulletModel_.reset(Model::CreateFromOBJ("enemyBullet", true));
}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	playerBullets_.push_back(std::unique_ptr<PlayerBullet>(playerBullet));
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	enemyBullets_.push_back(std::unique_ptr<EnemyBullet>(enemyBullet));
}

void GameScene::CheckAllCollision() {

	AABB enemy = {
	    enemy_->GetWorldPos() - enemy_->GetSize(),
		enemy_->GetWorldPos() + enemy_->GetSize()
	};

	AABB player = {
	    player_->GetWorldPos() - player_->GetSize(), 
		player_->GetWorldPos() + player_->GetSize()
	};

	//敵と自弾との衝突判定
	for (const auto& bullet : playerBullets_) {
		Sphere playerBullet = {bullet->GetWorldPos(), 1.0f};
		if (IsCollision(enemy, playerBullet)) {
			bullet->OnCollision();
			enemy_->OnCollision();
		}
	}

	//プレイヤーと敵弾との衝突判定
	for (const auto& bullet : enemyBullets_) {
		Sphere enemyBullet = {bullet->GetWorldPos(), 1.0f};
		if (IsCollision(player, enemyBullet)) {
			bullet->OnCollision();
			player_->OnCollision();
		}
	}

	//自弾と敵弾との衝突判定
	for (const auto& bulletA : playerBullets_) {
		Sphere playerBullet = {bulletA->GetWorldPos(), 1.0f};
		for (const auto& bulletB : enemyBullets_) {
			Sphere enemyBullet = {bulletB->GetWorldPos(), 1.0f};
			if (IsCollision(playerBullet, enemyBullet)) {
				bulletA->OnCollision();
				bulletB->OnCollision();
			}
		}
	}

}