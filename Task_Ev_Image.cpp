//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ev_Image.h"

namespace  Ev_Image
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
		this->render2D_Priority[1] = 0.2f;
		this->imageName = "";
		this->pos = ML::Vec2(0, 0);
		this->drawBase = ML::Box2D(0, 0, 0, 0);
		this->src = ML::Box2D(0, 0, 0, 0);
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->drawBase;
		draw.Offset(this->pos);
		DG::Image_Draw(this->imageName, draw, this->src);
	}
	//----------------------------------------------------------

	//�^�X�N�𐶐����邩�A�ǂ��߂��̃^�X�N�̏����X�V����
	void Object::CreateOrReset(stringstream& ss_)
	{
		//�p�����[�^�𕪉�����
		string taskName;
		ss_ >> taskName;
		//�V�K�쐬���X�V���̔���
		auto p = ge->GetTask_One_GN<Object>("�C�x���g�摜", taskName);

		//�V�K�쐬�̏ꍇ
		if (nullptr == p)
		{
			p = Object::Create(true);
			p->Set(taskName, ss_);


		}
		//�X�V�̏ꍇ
		else
		{
			p->Set(taskName, ss_);
		}
	 }

	//�\�����郁�b�Z�[�W��ݒ肷��
	void Object::Set(const string& taskName_, stringstream& ss_)
	{
		//�p�����[�^�𕪉�����
		string filePath;
		ss_ >> filePath;
		//�摜�\�������ɂ�����������
		if (filePath=="off")
		{
			this->Kill();
			return;
		}
		//�^�X�N����ݒ�
		this->name = taskName_;
		//�C���[�W����ݒ�
		this->imageName = this->groupName + this->name + "Img";
		//�摜�ǂݍ���
		DG::Image_Create(this->imageName, filePath);
		
		//�\���ʒu�E������ǂݍ���
		enum XPosition
		{
			Left,Center,Right
		};
		XPosition xPos = Left;//�\���ʒu
		bool xRevers = false;//�摜���]
		bool yRevers = false;//�摜���]
		string posAndRev;
		ss_>> posAndRev;
		//�񂹁E���]�̎w��
		if (string::npos!=posAndRev.find("L"))
		{
			xPos = Left;
		}
		if (string::npos != posAndRev.find("R"))
		{
			xPos = Right;
		}
		if (string::npos != posAndRev.find("C"))
		{
			xPos = Center;
		}
		if (string::npos != posAndRev.find("X"))
		{
			xRevers = true;
		}
		else
		{
			xRevers =false;
		}
		if (string::npos != posAndRev.find("Y"))
		{
			yRevers = true;
		}
		else
		{
			yRevers = false;
		}

		//�摜�̃T�C�Y���炢����ݒ�
		POINT s = DG::Image_Size(this->imageName);
		this->drawBase = ML::Box2D(0, 0, s.x, s.y);
		this->src = ML::Box2D(0, 0, s.x, s.y);

		//��
		if (Left==xPos)
		{
			this->pos.x = 0;
		}
		else if(Right==xPos)
		{
			this->pos.x = (float)ge->screen2DWidth - s.x;

		}
		else
		{
			this->pos.x = (ge->screen2DWidth - s.x)/2.0f;
		}
		//���]
		if (true==xRevers)
		{
			this->src.x = s.x;
			this->src.w = -s.x;
		}
		if (true == yRevers)
		{
			this->src.y = s.y;
			this->src.h= -s.y;
		}

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