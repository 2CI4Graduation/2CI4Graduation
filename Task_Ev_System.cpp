//----------------------------------------------------------------------------------------------------------------------------
//イベント実行エンジンのシステム制御系メソッド
//-------------------------------------------------------------------------------------------------------------------
#include "MyPG.h"
#include "Task_EventEngine.h"
#include"BChara.h"
#include"AppearObject.h"
#include"Task_Map2D.h"
namespace EventEngine
{//イベントフラグのデータ書き換え

	bool Object::EventFlag(stringstream& ss_)
	{
		string flagname;
		string sign;
		float value;
		ss_ >> flagname >> sign >> value;
		if (sign=="=")
		{
			ge->evFlags[flagname] = value;
		}
		else if(sign=="+")
		{
			ge->evFlags[flagname] += value;
		}
		else if (sign == "-")
		{
			ge->evFlags[flagname] -= value;
		}
		else
		{
			return false;
		}
		return true;
	}
	bool Object::If(stringstream& ss_)
	{
		//パラメータから対象を得る
		string flagKind;
		ss_ >> flagKind;
		bool flag = false;
		//int skip=0;
		string labelName;
		if (flagKind == "ev_flag")
		{
			string flagName;
			string sign;
			float value;
			ss_ >> flagName >> sign >> value >> labelName;
			//比較
			if (sign=="==")
			{
				flag = ge->evFlags[flagName] == value;
			}
			else	if (sign == ">")
			{
				flag = ge->evFlags[flagName] > value;
			}
			else if (sign == "<")
			{
				flag = ge->evFlags[flagName] <value;
			}
			else
			{
				return false;
			}
		}
		//キャラクターのパラメータを対象とする場合
		else if(flagKind=="charaParam")
		{
			//未実装
		}
		//環境変数を対象とする場合
		else if(flagKind=="System")
		{
			//未実装
		}

		//目的のラベルまで進む
		if (true==flag)
		{
			//読み込み位置をファイルの先頭へ戻す
			this->evFile.seekg(0, ios_base::beg);
			//1行読み込みを繰り返す
			string lt;
			while (this->ReadLine(lt))
			{
				//行を解読
				string::size_type t = lt.find(">");
				string headerStr = lt.substr(0, t);
				string dataStr = lt.substr(t + 1);
				if ("label"==headerStr)
				{
					//読み込んだ行がラベル
					stringstream ss;
					ss << dataStr;
					string labelName2;
					ss >> labelName2;
					if (labelName==labelName2)
					{//ラベルネームが一致
						break;

					}
				}
			}
			


		}
		return true;
	}
	//画像の読み込み
	bool Object::Image(stringstream& ss_)
	{
		string OffScreenName;
		string filePath;
		ss_ >> OffScreenName >> filePath;
		DG::Image_Create(OffScreenName, filePath);
		return true;
	}
	//イベントファイルの再選択
	bool Object::FileSet(stringstream& ss_)
	{
		string filePath;
		ss_ >> filePath;
		return this->Set(filePath);
	}
	//オブジェクト（キャラクタ）生成
	bool Object::AppearObject(stringstream& ss_)
	{
		//パラメータから対象を得る
		string kind;
		float posX, posY;
		ss_ >> kind >> posX >> posY;
		//オブジェクト生成（エンジンに登録）
		BChara::SP obj = AppearObject_BChara(kind);
		if (obj)
		{
			obj->pos.x = posX;
			obj->pos.y = posY;
			obj->Stop();//停止状態にしておく

		}
		//任意パラメータへの対応
		while (false==ss_.eof())
		{
			string paramName, eq;
			ss_ >> paramName >> eq;
			//任意パラメータでないなら
			if (eq != "=")
			{
				break;
			}
			if (paramName=="name")
			{
				ss_ >> obj->name;
			}
			/*if (paramName == "jumpPow")
			{
				ss_ >> obj->jumpPow;
			}
			if (paramName == "maxFallSpeed")
			{
				ss_ >> obj->maxFallSpeed;
			}
			if (paramName == "maxSpeed")
			{
				ss_ >> obj->maxSpeed;
			}*/
			if (paramName == "eventFileName")
			{
				ss_ >> obj->eventFileName;

			}
			//他のパラメータも必要に応じて増やしていく
		}
		return nullptr != obj;
	}
	//マップ再読み込み
	bool Object::MapLoad(stringstream& ss_)
	{
		auto map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
		if (nullptr==map)
		{
			map = Map2D::Object::Create(true);
		}
		//マップ読み込み
		string mapFilePath;
		ss_ >> mapFilePath;
		return map->Load(mapFilePath);
	}
	//キャラ情報の変更
	bool Object::ModifyChara(stringstream& ss_)
	{
		//パラメータから対象を得る
		string gname, name;
		ss_ >> gname >> name;
		//オブジェクトを検出
		if (auto mc=ge->GetTask_One_GN<BChara>(gname,name))
		{
			while (false == ss_.eof())
			{
				string paramName, eq;
				ss_ >> paramName >> eq;
				//=に対する処理
				if (eq == "=")
				{


					if (paramName == "posX")
					{
						ss_ >> mc->pos.x;
					}
					if (paramName == "posY")
					{
						ss_ >> mc->pos.y;
					}
					/*if (paramName == "jumpPow")
					{
						ss_ >> mc->jumpPow;
					}
					if (paramName == "maxFallSpeed")
					{
						ss_ >> mc->maxFallSpeed;
					}
					if (paramName == "maxSpeed")
					{
						ss_ >> mc->maxSpeed;
					}*/
					if (paramName == "event")
					{
						ss_ >> mc->eventFileName;
					}
					//他のパラメータも必要に応じて追加
				}
			}
			return true;
		}
		return false;
	}

	//オブジェクト（キャラクタ）の削除
	bool Object::KillObject(stringstream& ss_)
	{
		//パラメータから対象を得る
		string gname;
		ss_ >> gname;
		//タスク名を含む場合
		if (false==ss_.eof())
		{
			string name;
			ss_ >> name;
			ge->KillAll_GN(gname, name);
		}
		//グループ名のみの場合
		else 
		{
			ge->KillAll_G(gname);
		}
		return true;
	}
}