#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�C�x���g���s�G���W��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace  EventEngine
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"�C�x���g");	//�O���[�v��
	const  string  defName(				"���s�G���W��");	//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
								//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
	//�ύX������������������������������������������������������
	public:
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		static WP instance;//���d�����h�~
		ifstream evFile;//�C�x���g�t�@�C��

		//�C�x���g���s�G���W���𐶐����܂��B
		//���ɃC�x���g���s�G���W�������݂��Ă���ꍇ�A�����͎��s���܂��B
		static Object::SP Object::Create_Mutex();
		//�C�x���g�t�@�C������o�^���C�x���g�J�n
		bool Set(const string& fPath_);

		//�P�s���̃f�[�^��ǂݍ���
		bool ReadLine(string& lineT_);

		//�s�f�[�^�ɑ΂��鏈�����s��
		bool Execute(string& hs_, string& ds_);

		//���V�X�e������n
		//�C�x���g�t���O�̃f�[�^�̏�������
		bool EventFlag(stringstream& ss_);
		//��������
		bool If(stringstream& ss_);
		//�摜�̓ǂݍ���
		bool Object::Image(stringstream& ss_);
		//�C�x���g�t�@�C���̍đI��
		bool FileSet(stringstream& ss_);
		//�I�u�W�F�N�g�i�L�����N�^�j����
		bool AppearObject(stringstream& ss_);
		//�}�b�v�ēǂݍ���
		bool MapLoad(stringstream& ss_);
		//�L�������̕ύX
		bool ModifyChara(stringstream& ss_);
		//�I�u�W�F�N�g�i�L�����N�^�j�̍폜
		bool KillObject(stringstream& ss_);
	};
}