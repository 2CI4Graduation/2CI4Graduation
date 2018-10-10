//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_EventEngine.h"
#include  "Task_Ev_Message.h"
#include  "Task_Ev_Image.h"
#include  "Task_Ev_FadeInOut.h"
#include"AppearObject.h"

namespace  EventEngine
{
	Object::WP  Object::instance;//多重生成防止
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		
		//★タスクの生成

		//イベント実行中に動くと困るものは全て停止させる
		//ge->StopAll_G("プレイヤ");
	

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (this->evFile.is_open())
		{
			this->evFile.close();//ファイルを閉じる
		}
		//停止させていたタスクをもとに戻す
		//ge->StopAll_G("プレイヤ",false);
		

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (!this->evFile)
		{
			this->Kill();
		}
		//新しい行を読み込む
		string lineText;
		string headerText;
		string dataText;
		while (this->ReadLine(lineText))
		{
			//行を解析
			string::size_type t = lineText.find(">");
			headerText = lineText.substr(0, t);
			dataText = lineText.substr(t + 1);
			//ヘッダ名に対応したイベント処理を行う
			bool next = this->Execute(headerText, dataText);
			//イベント処理失敗もしくは自身の状態がアクティブでなくなった場合ループをぬける
			if (false==next||BTask::eActive!=this->CheckState())
			{
				break;
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//イベント実行エンジンを生成します。
	//既にイベント実行エンジンが存在している場合、生成は失敗します。
	Object::SP Object::Create_Mutex()
	{
		//新規作成か更新かの判別
		if (auto p = instance.lock())
		{
			return false;//既に存在する
		}
		else//新規作成
		{
			p = Object::Create(true);
			instance = p;
			return p;
		}
	}
	//イベントファイル名を登録＆イベント開始
	bool Object::Set(const string& fPath_)
	{
		//今処理中のファイルを閉じる
		if (this->evFile.is_open())
		{
			this->evFile.close();
		}
		//イベントファイル読み込み
		this->evFile.open(fPath_);
		//ファイルが開けない　（失敗
		if (!this->evFile)
		{
			return false;
		}
		return true;
	}

	//１行分のデータを読み込む
	bool Object::ReadLine(string& lineT_)
	{
		//一行読み込む
		bool rtv = false;
		while (getline(this->evFile,lineT_))
		{
			//整合性チェック

			//行内に空白（全角）以外の文字がない
			if (string::npos==lineT_.find_first_not_of("  "))
			{
				continue;
			}
			//最初の文字が/の場合、コメントとみなす
			if ('/' == lineT_.at(0))
			{
				continue;
			}
			//ヘッダ名とデータの境目である＞が含まれていない
			if (string::npos == lineT_.find(">"))
			{
				continue;
			}
			
			rtv = true;//有効な行とみなす
			break;

		}
		return rtv;
	}

	//行データに対する処理を行う
	bool Object::Execute(string& hs_, string& ds_)
	{

		//トークン削除（こまかい文法チェックはしていない）
		string::size_type t;
		while ((t=ds_.find_first_of("(,);")) != string::npos)
		{
			ds_[t] = ' ';

		}
		//
		stringstream ss;
		ss << ds_;
		//イベント終了
		if (hs_=="end")
		{
			this->Kill();
		}
		//メッセージ表示
		else if(hs_=="msg")
		{
			Ev_message::Object::CreateOrReset(ss);
		}
		//画像読み込み＆表示
		else if(hs_=="evimg")
		{
			Ev_Image::Object::CreateOrReset(ss);
		}
		//条件分岐
		else if (hs_=="if")
		{
			this->If(ss);
		}
		//イベントフラグデータ変更
		else if(hs_=="flag")
		{
			this->EventFlag(ss);
		}
		else if (hs_=="fade_io")
		{
			Ev_FadeInOut::Object::CreateOrFadeOut(ss);
		}
		//画像の読み込み＆入れ替え
		else if (hs_=="img")
		{
			this->Image(ss);
		}
		//イベントファイルの再選択
		else if (hs_ == "fileset")
		{
			this->FileSet(ss);
		}
		//キャラクタ（オブジェクト）削除
		else if (hs_ == "kill")
		{
			this->KillObject(ss);
		}
		//キャラクタ（オブジェクト）出現
		else if (hs_ == "appear")
		{
			this->AppearObject(ss);
		}
		
		//キャラクタパラメータ変更
		else if (hs_ == "chara")
		{
			this->ModifyChara(ss);
		}
		//マップ再読み込み
		else if (hs_ == "map")
		{
			this->MapLoad(ss);
		}
		
		//ラベル
		else if (hs_=="label")
		{
			
			
		}
		else
		{
			return false;
		}
		return true;
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}