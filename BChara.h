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
	enum Motion
	{
		Stand,
		Gu,
		Choki,
		Paa,
		Takeoff,
		Jump,
		Fall,
		Landing,
		Damage
	};
};
