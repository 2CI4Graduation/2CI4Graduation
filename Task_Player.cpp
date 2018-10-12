//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerImg";
		DG::Image_Create(this->imageName, "./data/image/HitTest.bmp");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[1] = 0.5f;			//�`�揇
		this->angle_LR = Right;						//���E
		this->motion = Stand;						//��ԊǗ�
		this->controllerName = "P1";				//�R���g���[���錾
		this->hitFlag = true;						//
		this->pow_jump = -7.5f;						//�W�����v����
		this->speed_fall = 0.5f;					//�������x���Z��
		this->max_FallSpeed = 1.5f;					//�������x���
		this->hitBase = ML::Box2D(32, 64, 64, 128);	//�����`
		this->neutral_height = 128;					//�ʏ펞�̔����`����
		this->squat_height = 64;

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->moveCnt++;
		this->animCnt++;
		auto  in = DI::GPad_GetState(this->controllerName);
		ML::Vec2  est(0, 0);

		this->CheckMove(est);
		//�����ڐG����
		this->hitFlag = this->CheckFoot();
		this->pos += this->moveVec;
		this->Think();
		this->Move();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D  draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D  src(0, 0, 100, 100);

		//	�ڐG�̗L���ɍ��킹�ĉ摜��ύX
		if (true == this->hitFlag) {
			src.Offset(100, 0);
		}
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//�ڐG���̉�������
	//����	�F	�i�U����,�U�����,�^���閳�G���ԁj
	void Object::Recieved(const BChara* from_, AttackInfo at_, const int& unHit_)
	{
		if (this->unHitCnt > 0)
		{
			return;//���G����
		}
		//���G���Ԃ��󂯎��
		this->unHitCnt = unHit_;
		//�_���[�W��Ԃɂ���
		this->UpDateMotion(Damage);
	}
	//�v�l
	void Object::Think()
	{
		//���݂̏�Ԃ�ۑ�
		Motion nm = this->motion;
		//�R���g���[���[�̐錾
		auto in = DI::GPad_GetState(this->controllerName);
		switch (this->motion)
		{
		default:
			break;
		case Stand:
			if (in.B1.down) { nm = Takeoff; }
			if (in.LStick.D.on) { nm = Squat; }
			break;
		case Squat:
			if (in.LStick.D.off) { nm = Stand; }
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
			if (this->moveVec.y==0.0f) { nm = Landing; }
			break;
		case Landing:
			if (this->moveCnt >= 8) { nm = Stand; }
			break;
		case Damage:
			if (this->unHitCnt <= 0) { nm = Stand; }
			break;
		}
		this->UpDateMotion(nm);
	}
	void Object::Move()
	{
		//�d�͉���
			//���ɉ�ʉ��[�Ŏ~�܂�
			//�d��
		if (this->pos.y <= 720 - this->hitBase.h / 2)
		{
			this->moveVec.y += this->speed_fall;
		}
		else
		{
			this->moveVec.y = 0.0f;
		}
		//�R���g���[���̐錾
		auto in = DI::GPad_GetState(this->controllerName);
		//���[�V�������̏���
		switch (this->motion)
		{
		default:
			break;
		case Stand:
			this->pos.y = float(720 - this->hitBase.h / 2);
			this->hitBase.h = this->neutral_height;
			break;
		case Squat:
			this->pos.y = 720;
			this->hitBase.h = this->squat_height;
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
				this->moveVec.y = this->pow_jump;
			}
			break;
		case Fall:
			break;
		case Landing:
			break;
		case Damage:
			break;
		}
		//�ړ��ʂ𔽉f����
		this->pos += this->moveVec;
	}
	//�A�j���[�V��������
	Object::DrawInfo Object::Anim()
	{
		//�f�t�H���g�̒l��p��
		ML::Color dc(1, 1, 1, 1);
		Object::DrawInfo imageTable[] = 
		{
			{ this->hitBase,ML::Box2D(0,0,0,0),dc }
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
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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