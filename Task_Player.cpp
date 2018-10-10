//-------------------------------------------------------------------
//�v���C��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "playerImg";
		DG::Image_Create(this->imageName, "./data/image/Image_Load.png");
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
		this->speed = 0.0f;															//���x
		this->gauge_Extra = 0.0f;													//EX�Q�[�W��
		this->add_Extra=0.0f;														//EX�Q�[�W������
		this->pos = ML::Vec2(float(ge->screenWidth/2), float(ge->screenHeight/2));	//���W
		this->moveVec=ML::Vec2(0.0f,0.0f);											//�ړ���
		this->gravity = ML::Gravity(32);											//�d�͉����x
		this->hitBase=ML::Box2D(-46,-46,92,92);										//�����`
		this->controllerName="P1";													//�R���g���[���[�錾

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
		this->Think();
		this->Move();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		ML::Box2D src(0, 0, 92, 92);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//-------------------------------------------------------------------
	//���[�V�������X�V�i�ύX�Ȃ��̏ꍇ	false)
	bool  Object::UpDateMotion(const Motion  nm_)
	{
		if (nm_ == this->motion)
		{
			return false;
		}
		else
		{
			this->motion = nm_;
			this->moveCnt = 0;
			this->animCnt = 0;
			return true;
		}
	}
	//�ڐG���̉�������
	//����	�F	�i�U����,�U�����\����,���G���ԁj
	void Object::Recieved(const BChara* from_, const AttackInfo at_, const int& unhitTime_)
	{
		//���G���Ԃ�����΍U�����󂯂Ȃ�
		if (this->unhitCnt > 0)
		{
			return;
		}
		//�O�����疳�G���Ԃ��w��
		this->unhitCnt = unhitTime_;
		//�����Ԃɂ���
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
			if (this->CheckFoot()) { nm = Landing; }
			break;
		case Landing:
			if (this->moveCnt >= 8) { nm = Stand; }
			break;
		case Damage:
			break;
		}
		this->UpDateMotion(nm);
	}
	void Object::Move()
	{
		//�d�͉���
		switch (this->motion) {
		default:
			//�㏸���������͑����ɒn�ʂ�����
			if (this->moveVec.y < 0 ||
				this->CheckFoot() == false) {
				this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_FallSpeed);
			}
			//�n�ʂɐڐG���Ă���
			else {
				this->moveVec.y = 0.0f;
			}
			break;
			//�ȉ��A�d�͂ɉe������Ȃ����

		}
		//�R���g���[���̐錾
		auto in = DI::GPad_GetState(this->controllerName);
		//���[�V�������̏���
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
			if (this->moveCnt == 0)
			{
				this->moveVec.y = this->pow_Jump;
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
		Object::DrawInfo imageTable[] = {
			{this->hitBase,ML::Box2D(0,0,0,0),dc}
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
	//�߂荞�܂Ȃ��ړ�����
	//void Object::CheckMove(ML::Vec2&  e_)
	//{
	//	//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
	//	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	//	if (nullptr == map) { return; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j

	//								   //�����ɑ΂���ړ�
	//	while (e_.x != 0) {
	//		float  preX = this->pos.x;
	//		if (e_.x >= 1) { this->pos.x += 1;		e_.x -= 1; }
	//		else if (e_.x <= -1) { this->pos.x -= 1;		e_.x += 1; }
	//		else { this->pos.x += e_.x;		e_.x = 0; }
	//		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
	//		if (true == map->CheckHit(hit)) {
	//			this->pos.x = preX;		//�ړ����L�����Z��
	//			break;
	//		}
	//	}
	//	//�c���ɑ΂���ړ�
	//	while (e_.y != 0) {
	//		float  preY = this->pos.y;
	//		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
	//		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
	//		else { this->pos.y += e_.y;		e_.y = 0; }
	//		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
	//		if (true == map->CheckHit(hit)) {
	//			this->pos.y = preY;		//�ړ����L�����Z��
	//			break;
	//		}
	//	}
	//}
	//-----------------------------------------------------------------------------
	//�����ڐG����
	//bool  Object::CheckFoot()
	//{
	//	//�����蔻�����ɂ��đ�����`�𐶐�
	//	ML::Box2D  foot(this->hitBase.x,
	//		this->hitBase.y + this->hitBase.h,
	//		this->hitBase.w,
	//		1);
	//	foot.Offset(this->pos);

	//	auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
	//	if (nullptr == map) { return false; }//�}�b�v��������Δ��肵�Ȃ�(�o���Ȃ��j
	//	return map->CheckHit(foot);
	//}
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