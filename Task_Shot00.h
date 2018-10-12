#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//プレイヤの出す弾
//-------------------------------------------------------------------
#include  "BChara.h"

namespace  Shot00
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName(		"弾（プレイヤ）");	//グループ名
	const  string  defName(				"NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		string imageName; 
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		int moveCnt;			//行動カウンタ
		int animCnt;			//アニメーションカウンタ
		int unhitCnt;			//無敵時間
		float speed;			//速度
		float pow_Jump;			//ジャンプ初速
		float max_FallSpeed;	//最大効果速度
		float gravity;			//重力加速度
		float gauge_Extra;		//EXゲージ量
		float add_Extra;		//EXゲージ増加量
		ML::Vec2 pos;			//座標
		ML::Vec2 moveVec;		//移動量
		ML::Box2D hitBase;		//判定矩形
		Motion motion;			//状態管理
		string controllerName;	//コントローラー宣言

								//構造体

								//攻撃情報
								//(攻撃力,命中精度,属性)
		struct AttackInfo
		{
			int power;	//攻撃力
			int element;//属性
		};
		//アニメーション情報構造体
		struct DrawInfo
		{
			ML::Box2D	draw, src;
			ML::Color	color;
		};
	public:
		//めり込まない移動処理
		//引数	：	（移動量）
		void  CheckMove(const ML::Vec2&  est_);
		//足元接触判定
		bool  CheckFoot();
		//思考
		void Think();
		//行動
		void Move();
		//モーションを更新（変更なしの場合	false)
		bool  UpDateMotion(const Motion  nm_);
		//接触時の応答処理
		//引数	：	（攻撃側,攻撃情報構造体,無敵時間）
		void Recieved(const BChara* from_, const AttackInfo at_, const int& unhitTime_);
		//アニメーション制御
		DrawInfo Anim();
		
	};
}