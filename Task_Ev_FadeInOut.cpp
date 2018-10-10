//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ev_FadeInOut.h"

namespace  Ev_FadeInOut
{
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
		this->render2D_Priority[1] = 0.0005f;
		this->Cnt = 0;
		this->imageName = "";
		this->Stop();
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		if (this->imageName!="")
		{
			DG::Image_Erase(this->imageName);
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		if (this->mode==In)
		{
			this->Cnt--;
			if (this->Cnt<0)
			{
				//イベントエンジン再開
				ge->StopAll_GN("イベント", "実行エンジン",false);
				//消滅させる
				this->Kill();
			}
		}

		if (this->mode==Out)
		{
			this->Cnt++;
			if (this->Cnt > 60)
			{
				//イベントエンジン再開
				ge->StopAll_GN("イベント", "実行エンジン", false);
				//停止状態にする
				this->Stop();
			}
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		DG::Image_Draw(this->imageName, draw, this->src, ML::Color(this->Cnt / 60.0f, 1, 1, 1));
	}
	//タスクを生成する（フェードイン）か、フェードアウトする
	 void Object::CreateOrFadeOut(stringstream& ss_)
	{
		//新規作成か更新の判断
		auto p = ge->GetTask_One_GN<Object>(defGroupName, defName);

		//新規作成
		if (nullptr==p)
		{
			p = Object::Create(true);
			p->Set(ss_);
		}
		//更新
		else
		{
			p->Set(ss_);
		}
	}

	//フェードインの準備を行う
	void Object::Set(stringstream& ss_)
	{
		//パラメータを分解
		string filePath;
		ss_ >> filePath;

		//フェードインする場合
		if (filePath=="in")
		{
			this->mode = In;
			this->Cnt=60;
		}
		//フェードアウト
		else
		{
			this->mode = Out;
			this->Cnt = 0;
			//イメージ名を生成
			this->imageName = this->groupName + this->name + "Img";
			//画像読み込み
			DG::Image_Create(this->imageName, filePath);
			POINT s = DG::Image_Size(this->imageName);
			this->src = ML::Box2D(0, 0, s.x, s.y);

		}

		//イベントエンジンを一時停止させる
		ge->StopAll_GN("イベント", "実行エンジン");
		//停止状態を解除する
		this->Stop(false);
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