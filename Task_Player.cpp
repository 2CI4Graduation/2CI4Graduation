//-------------------------------------------------------------------
//プレイヤ
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "playerImg";
		DG::Image_Create(this->imageName, "./data/image/Image_Load.png");
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
		this->speed = 0.0f;															//速度
		this->gauge_Extra = 0.0f;													//EXゲージ量
		this->add_Extra=0.0f;														//EXゲージ増加量
		this->pos = ML::Vec2(float(ge->screenWidth/2), float(ge->screenHeight/2));	//座標
		this->moveVec=ML::Vec2(0.0f,0.0f);											//移動量
		this->gravity = ML::Gravity(32);											//重力加速度
		this->hitBase=ML::Box2D(-46,-46,92,92);										//判定矩形
		this->controllerName="P1";													//コントローラー宣言

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
		this->Think();
		this->Move();
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 92, 92);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//-------------------------------------------------------------------
	//モーションを更新（変更なしの場合	false)
	bool  Object::UpDateMotion(const Motion  nm_)
	{
		if (nm_ == this->motion)
		{
			return false;
		}
		else
		{
			this->motion = nm_;
			this->moveCnt = 0;
			this->animCnt = 0;
			return true;
		}
	}
	//接触時の応答処理
	//引数	：	（攻撃側,攻撃情報構造体,無敵時間）
	void Object::Recieved(const BChara* from_, const AttackInfo at_, const int& unhitTime_)
	{
		//無敵時間があれば攻撃を受けない
		if (this->unhitCnt > 0)
		{
			return;
		}
		//外部から無敵時間を指定
		this->unhitCnt = unhitTime_;
		//やられ状態にする
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
			if (this->CheckFoot()) { nm = Landing; }
			break;
		case Landing:
			if (this->moveCnt >= 8) { nm = Stand; }
			break;
		case Damage:
			break;
		}
		this->UpDateMotion(nm);
	}
	void Object::Move()
	{
		//重力加速
		switch (this->motion) {
		default:
			//上昇中もしくは足元に地面が無い
			if (this->moveVec.y < 0 ||
				this->CheckFoot() == false) {
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_FallSpeed);
			}
			//地面に接触している
			else {
				this->moveVec.y = 0.0f;
			}
			break;
			//以下、重力に影響されない状態

		}
		//コントローラの宣言
		auto in = DI::GPad_GetState(this->controllerName);
		//モーション毎の処理
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
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->pow_Jump;
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
		Object::DrawInfo imageTable[] = {
			{this->hitBase,ML::Box2D(0,0,0,0),dc}
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
	//めり込まない移動処理
	//void Object::CheckMove(ML::Vec2&  e_)
	//{
	//	//マップが存在するか調べてからアクセス
	//	auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	//	if (nullptr == map) { return; }//マップが無ければ判定しない(出来ない）

	//								   //横軸に対する移動
	//	while (e_.x != 0) {
	//		float  preX = this->pos.x;
	//		if (e_.x >= 1) { this->pos.x += 1;		e_.x -= 1; }
	//		else if (e_.x <= -1) { this->pos.x -= 1;		e_.x += 1; }
	//		else { this->pos.x += e_.x;		e_.x = 0; }
	//		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
	//		if (true == map->CheckHit(hit)) {
	//			this->pos.x = preX;		//移動をキャンセル
	//			break;
	//		}
	//	}
	//	//縦軸に対する移動
	//	while (e_.y != 0) {
	//		float  preY = this->pos.y;
	//		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
	//		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
	//		else { this->pos.y += e_.y;		e_.y = 0; }
	//		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
	//		if (true == map->CheckHit(hit)) {
	//			this->pos.y = preY;		//移動をキャンセル
	//			break;
	//		}
	//	}
	//}
	//-----------------------------------------------------------------------------
	//足元接触判定
	//bool  Object::CheckFoot()
	//{
	//	//あたり判定を基にして足元矩形を生成
	//	ML::Box2D  foot(this->hitBase.x,
	//		this->hitBase.y + this->hitBase.h,
	//		this->hitBase.w,
	//		1);
	//	foot.Offset(this->pos);

	//	auto   map = ge->GetTask_One_GN<Map2D::Object>("フィールド", "マップ");
	//	if (nullptr == map) { return false; }//マップが無ければ判定しない(出来ない）
	//	return map->CheckHit(foot);
	//}
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