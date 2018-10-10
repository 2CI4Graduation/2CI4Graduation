//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ev_Message.h"
#include  "Task_EventEngine.h"

namespace  Ev_message
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		{
			struct BGImageParam
			{
				string name;
				string path;
			};
			BGImageParam w[10] = {
				{"BMG 標準","./data/image/MessageF.png"},
			{ "BMG 怒り","./data/image/MessageF.png" },//以下仮
			{ "BMG 困惑","./data/image/MessageF.png" },
			{ "BMG 予約A","./data/image/MessageF.png" },
			{ "BMG 予約B","./data/image/MessageF.png" },
			{ "BMG 予約C","./data/image/MessageF.png" },
			{ "BMG 予約D","./data/image/MessageF.png" },
			{ "BMG 予約E","./data/image/MessageF.png" },
			{ "BMG 予約F","./data/image/MessageF.png" },
			{ "BMG 予約G","./data/image/MessageF.png" },
			};
			for (int i = 0; i <_countof(this->imageNames); ++i)
			{
				this->imageNames[i] = w[i].name;
				DG::Image_Create(w[i].name, w[i].path);
			}
		}
		{
			struct FontParam
			{
				string name;
				string fontName;
				int width;
				int height;
			};
			FontParam w[3] =
			{
				{"MFont 小","HG丸ｺﾞｼｯｸM-PRO",8,16},
			{ "MFont 中","HG丸ｺﾞｼｯｸM-PRO",12,24 },
			{ "MFont 大","HG丸ｺﾞｼｯｸM-PRO",16,32 },
			};
			for (int i = 0;i<_countof(this->fontNames); ++i)
			{
				this->fontNames[i] = w[i].name;
				DG::Font_Create(w[i].name, w[i].fontName, w[i].width, w[i].height);
			}
		}







		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		for (int i = 0; i < _countof(this->imageNames); ++i)
		{
			DG::Image_Erase(this->imageNames[i]);
		}

		for (int i = 0; i < _countof(this->fontNames); ++i)
		{
			DG::Font_Erase(this->fontNames[i]);
		}
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
		this->Stop();//自身を停止状態にしておく
		this->render2D_Priority[1] = 0.1f;
		this->pos.x = 0;
		this->pos.y = 270 - 128;
		this->msgText = "";
		this->bgNumber = 0;
		this->fontNumber = 1;

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//制限時間経過もしくはボタンが押されたとき
		if ((this->timeLimit != 0 && this->timeCnt >= this->timeLimit) || in.B2.down)
		{
			//自身は停止状態にする
			this->Stop();
			//エンジンの停止状態を解除する
			ge->StopAll_GN("イベント", "実行エンジン",false);
		}
		else
		{
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 480, 128);
		ML::Box2D src(0, 0, 240, 64);
		draw.Offset(this->pos);
		DG::Image_Draw(this->res->imageNames[this->bgNumber], draw, src, ML::Color(0.9f, 1, 1, 1));
		ML::Box2D drawF(6, 6, 480-12, 128-12);
		drawF.Offset(this->pos);
		DG::Font_Draw(this->res->fontNames[this->fontNumber], drawF,this->msgText);
	}
	//-------------------------------------------------------------------------------
	//表示するメッセージを設定する
	void Object::Set(const string& taskName_, stringstream& ss_)
	{
		//パラメータを分解する
		int timeL;
		string msg;
		ss_ >> timeL >> msg;

		//タスク名を設定
		this->name = taskName_;
		//メッセージ枠消去要求へ対応する
		if (msg == "off")
		{
			this->Kill();
			return;
		}
		this->timeCnt = 0;
		this->timeLimit = timeL;
		this->msgText = msg;
		//*を改行に置き換える
		string::size_type ast;
		while ((ast = msgText.find("*")) != string::npos)
		{
			msgText.at(ast) ='\n';
		}
		//イベントエンジンを一時停止させる
		ge->StopAll_GN("イベント", "実行エンジン");
		//自身の停止状態を解除する
		this->Stop(false);
	}

	//タスクを生成するか、同名のタスクの情報を更新する
	 void Object::CreateOrReset(stringstream& ss_)
	{
		//パラメータを分解する
		string taskName;
		ss_ >> taskName;
		//新規作成か更新かの判別
		auto p = ge->GetTask_One_GN<Object>("メッセージ表示枠", taskName);

		//新規作成の場合
		if (nullptr==p)
		{
			p = Object::Create(true);
			p->Set(taskName, ss_);
			

		}
		//更新の場合
		else
		{
			p->Set(taskName, ss_);
		}
			 

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