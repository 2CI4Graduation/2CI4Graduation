//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Shot00.h"
#include  "Task_Player.h"
#include  "Task_Map2D.h"

#include  "BChara.h"



namespace  Shot00
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "Shot00Img";
		DG::Image_Create(this->imageName, "./data/image/Shot00.png");
		
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
		this->render2D_Priority[1] = 0.4f;
		/*this->pos.x = 0;
		this->pos.y = 0;*/
		this->hitBase = ML::Box2D(-8, -8, 16, 16);
		this->hp = 5;
		/*this->moveVec = ML::Vec2(0, 0);
		this->moveCnt = 0;*/
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
		//���E�̎��Ԃ��}���������
		if (this->moveCnt>=30)
		{
			//���Ő\��	
			this->Kill();
			return;

		}
		//�ړ�

		this->pos += this->moveVec;

		//�ړ���ŏ�Q���ɐڐG���������
		//�}�b�v�����݂��邩���ׂĂ���A�N�Z�X
		if (auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v"))
		{
			ML::Box2D hit = this->hitBase.OffsetCopy(this->pos);
			if (true==map->CheckHit(hit))
			{
				this->Kill();

				//�Ƃ肠�����ق�
				for (int  c = 0; c <4; c++)
				{
					auto eff = Effect00::Object::Create(true);
					eff->pos = this->pos;
				}
				return;
			}
			
		}
		//�G�ΏۂƏՓ˔��聕�_���[�W��^���鏈��
		//{
		//ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		//auto targets = ge->GetTask_Group_G<BChara>("�G");

		//for (auto it = targets->begin(); it != targets->end(); it++)
		//{
		//	//����ɐڐG�̗L�����m�F������
		//	if ((*it)->CheckHit(me))
		//	{
		//		//����Ƀ_���[�W�̏������s�킹��
		//		BChara::AttackInfo at = { this->hp,0,0 };
		//		(*it)->Received(this, at);
		//		

		//		//�Ƃ肠�����ق�
		//		for (int c = 0; c <4; c++)
		//		{
		//			auto eff = Effect00::Object::Create(true);
		//			eff->pos = this->pos;
		//		}
		//		this->Kill();
		//		break;
		//	}

		BChara::AttackInfo at = { this->hp,0,0 };
		if (true == this->Attack_Std("�G", at))
		{
			//�ڐG���Ɏ����ɉ����������Ȃ�
			//�Ƃ肠�����ق�
					for (int c = 0; c <4; c++)
					{
						auto eff = Effect00::Object::Create(true);
						eff->pos = this->pos;
					}
					this->Kill();
		}
		
	

	//	ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
	//	////�G�����ׂĒ��o
	//	auto target = ge->GetTask_Group_G<Enemy00::Object>("�G");
	//	for (auto it = target->begin(); it!=target->end(); it++){
	///*	for (auto it = ge->qa_Enemys->begin(); it != ge->qa_Enemys->end(); it++)
	//	{
	//		if ((*it)->CheckState() == BTask::eKill) { continue; }
	//	{*/
	//		ML::Box2D you = (*it)->hitBase.OffsetCopy((*it)->pos);
	//		if (true == you.Hit(me))
	//		{
	//			(*it)->Kill();
	//			this->Kill();

	//			//�Ƃ肠�����ق�
	//			for (int c = 0; c <4; c++)
	//			{
	//				auto eff = Effect00::Object::Create(true);
	//				eff->pos = this->pos;
	//			}
	//			break;
	//		}
	//	}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(-8, -8, 16, 16);
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 32, 32);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
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