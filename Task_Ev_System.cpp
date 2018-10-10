//----------------------------------------------------------------------------------------------------------------------------
//�C�x���g���s�G���W���̃V�X�e������n���\�b�h
//-------------------------------------------------------------------------------------------------------------------
#include "MyPG.h"
#include "Task_EventEngine.h"
#include"BChara.h"
#include"AppearObject.h"
#include"Task_Map2D.h"
namespace EventEngine
{//�C�x���g�t���O�̃f�[�^��������

	bool Object::EventFlag(stringstream& ss_)
	{
		string flagname;
		string sign;
		float value;
		ss_ >> flagname >> sign >> value;
		if (sign=="=")
		{
			ge->evFlags[flagname] = value;
		}
		else if(sign=="+")
		{
			ge->evFlags[flagname] += value;
		}
		else if (sign == "-")
		{
			ge->evFlags[flagname] -= value;
		}
		else
		{
			return false;
		}
		return true;
	}
	bool Object::If(stringstream& ss_)
	{
		//�p�����[�^����Ώۂ𓾂�
		string flagKind;
		ss_ >> flagKind;
		bool flag = false;
		//int skip=0;
		string labelName;
		if (flagKind == "ev_flag")
		{
			string flagName;
			string sign;
			float value;
			ss_ >> flagName >> sign >> value >> labelName;
			//��r
			if (sign=="==")
			{
				flag = ge->evFlags[flagName] == value;
			}
			else	if (sign == ">")
			{
				flag = ge->evFlags[flagName] > value;
			}
			else if (sign == "<")
			{
				flag = ge->evFlags[flagName] <value;
			}
			else
			{
				return false;
			}
		}
		//�L�����N�^�[�̃p�����[�^��ΏۂƂ���ꍇ
		else if(flagKind=="charaParam")
		{
			//������
		}
		//���ϐ���ΏۂƂ���ꍇ
		else if(flagKind=="System")
		{
			//������
		}

		//�ړI�̃��x���܂Ői��
		if (true==flag)
		{
			//�ǂݍ��݈ʒu���t�@�C���̐擪�֖߂�
			this->evFile.seekg(0, ios_base::beg);
			//1�s�ǂݍ��݂��J��Ԃ�
			string lt;
			while (this->ReadLine(lt))
			{
				//�s�����
				string::size_type t = lt.find(">");
				string headerStr = lt.substr(0, t);
				string dataStr = lt.substr(t + 1);
				if ("label"==headerStr)
				{
					//�ǂݍ��񂾍s�����x��
					stringstream ss;
					ss << dataStr;
					string labelName2;
					ss >> labelName2;
					if (labelName==labelName2)
					{//���x���l�[������v
						break;

					}
				}
			}
			


		}
		return true;
	}
	//�摜�̓ǂݍ���
	bool Object::Image(stringstream& ss_)
	{
		string OffScreenName;
		string filePath;
		ss_ >> OffScreenName >> filePath;
		DG::Image_Create(OffScreenName, filePath);
		return true;
	}
	//�C�x���g�t�@�C���̍đI��
	bool Object::FileSet(stringstream& ss_)
	{
		string filePath;
		ss_ >> filePath;
		return this->Set(filePath);
	}
	//�I�u�W�F�N�g�i�L�����N�^�j����
	bool Object::AppearObject(stringstream& ss_)
	{
		//�p�����[�^����Ώۂ𓾂�
		string kind;
		float posX, posY;
		ss_ >> kind >> posX >> posY;
		//�I�u�W�F�N�g�����i�G���W���ɓo�^�j
		BChara::SP obj = AppearObject_BChara(kind);
		if (obj)
		{
			obj->pos.x = posX;
			obj->pos.y = posY;
			obj->Stop();//��~��Ԃɂ��Ă���

		}
		//�C�Ӄp�����[�^�ւ̑Ή�
		while (false==ss_.eof())
		{
			string paramName, eq;
			ss_ >> paramName >> eq;
			//�C�Ӄp�����[�^�łȂ��Ȃ�
			if (eq != "=")
			{
				break;
			}
			if (paramName=="name")
			{
				ss_ >> obj->name;
			}
			/*if (paramName == "jumpPow")
			{
				ss_ >> obj->jumpPow;
			}
			if (paramName == "maxFallSpeed")
			{
				ss_ >> obj->maxFallSpeed;
			}
			if (paramName == "maxSpeed")
			{
				ss_ >> obj->maxSpeed;
			}*/
			if (paramName == "eventFileName")
			{
				ss_ >> obj->eventFileName;

			}
			//���̃p�����[�^���K�v�ɉ����đ��₵�Ă���
		}
		return nullptr != obj;
	}
	//�}�b�v�ēǂݍ���
	bool Object::MapLoad(stringstream& ss_)
	{
		auto map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
		if (nullptr==map)
		{
			map = Map2D::Object::Create(true);
		}
		//�}�b�v�ǂݍ���
		string mapFilePath;
		ss_ >> mapFilePath;
		return map->Load(mapFilePath);
	}
	//�L�������̕ύX
	bool Object::ModifyChara(stringstream& ss_)
	{
		//�p�����[�^����Ώۂ𓾂�
		string gname, name;
		ss_ >> gname >> name;
		//�I�u�W�F�N�g�����o
		if (auto mc=ge->GetTask_One_GN<BChara>(gname,name))
		{
			while (false == ss_.eof())
			{
				string paramName, eq;
				ss_ >> paramName >> eq;
				//=�ɑ΂��鏈��
				if (eq == "=")
				{


					if (paramName == "posX")
					{
						ss_ >> mc->pos.x;
					}
					if (paramName == "posY")
					{
						ss_ >> mc->pos.y;
					}
					/*if (paramName == "jumpPow")
					{
						ss_ >> mc->jumpPow;
					}
					if (paramName == "maxFallSpeed")
					{
						ss_ >> mc->maxFallSpeed;
					}
					if (paramName == "maxSpeed")
					{
						ss_ >> mc->maxSpeed;
					}*/
					if (paramName == "event")
					{
						ss_ >> mc->eventFileName;
					}
					//���̃p�����[�^���K�v�ɉ����Ēǉ�
				}
			}
			return true;
		}
		return false;
	}

	//�I�u�W�F�N�g�i�L�����N�^�j�̍폜
	bool Object::KillObject(stringstream& ss_)
	{
		//�p�����[�^����Ώۂ𓾂�
		string gname;
		ss_ >> gname;
		//�^�X�N�����܂ޏꍇ
		if (false==ss_.eof())
		{
			string name;
			ss_ >> name;
			ge->KillAll_GN(gname, name);
		}
		//�O���[�v���݂̂̏ꍇ
		else 
		{
			ge->KillAll_G(gname);
		}
		return true;
	}
}