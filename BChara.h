#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇

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
	//状態管理
	enum Motion
	{
		Stand,	//立ち
		Squat,	//しゃがみ
		Gu,		//グー
		Choki,	//チョキ
		Paa,	//パー
		Takeoff,//離陸
		Jump,	//ジャンプ中
		Fall,	//落下中
		Landing,//着地
		Damage	//ダメージ
	};

	//キャラクタ共通メンバ変数
	Motion motion;			//状態管理
	ML::Vec2    pos;		//キャラクタ位置
	ML::Box2D   hitBase;	//あたり判定範囲
	ML::Vec2	moveVec;	//移動ベクトル
	int			moveCnt;	//行動カウンタ
	int			animCnt;	//アニメーションカウンタ
	int			unHitCnt;	//無敵時間カウンタ
	//左右の向き（2D横視点ゲーム専用）
	enum Angle_LR { Left, Right };
	Angle_LR	angle_LR;
	WP			target;
	string eventFileName;

	//メンバ変数に最低限の初期化を行う
	//★★メンバ変数を追加したら必ず初期化も追加する事★★
	BChara()
		: pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		, unHitCnt(0)
		, angle_LR(Right)
	{
    }
	virtual  ~BChara(){}

	//キャラクタ共通メソッド
	//めり込まない移動処理
	virtual  void  CheckMove(ML::Vec2&  est_);
	//足元接触判定
	virtual  bool  CheckFoot();

	//思考
	virtual void Think()
	{
		ML::MsgBox("Recieved 実装されていません");
	}
	//行動
	virtual void Move()
	{
		ML::MsgBox("Recieved 実装されていません");
	}
	//モーションを更新（変更なしの場合	false)
	bool  UpDateMotion(const Motion  nm_);
	//接触時の応答処理
	//引数	：	（攻撃側,攻撃情報構造体,無敵時間）
	virtual void Recieved(const BChara* from_, const AttackInfo at_, const int& unhitTime_)
	{
		ML::MsgBox("Recieved 実装されていません");
	}
};
