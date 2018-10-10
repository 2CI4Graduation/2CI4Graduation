//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ev_Image.h"

namespace  Ev_Image
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
		this->render2D_Priority[1] = 0.2f;
		this->imageName = "";
		this->pos = ML::Vec2(0, 0);
		this->drawBase = ML::Box2D(0, 0, 0, 0);
		this->src = ML::Box2D(0, 0, 0, 0);
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase;
		draw.Offset(this->pos);
		DG::Image_Draw(this->imageName, draw, this->src);
	}
	//----------------------------------------------------------

	//タスクを生成するか、どうめいのタスクの情報を更新する
	void Object::CreateOrReset(stringstream& ss_)
	{
		//パラメータを分解する
		string taskName;
		ss_ >> taskName;
		//新規作成か更新かの判別
		auto p = ge->GetTask_One_GN<Object>("イベント画像", taskName);

		//新規作成の場合
		if (nullptr == p)
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

	//表示するメッセージを設定する
	void Object::Set(const string& taskName_, stringstream& ss_)
	{
		//パラメータを分解する
		string filePath;
		ss_ >> filePath;
		//画像表示解除にたいおうする
		if (filePath=="off")
		{
			this->Kill();
			return;
		}
		//タスク名を設定
		this->name = taskName_;
		//イメージ名を設定
		this->imageName = this->groupName + this->name + "Img";
		//画像読み込み
		DG::Image_Create(this->imageName, filePath);
		
		//表示位置・向きを読み込み
		enum XPosition
		{
			Left,Center,Right
		};
		XPosition xPos = Left;//表示位置
		bool xRevers = false;//画像反転
		bool yRevers = false;//画像反転
		string posAndRev;
		ss_>> posAndRev;
		//寄せ・反転の指定
		if (string::npos!=posAndRev.find("L"))
		{
			xPos = Left;
		}
		if (string::npos != posAndRev.find("R"))
		{
			xPos = Right;
		}
		if (string::npos != posAndRev.find("C"))
		{
			xPos = Center;
		}
		if (string::npos != posAndRev.find("X"))
		{
			xRevers = true;
		}
		else
		{
			xRevers =false;
		}
		if (string::npos != posAndRev.find("Y"))
		{
			yRevers = true;
		}
		else
		{
			yRevers = false;
		}

		//画像のサイズからいちを設定
		POINT s = DG::Image_Size(this->imageName);
		this->drawBase = ML::Box2D(0, 0, s.x, s.y);
		this->src = ML::Box2D(0, 0, s.x, s.y);

		//寄せ
		if (Left==xPos)
		{
			this->pos.x = 0;
		}
		else if(Right==xPos)
		{
			this->pos.x = (float)ge->screen2DWidth - s.x;

		}
		else
		{
			this->pos.x = (ge->screen2DWidth - s.x)/2.0f;
		}
		//反転
		if (true==xRevers)
		{
			this->src.x = s.x;
			this->src.w = -s.x;
		}
		if (true == yRevers)
		{
			this->src.y = s.y;
			this->src.h= -s.y;
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