//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/HitTest.bmp");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
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
		this->render2D_Priority[1] = 0.5f;			//描画順
		this->angle_LR = Right;						//左右
		this->motion = Stand;						//状態管理
		this->controllerName = "P1";				//コントローラ宣言
		this->hitFlag = true;						//
		this->pow_jump = -7.5f;						//ジャンプ初速
		this->speed_fall = 0.5f;					//落下速度加算量
		this->max_FallSpeed = 1.5f;					//落下速度上限
		this->hitBase = ML::Box2D(32, 64, 64, 128);	//判定矩形
		this->neutral_height = 128;					//通常時の判定矩形高さ
		this->squat_height = 64;

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
		this->moveCnt++;
		this->animCnt++;
		auto  in = DI::GPad_GetState(this->controllerName);
		ML::Vec2  est(0, 0);

		this->CheckMove(est);
		//足元接触判定
		this->hitFlag = this->CheckFoot();
		this->pos += this->moveVec;
		this->Think();
		this->Move();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D  src(0, 0, 100, 100);

		//	接触の有無に合わせて画像を変更
		if (true == this->hitFlag) {
			src.Offset(100, 0);
		}
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//接触時の応答処理
	//引数	：	（攻撃側,攻撃情報,与える無敵時間）
	void Object::Recieved(const BChara* from_, AttackInfo at_, const int& unHit_)
	{
		if (this->unHitCnt > 0)
		{
			return;//無敵時間
		}
		//無敵時間を受け取る
		this->unHitCnt = unHit_;
		//ダメージ状態にする
		this->UpDateMotion(Damage);
	}
	//思考
	void Object::Think()
	{
		//現在の状態を保存
		Motion nm = this->motion;
		//コントローラーの宣言
		auto in = DI::GPad_GetState(this->controllerName);
		switch (this->motion)
		{
		default:
			break;
		case Stand:
			if (in.B1.down) { nm = Takeoff; }
			if (in.LStick.D.on) { nm = Squat; }
			break;
		case Squat:
			if (in.LStick.D.off) { nm = Stand; }
			break;
		case Gu:
			break;
		case Choki:
			break;
		case Paa:
			break;
		case Takeoff:
			if (this->moveCnt >= 8) { nm = Jump; }
			break;
		case Jump:
			if (this->moveVec.y >= 0) { nm = Fall; }
			break;
		case Fall:
			if (this->moveVec.y==0.0f) { nm = Landing; }
			break;
		case Landing:
			if (this->moveCnt >= 8) { nm = Stand; }
			break;
		case Damage:
			if (this->unHitCnt <= 0) { nm = Stand; }
			break;
		}
		this->UpDateMotion(nm);
	}
	void Object::Move()
	{
		//重力加速
			//仮に画面下端で止まる
			//重力
		if (this->pos.y <= 720 - this->hitBase.h / 2)
		{
			this->moveVec.y += this->speed_fall;
		}
		else
		{
			this->moveVec.y = 0.0f;
		}
		//コントローラの宣言
		auto in = DI::GPad_GetState(this->controllerName);
		//モーション毎の処理
		switch (this->motion)
		{
		default:
			break;
		case Stand:
			this->pos.y = float(720 - this->hitBase.h / 2);
			this->hitBase.h = this->neutral_height;
			break;
		case Squat:
			this->pos.y = 720;
			this->hitBase.h = this->squat_height;
			break;
		case Gu:
			break;
		case Choki:
			break;
		case Paa:
			break;
		case Takeoff:
			break;
		case Jump:
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->pow_jump;
			}
			break;
		case Fall:
			break;
		case Landing:
			break;
		case Damage:
			break;
		}
		//移動量を反映する
		this->pos += this->moveVec;
	}
	//アニメーション制御
	Object::DrawInfo Object::Anim()
	{
		//デフォルトの値を用意
		ML::Color dc(1, 1, 1, 1);
		Object::DrawInfo imageTable[] = 
		{
			{ this->hitBase,ML::Box2D(0,0,0,0),dc }
		};
		Object::DrawInfo rtv;
		switch (this->motion)
		{
		default:
			break;
		case Stand:
			break;
		case Gu:
			break;
		case Choki:
			break;
		case Paa:
			break;
		case Takeoff:
			break;
		case Jump:
			break;
		case Fall:
			break;
		case Landing:
			break;
		case Damage:
			break;
		}
		return rtv;
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