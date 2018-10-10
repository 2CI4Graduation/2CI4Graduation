//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ev_Message.h"
#include  "Task_EventEngine.h"

namespace  Ev_message
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		{
			struct BGImageParam
			{
				string name;
				string path;
			};
			BGImageParam w[10] = {
				{"BMG �W��","./data/image/MessageF.png"},
			{ "BMG �{��","./data/image/MessageF.png" },//�ȉ���
			{ "BMG ���f","./data/image/MessageF.png" },
			{ "BMG �\��A","./data/image/MessageF.png" },
			{ "BMG �\��B","./data/image/MessageF.png" },
			{ "BMG �\��C","./data/image/MessageF.png" },
			{ "BMG �\��D","./data/image/MessageF.png" },
			{ "BMG �\��E","./data/image/MessageF.png" },
			{ "BMG �\��F","./data/image/MessageF.png" },
			{ "BMG �\��G","./data/image/MessageF.png" },
			};
			for (int i = 0; i <_countof(this->imageNames); ++i)
			{
				this->imageNames[i] = w[i].name;
				DG::Image_Create(w[i].name, w[i].path);
			}
		}
		{
			struct FontParam
			{
				string name;
				string fontName;
				int width;
				int height;
			};
			FontParam w[3] =
			{
				{"MFont ��","HG�ۺ޼��M-PRO",8,16},
			{ "MFont ��","HG�ۺ޼��M-PRO",12,24 },
			{ "MFont ��","HG�ۺ޼��M-PRO",16,32 },
			};
			for (int i = 0;i<_countof(this->fontNames); ++i)
			{
				this->fontNames[i] = w[i].name;
				DG::Font_Create(w[i].name, w[i].fontName, w[i].width, w[i].height);
			}
		}







		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (int i = 0; i < _countof(this->imageNames); ++i)
		{
			DG::Image_Erase(this->imageNames[i]);
		}

		for (int i = 0; i < _countof(this->fontNames); ++i)
		{
			DG::Font_Erase(this->fontNames[i]);
		}
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
		this->Stop();//���g���~��Ԃɂ��Ă���
		this->render2D_Priority[1] = 0.1f;
		this->pos.x = 0;
		this->pos.y = 270 - 128;
		this->msgText = "";
		this->bgNumber = 0;
		this->fontNumber = 1;

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
		auto in = DI::GPad_GetState("P1");
		//�������Ԍo�߂������̓{�^���������ꂽ�Ƃ�
		if ((this->timeLimit != 0 && this->timeCnt >= this->timeLimit) || in.B2.down)
		{
			//���g�͒�~��Ԃɂ���
			this->Stop();
			//�G���W���̒�~��Ԃ���������
			ge->StopAll_GN("�C�x���g", "���s�G���W��",false);
		}
		else
		{
			this->timeCnt++;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0, 480, 128);
		ML::Box2D src(0, 0, 240, 64);
		draw.Offset(this->pos);
		DG::Image_Draw(this->res->imageNames[this->bgNumber], draw, src, ML::Color(0.9f, 1, 1, 1));
		ML::Box2D drawF(6, 6, 480-12, 128-12);
		drawF.Offset(this->pos);
		DG::Font_Draw(this->res->fontNames[this->fontNumber], drawF,this->msgText);
	}
	//-------------------------------------------------------------------------------
	//�\�����郁�b�Z�[�W��ݒ肷��
	void Object::Set(const string& taskName_, stringstream& ss_)
	{
		//�p�����[�^�𕪉�����
		int timeL;
		string msg;
		ss_ >> timeL >> msg;

		//�^�X�N����ݒ�
		this->name = taskName_;
		//���b�Z�[�W�g�����v���֑Ή�����
		if (msg == "off")
		{
			this->Kill();
			return;
		}
		this->timeCnt = 0;
		this->timeLimit = timeL;
		this->msgText = msg;
		//*�����s�ɒu��������
		string::size_type ast;
		while ((ast = msgText.find("*")) != string::npos)
		{
			msgText.at(ast) ='\n';
		}
		//�C�x���g�G���W�����ꎞ��~������
		ge->StopAll_GN("�C�x���g", "���s�G���W��");
		//���g�̒�~��Ԃ���������
		this->Stop(false);
	}

	//�^�X�N�𐶐����邩�A�����̃^�X�N�̏����X�V����
	 void Object::CreateOrReset(stringstream& ss_)
	{
		//�p�����[�^�𕪉�����
		string taskName;
		ss_ >> taskName;
		//�V�K�쐬���X�V���̔���
		auto p = ge->GetTask_One_GN<Object>("���b�Z�[�W�\���g", taskName);

		//�V�K�쐬�̏ꍇ
		if (nullptr==p)
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