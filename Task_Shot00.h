#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�v���C���̏o���e
//-------------------------------------------------------------------
#include  "BChara.h"

namespace  Shot00
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName(		"�e�i�v���C���j");	//�O���[�v��
	const  string  defName(				"NoName");	//�^�X�N��
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
		string imageName; 
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
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
		int moveCnt;			//�s���J�E���^
		int animCnt;			//�A�j���[�V�����J�E���^
		int unhitCnt;			//���G����
		float speed;			//���x
		float pow_Jump;			//�W�����v����
		float max_FallSpeed;	//�ő���ʑ��x
		float gravity;			//�d�͉����x
		float gauge_Extra;		//EX�Q�[�W��
		float add_Extra;		//EX�Q�[�W������
		ML::Vec2 pos;			//���W
		ML::Vec2 moveVec;		//�ړ���
		ML::Box2D hitBase;		//�����`
		Motion motion;			//��ԊǗ�
		string controllerName;	//�R���g���[���[�錾

								//�\����

								//�U�����
								//(�U����,�������x,����)
		struct AttackInfo
		{
			int power;	//�U����
			int element;//����
		};
		//�A�j���[�V�������\����
		struct DrawInfo
		{
			ML::Box2D	draw, src;
			ML::Color	color;
		};
	public:
		//�߂荞�܂Ȃ��ړ�����
		//����	�F	�i�ړ��ʁj
		void  CheckMove(const ML::Vec2&  est_);
		//�����ڐG����
		bool  CheckFoot();
		//�v�l
		void Think();
		//�s��
		void Move();
		//���[�V�������X�V�i�ύX�Ȃ��̏ꍇ	false)
		bool  UpDateMotion(const Motion  nm_);
		//�ڐG���̉�������
		//����	�F	�i�U����,�U�����\����,���G���ԁj
		void Recieved(const BChara* from_, const AttackInfo at_, const int& unhitTime_);
		//�A�j���[�V��������
		DrawInfo Anim();
		
	};
}