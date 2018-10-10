#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�t�F�[�h�C�����A�E�g
//-------------------------------------------------------------------
#include "BChara.h"
#include "GameEngine_Ver3_7.h"

namespace  Ev_FadeInOut
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"�t�F�[�hIO");	//�O���[�v��
	const  string  defName(				"Def");		//�^�X�N��
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
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
	};
	////-------------------------------------------------------------------
	//class  Object : public  BChara
	//{
	//public:
	//	virtual  ~Object();
	//	typedef  shared_ptr<Object>		SP;
	//	typedef  weak_ptr<Object>		WP;
	//	//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
	//	static  Object::SP  Create(bool flagGameEnginePushBack_);
	//	Resource::SP	res;
	//private:

	//	Object();
	//	bool  B_Initialize();
	//	bool  B_Finalize();
	//	bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
	//	void  UpDate();		//�u���s�v�P�t���[�����ɍs������
	//	void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
	//	bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//public:
	////�ύX������������������������������������������������������
	//	//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
	//	//BChara�Ɋ܂܂�Ȃ����m�݂̂����ɒǉ�����
	//	
	//};

	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
								//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		string imageName;
		int Cnt;
		ML::Box2D src;
		enum Mode{Out,In};
		Mode mode;

		//�^�X�N�𐶐�����i�t�F�[�h�C���j���A�t�F�[�h�A�E�g����
		static void CreateOrFadeOut(stringstream& ss_);
		//�t�F�[�h�C���̏������s��
		void Set(stringstream& ss_);
	};
}