#pragma once
#include "Vec3.h"
#include <variant>
#include <map>
#include <string>

class GlobalVariables {
private:

	//項目
	using Item = std::variant<int32_t,float,Vec3>;
	//グループ
	using Group = std::map<std::string,Item>;

	//全データ
	std::map<std::string, Group> datas_;

	
	const std::string kDirectoryPath = "Resources/GlobalVariables/";

public:

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);
	/// <summary>
	/// ディレクトリの全ファイルの読み込み
	/// </summary>
	void LoadFiles();
	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName">グループ</param>
	void LoadFile(const std::string& groupName);

	//値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//値のセット(Vec3)
	void SetValue(const std::string& groupName, const std::string& key, const Vec3& value);

	//項目の追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	//項目の追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	//項目の追加(Vec3)
	void AddItem(const std::string& groupName, const std::string& key, const Vec3& value);

	//値の取得
	int32_t GetIntValue(const std::string& groupName, const std::string& key) const;
	float GetFloatValue(const std::string& groupName, const std::string& key) const;
	Vec3 GetVec3Value(const std::string& groupName, const std::string& key) const;

public:

	static GlobalVariables* GetInstance(); 

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	GlobalVariables& operator=(const GlobalVariables&) = delete;

};
