

#include "GlobalVariables.h"
#ifdef _DEBUG
#include "Imgui.h"
#endif // _DEBUG
#include <json.hpp>
#include <assert.h>
#include <fstream>
#include <Windows.h>

using namespace nlohmann;

GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;

	return &instance;
}

void GlobalVariables::CreateGroup(const std::string& groupName) {
	//指定のオブジェクトがなければ追加する
	datas_[groupName];
}

void GlobalVariables::Update() {

#ifdef _DEBUG
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) {
		return;
	}

	// 各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
	     ++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}

		// 各項目について
		for (std::map<std::string, Item>::iterator itItem = group.begin(); itItem != group.end();
		     ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;
			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item)) {
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 1000);
				// float型の値を保持していれば
			} else if (std::holds_alternative<float>(item)) {
				float* ptr = std::get_if<float>(&item);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0.0f, 1000.0f);
				// Vec3型の値を保持していれば
			} else if (std::holds_alternative<Vec3>(item)) {
				Vec3* ptr = std::get_if<Vec3>(&item);
				ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
			}
		}

		// 改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
#endif // _DEBUG


	
}

void GlobalVariables::SaveFile(const std::string& groupName) {

	//グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	//未登録チェック
	assert(itGroup != datas_.end());

	json root;

	root = json::object();
	//josnオブジェクトの登録
	root[groupName] = json::object();
	//各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();itItem != itGroup->second.end(); ++itItem) {
		//項目名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;
		//int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item)) {
			//int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item);
		  //float型の値を保持していれば
		} else if (std::holds_alternative<float>(item)) {
			//float型の値を登録
			root[groupName][itemName] = std::get<float>(item);
		  //Vec3型の値を保持していれば
		} else if (std::holds_alternative<Vec3>(item)) {
			//float型のjson配列登録
			Vec3 value = std::get<Vec3>(item);
			root[groupName][itemName] = json::array({value.x, value.y, value.z});
		}
	}

	//ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directories(kDirectoryPath);
	}
	//書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//書き込み用のファイルストリーム
	std::ofstream ofs;
	//ファイルを書き込み用に開く
	ofs.open(filePath);

	//ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}
	//ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	//ファイルを閉じる
	ofs.close();
}

void GlobalVariables::LoadFiles() {

	std::string DirectoryPath = "Resources/GlobalVariables/";
	//
	if (!std::filesystem::exists(DirectoryPath)) { return; }

	std::filesystem::directory_iterator dir_it(DirectoryPath);
	for (const std::filesystem::directory_entry& enter : dir_it) {
		//ファイルパスを取得
		const std::filesystem::path& filePath = enter.path();

		//ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		//.jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) { continue; }

		//ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

void GlobalVariables::LoadFile(const std::string& groupName) {

	//読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);

	// ファイルオープン失敗？
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	json root;
	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();

	//グループを検索
	json::iterator itGroup = root.find(groupName);
	//未登録チェック
	assert(itGroup != root.end());

	//各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		//アイテム名を取得
		const std::string& itemName = itItem.key();

		//int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			//int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		  //float型の値を保持していれば
		} else if (itItem->is_number_float()) {
			//float型の値を登録
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));
		  //要素数3の配列であれば
		} else if (itItem->is_array() && itItem->size() == 3) {
			//float型のjson配列登録
			Vec3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			SetValue(groupName, itemName, value);
		}
	}
}


void GlobalVariables::SetValue(const std::string& groupName, const std::string& key,int32_t value) {
	//groupの参照を取得
	Group& group = datas_[groupName];
	//新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	//設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// groupの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vec3& value) {
	// groupの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

	auto group = datas_.find(groupName);

	if (group == datas_.end()) { return; }

	if (group->second.find(key) == group->second.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {

	auto group = datas_.find(groupName);

	if (group == datas_.end()) { return; }

	if (group->second.find(key) == group->second.end()) {
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vec3& value) {

	auto group = datas_.find(groupName);

	if (group == datas_.end()) { return; }

	if (group->second.find(key) == group->second.end()) {
		SetValue(groupName, key, value);
	}
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) const {
	assert(datas_.find(groupName) != datas_.end());

	const Group& group = datas_.at(groupName);

	assert(group.find(key) != group.end());

	return std::get<int>(group.find(key)->second);
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) const {
	assert(datas_.find(groupName) != datas_.end());

	const Group& group = datas_.at(groupName);

	assert(group.find(key) != group.end());

	return std::get<float>(group.find(key)->second);
}

Vec3 GlobalVariables::GetVec3Value(const std::string& groupName, const std::string& key) const {
	assert(datas_.find(groupName) != datas_.end());

	const Group& group = datas_.at(groupName);

	assert(group.find(key) != group.end());

	return std::get<Vec3>(group.find(key)->second);
}