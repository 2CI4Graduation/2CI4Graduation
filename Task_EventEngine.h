#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//イベント実行エンジン
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  EventEngine
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName(		"イベント");	//グループ名
	const  string  defName(				"実行エンジン");	//タスク名
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
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
								//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		static WP instance;//多重生成防止
		ifstream evFile;//イベントファイル

		//イベント実行エンジンを生成します。
		//既にイベント実行エンジンが存在している場合、生成は失敗します。
		static Object::SP Object::Create_Mutex();
		//イベントファイル名を登録＆イベント開始
		bool Set(const string& fPath_);

		//１行分のデータを読み込む
		bool ReadLine(string& lineT_);

		//行データに対する処理を行う
		bool Execute(string& hs_, string& ds_);

		//☆システム制御系
		//イベントフラグのデータの書き換え
		bool EventFlag(stringstream& ss_);
		//条件分岐
		bool If(stringstream& ss_);
		//画像の読み込み
		bool Object::Image(stringstream& ss_);
		//イベントファイルの再選択
		bool FileSet(stringstream& ss_);
		//オブジェクト（キャラクタ）生成
		bool AppearObject(stringstream& ss_);
		//マップ再読み込み
		bool MapLoad(stringstream& ss_);
		//キャラ情報の変更
		bool ModifyChara(stringstream& ss_);
		//オブジェクト（キャラクタ）の削除
		bool KillObject(stringstream& ss_);
	};
}