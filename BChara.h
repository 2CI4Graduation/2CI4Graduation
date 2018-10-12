#pragma warning(disable:4996)
#pragma once
//-----------------------------------------------------------------------------
//�L�����N�^�ėp�X�[�p�[�N���X
//-----------------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

class BChara : public BTask
{
	//�ύX�s����������������������������������������������������
public:
	typedef shared_ptr<BChara>		SP;
	typedef weak_ptr<BChara>		WP;
public:
	//�ύX������������������������������������������������������

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
	//��ԊǗ�
	enum Motion
	{
		Stand,	//����
		Squat,	//���Ⴊ��
		Gu,		//�O�[
		Choki,	//�`���L
		Paa,	//�p�[
		Takeoff,//����
		Jump,	//�W�����v��
		Fall,	//������
		Landing,//���n
		Damage	//�_���[�W
	};

	//�L�����N�^���ʃ����o�ϐ�
	Motion motion;			//��ԊǗ�
	ML::Vec2    pos;		//�L�����N�^�ʒu
	ML::Box2D   hitBase;	//�����蔻��͈�
	ML::Vec2	moveVec;	//�ړ��x�N�g��
	int			moveCnt;	//�s���J�E���^
	int			animCnt;	//�A�j���[�V�����J�E���^
	int			unHitCnt;	//���G���ԃJ�E���^
	//���E�̌����i2D�����_�Q�[����p�j
	enum Angle_LR { Left, Right };
	Angle_LR	angle_LR;
	WP			target;
	string eventFileName;

	//�����o�ϐ��ɍŒ���̏��������s��
	//���������o�ϐ���ǉ�������K�����������ǉ����鎖����
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

	//�L�����N�^���ʃ��\�b�h
	//�߂荞�܂Ȃ��ړ�����
	virtual  void  CheckMove(ML::Vec2&  est_);
	//�����ڐG����
	virtual  bool  CheckFoot();

	//�v�l
	virtual void Think()
	{
		ML::MsgBox("Recieved ��������Ă��܂���");
	}
	//�s��
	virtual void Move()
	{
		ML::MsgBox("Recieved ��������Ă��܂���");
	}
	//���[�V�������X�V�i�ύX�Ȃ��̏ꍇ	false)
	bool  UpDateMotion(const Motion  nm_);
	//�ڐG���̉�������
	//����	�F	�i�U����,�U�����\����,���G���ԁj
	virtual void Recieved(const BChara* from_, const AttackInfo at_, const int& unhitTime_)
	{
		ML::MsgBox("Recieved ��������Ă��܂���");
	}
};
