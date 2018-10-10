//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ev_FadeInOut.h"

namespace  Ev_FadeInOut
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
		this->render2D_Priority[1] = 0.0005f;
		this->Cnt = 0;
		this->imageName = "";
		this->Stop();
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		if (this->imageName!="")
		{
			DG::Image_Erase(this->imageName);
		}

		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		if (this->mode==In)
		{
			this->Cnt--;
			if (this->Cnt<0)
			{
				//�C�x���g�G���W���ĊJ
				ge->StopAll_GN("�C�x���g", "���s�G���W��",false);
				//���ł�����
				this->Kill();
			}
		}

		if (this->mode==Out)
		{
			this->Cnt++;
			if (this->Cnt > 60)
			{
				//�C�x���g�G���W���ĊJ
				ge->StopAll_GN("�C�x���g", "���s�G���W��", false);
				//��~��Ԃɂ���
				this->Stop();
			}
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		DG::Image_Draw(this->imageName, draw, this->src, ML::Color(this->Cnt / 60.0f, 1, 1, 1));
	}
	//�^�X�N�𐶐�����i�t�F�[�h�C���j���A�t�F�[�h�A�E�g����
	 void Object::CreateOrFadeOut(stringstream& ss_)
	{
		//�V�K�쐬���X�V�̔��f
		auto p = ge->GetTask_One_GN<Object>(defGroupName, defName);

		//�V�K�쐬
		if (nullptr==p)
		{
			p = Object::Create(true);
			p->Set(ss_);
		}
		//�X�V
		else
		{
			p->Set(ss_);
		}
	}

	//�t�F�[�h�C���̏������s��
	void Object::Set(stringstream& ss_)
	{
		//�p�����[�^�𕪉�
		string filePath;
		ss_ >> filePath;

		//�t�F�[�h�C������ꍇ
		if (filePath=="in")
		{
			this->mode = In;
			this->Cnt=60;
		}
		//�t�F�[�h�A�E�g
		else
		{
			this->mode = Out;
			this->Cnt = 0;
			//�C���[�W���𐶐�
			this->imageName = this->groupName + this->name + "Img";
			//�摜�ǂݍ���
			DG::Image_Create(this->imageName, filePath);
			POINT s = DG::Image_Size(this->imageName);
			this->src = ML::Box2D(0, 0, s.x, s.y);

		}

		//�C�x���g�G���W�����ꎞ��~������
		ge->StopAll_GN("�C�x���g", "���s�G���W��");
		//��~��Ԃ���������
		this->Stop(false);
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