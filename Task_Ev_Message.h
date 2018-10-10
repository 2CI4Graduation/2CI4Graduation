#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//メッセージ表示
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  Ev_message
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName(		"メッセージ表示枠");	//グループ名
	const  string  defName(				"Noname");	//タスク名
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
		string imageNames[10];//メッセージ表示枠10種類
		string fontNames[3];//フォント3種類

	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
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
		int bgNumber;
		int fontNumber;
		ML::Vec2 pos;
		string msgText;
		int timeCnt;
		int timeLimit;

		//タスクを生成するか、同名のタスクの情報を更新する
		static void CreateOrReset(stringstream& ss_);
		//表示するメッセージを設定する
		void Set(const string& taskName_, stringstream& ss_);
		
	};
}