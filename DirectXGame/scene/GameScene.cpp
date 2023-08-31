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

	titleTexture_ = TextureManager::Load("Title.png");
	button_BTexture_ = TextureManager::Load("Button_B.png");
	enemyHpTexture_ = TextureManager::Load("white1x1.png");
	playerHpIconTexture_ = TextureManager::Load("player_HP_icon.png");
	gameOverTexture_ = TextureManager::Load("GameOver.png");
	gameClearTexture_ = TextureManager::Load("GameClear.png");
	
	player_ = std::make_unique<Player>();
	std::vector<Model*> playerModels = {
	    playerBodyModel_.get(),playerHeadModel_.get(),
		playerBulletModel_.get()
	};
	player_->Initialize(playerModels);
	player_->SetGameScene(this);

	enemy_ = std::make_unique<Enemy>();
	std::vector<Model*> enemyModels = {
		enemyBodyModel1_.get(),enemyBodyModel2_.get(),
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

	title_.reset(Sprite::Create(titleTexture_, {0, 0}));
	button_B_.reset(Sprite::Create(button_BTexture_, {590, 400}));
	enemyHp_.reset(Sprite::Create(enemyHpTexture_, {400, 50}));
	player_hp_icon1_.reset(Sprite::Create(playerHpIconTexture_, {20, 20}));
	player_hp_icon2_.reset(Sprite::Create(playerHpIconTexture_, {70, 20}));
	player_hp_icon3_.reset(Sprite::Create(playerHpIconTexture_, {120, 20}));
	player_hp_icon4_.reset(Sprite::Create(playerHpIconTexture_, {170, 20}));
	player_hp_icon5_.reset(Sprite::Create(playerHpIconTexture_, {220, 20}));
	gameOverText_.reset(Sprite::Create(gameOverTexture_, {0, 0}));
	gameClearText_.reset(Sprite::Create(gameClearTexture_, {0, 0}));
	
}

void GameScene::Update() {
	
	XINPUT_STATE joyState;

	skydome_->Update();
	ground_->Update();

	if (scene_ == Scene::Title) {
		count_++;
		if (count_ > 90) { count_ = 0; }
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (!(preJoyState.Gamepad.wButtons& XINPUT_GAMEPAD_B) && (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
				scene_ = Scene::InGame;
				enemy_->SceneInitialize();
				player_->SceneInitialize();
				count_ = 0;
				size_.x = 800.0f;
			}
		}
		
	}

	

	if (scene_ == Scene::InGame) {
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
		
		camera_->Update();

		enemyHp_->SetSize(size_);

		viewProjection_.matView = camera_->GetViewProjection().matView;
		viewProjection_.matProjection = camera_->GetViewProjection().matProjection;
	}

	if (scene_ == Scene::Clear || scene_ == Scene::GameOver) {
		for (const auto& bullet : playerBullets_) {
			bullet->OnCollision();
		}
		for (const auto& bullet : enemyBullets_) {
			bullet->OnCollision();
		}
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) && (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
				scene_ = Scene::Title;
				count_ = 0;
			}
		}
		count_++;
		if (count_ > 90) { count_ = 0; }
	}
	
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		preJoyState = joyState;
	}
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
	
	
	if (scene_ == Scene::InGame) {
		player_->Draw(viewProjection_);
		for (const auto& bullet : playerBullets_) {
			bullet->Draw(viewProjection_);
		}
		for (const auto& bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}
		enemy_->Draw(viewProjection_);
		

		CheckAllCollision();
	}
	skydome_->Draw(viewProjection_);
	ground_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	if (scene_ == Scene::Title) {
		title_->Draw();
		
	}
	
	if (scene_ != Scene::InGame) {
		if (count_ <= 60) {
			button_B_->Draw();
		}
	}

	if (scene_ == Scene::InGame) {
		enemyHp_->Draw();
		if (player_->GetHP() >= 1) { player_hp_icon1_->Draw(); }
		if (player_->GetHP() >= 2) { player_hp_icon2_->Draw(); }
		if (player_->GetHP() >= 3) { player_hp_icon3_->Draw(); }
		if (player_->GetHP() >= 4) { player_hp_icon4_->Draw(); }
		if (player_->GetHP() >= 5) { player_hp_icon5_->Draw(); }
	}

	if (scene_ == Scene::Clear) {
		gameClearText_->Draw();
	}
	if (scene_ == Scene::GameOver) {
		gameOverText_->Draw();
	}

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

	enemyBodyModel1_.reset(Model::CreateFromOBJ("enemy_Body1", true));
	enemyBodyModel2_.reset(Model::CreateFromOBJ("enemy_Body2", true));
	enemyBodyModel3_.reset(Model::CreateFromOBJ("enemy_Body3", true));
	enemyHeadModel_.reset(Model::CreateFromOBJ("EnemyHead", true));

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

	float redius = 0.7f;

	//敵と自弾との衝突判定
	for (const auto& bullet : playerBullets_) {
		Sphere playerBullet = {bullet->GetWorldPos(), redius};
		if (IsCollision(enemy, playerBullet)) {
			bullet->OnCollision();
			enemy_->OnCollision();
			size_.x -= 4.0f;
		}
	}

	//プレイヤーと敵弾との衝突判定
	for (const auto& bullet : enemyBullets_) {
		Sphere enemyBullet = {bullet->GetWorldPos(), redius};
		if (IsCollision(player, enemyBullet)) {
			bullet->OnCollision();
			player_->OnCollision();
		}
	}

	//自弾と敵弾との衝突判定
	for (const auto& bulletA : playerBullets_) {
		Sphere playerBullet = {bulletA->GetWorldPos(), redius};
		for (const auto& bulletB : enemyBullets_) {
			Sphere enemyBullet = {bulletB->GetWorldPos(), redius};
			if (IsCollision(playerBullet, enemyBullet)) {
				bulletA->OnCollision();
				//bulletB->OnCollision();
			}
		}
	}
	//プレイヤーと敵との衝突判定
	if (IsCollision(player, enemy)) {
		player_->OnCollision();
	}

}

void GameScene::ToClearScene() { scene_ = Scene::Clear; }

void GameScene::ToGameOverScene() { scene_ = Scene::GameOver; }