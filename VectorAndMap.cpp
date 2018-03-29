#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>
#define NAME_LENGTH 20
#define DEAD 0
#define LIVE 1

using namespace std;

class MonsterManager;
class Monster;
class Player;

class Monster
{
private :
	MonsterManager * manager;
	char name[NAME_LENGTH];
	int hp;
	int damage;
public :
	Monster(const char * _name, int _hp, int _damage);

	char * GetName()
	{
		return name;
	}
	int GetHP()
	{
		return hp;
	}
	int GetDamage()
	{
		return damage;
	}

	// ������� �ް�, hp���� ��ȯ
	int ReceiveDamage(int _damage);
	// ���Ͱ� �÷��̾�� ����, ������ DEAD, ��� LIVE��ȯ
	int Attack(Player * player);
};

class Player
{
private :
	MonsterManager * manager;
	int hp;
	int damage;

public :
	Player(int _hp, int _damage);

	int GetHP()
	{
		return hp;
	}
	int GetDamage()
	{
		return damage;
	}

	// ������� �ް�, hp���� ��ȯ
	int ReceiveDamage(int _damage);
	// �÷��̾ ���Ϳ��� ����, ������ DEAD, ��� LIVE��ȯ
	int Attack(Monster * monster);
};

class MonsterManager
{
private :
	vector<Monster*> * monsterVector;
	Player * player;
	static MonsterManager * instance;
	
	MonsterManager();

public :
	static MonsterManager * GetInstance()
	{
		if (instance == NULL) instance = new MonsterManager();
		return instance;
	}

	~MonsterManager();
	// �޴� ����
	void ShowMenu();
	// ���� ����� �޼ҵ�
	void CreateMonster();
	// ���Ϳ� �ο�� �޼ҵ�
	int FightMonster();
	// ���͸� �����ϴ� �޼ҵ�
	void RemoveMonster(Monster * monster);
};
// MonsterManager static ���� �ʱ�ȭ
MonsterManager * MonsterManager::instance = NULL;

int main(void)
{
	MonsterManager * manager = MonsterManager::GetInstance();

	// �� ����
	map<string, Monster*> monsterMap;

	// Map�� 3���� ���� ������ �ִ´�.
	monsterMap.insert(make_pair("Rabbit", new Monster("Rabbit", 20, 10)));
	monsterMap.insert(make_pair("Onnie", new Monster("Onnie", 50, 20)));
	monsterMap.insert(make_pair("Boss", new Monster("Boss", 100, 40)));

	// Map�� 3���� ���� ������ ��� �� �����Ѵ�.
	cout << "Rabbit ����" << endl;
	cout << "HP : " << monsterMap.find("Rabbit")->second->GetHP() << endl;
	cout << "Damage : " << monsterMap.find("Rabbit")->second->GetDamage() << endl;
	delete monsterMap.find("Rabbit")->second;

	cout << "Onnie ����" << endl;
	cout << "HP : " << monsterMap.find("Onnie")->second->GetHP() << endl;
	cout << "Damage : " << monsterMap.find("Onnie")->second->GetDamage() << endl;
	delete monsterMap.find("Onnie")->second;

	cout << "Boss ����" << endl;
	cout << "HP : " << monsterMap.find("Boss")->second->GetHP() << endl;
	cout << "Damage : " << monsterMap.find("Boss")->second->GetDamage() << endl;
	delete monsterMap.find("Boss")->second;
	

	delete manager;
}

// ���� ������
Monster::Monster(const char * _name, int _hp, int _damage) : hp(_hp), damage(_damage)
{
	strcpy_s(name, NAME_LENGTH, _name);
	manager = MonsterManager::GetInstance();
}

// ���Ͱ� ������� �޴� �޼ҵ�
int Monster::ReceiveDamage(int _damage)
{
	cout << endl << name << "��(��) " << _damage << " ������� �Ծ���!" << endl;
	hp -= _damage;

	return hp;
}

// ���Ͱ� �÷��̾ �����ϴ� �޼ҵ�
int Monster::Attack(Player * player)
{
	if (player->ReceiveDamage(damage) <= 0)
	{
		cout << "�÷��̾� ���..." << endl;
		return DEAD;
	}

	return LIVE;
}


// �÷��̾� ������
Player::Player(int _hp, int _damage) : hp(_hp), damage(_damage)
{
	manager = MonsterManager::GetInstance();
}

// �÷��̾ �������� �޴� �޼ҵ�
int Player::ReceiveDamage(int _damage)
{
	cout << endl << "�÷��̾ " << _damage << " ������� �Ծ���!" << endl;
	hp -= _damage;

	return hp;
}

// �÷��̾ ���͸� �����ϴ� �޼ҵ�
int Player::Attack(Monster * monster)
{
	// ������ hp�� 0�� �Ǿ��� ���
	if (monster->ReceiveDamage(damage) <= 0)
	{
		manager->RemoveMonster(monster);
		return DEAD;
	}
	return LIVE;
}

MonsterManager::MonsterManager()
{
	instance = this;
	int hp;
	int damage;
	monsterVector = new vector<Monster*>();

	cout << "[���� ����]" << endl;
	cout << "�÷��̾� HP >> ";
	cin >> hp;
	cout << "�÷��̾� Damage >> ";
	cin >> damage;
	cout << endl;
	player = new Player(hp, damage);

	while (1)
	{
		ShowMenu();
		int select;
		cin >> select;

		switch (select)
		{
			case 1:
				CreateMonster();
				break;
			case 2:
				if (FightMonster() == DEAD)
				{
					return;
				}
				break;
			case 0:
				return;
		}
	}
}

// ���� ���� ����
MonsterManager::~MonsterManager()
{
	instance = NULL;
	vector<Monster*>::iterator iter = monsterVector->begin();

	while (iter != monsterVector->end())
	{
		delete *iter;
		iter++;
	}
	delete player;
	delete monsterVector;
}

// �÷��̾� �⺻ �޴� ��� �޼ҵ�
void MonsterManager::ShowMenu()
{
	cout << "[�÷��̾�� ������ �����Ͽ���...]" << endl;
	cout << "1. ���͸� �����Ѵ�." << endl;
	cout << "2. ���Ϳ� �ο��." << endl;
	cout << "0. �ڻ��ϰ� ������ ������." << endl << endl;
	cout << " >> ";
}

// ���� ���Ŀ� ���Լ�
bool CompareMonster(Monster * monster1, Monster * monster2)
{
	return monster1->GetHP() < monster2->GetHP();
}

// ���� ���� �޼ҵ�
void MonsterManager::CreateMonster()
{
	char tempName[NAME_LENGTH];
	int tempHP, tempDamage;
	Monster * newMonster;

	cout << "[�÷��̾ �ɽ��� ������ ���͸� �����...]" << endl;
	cout << "������ �̸� >> ";
	cin >> tempName;
	cout << "������ HP >> ";
	cin >> tempHP;
	cout << "������ Damage >> ";
	cin >> tempDamage;

	newMonster = new Monster(tempName, tempHP, tempDamage);
	monsterVector->push_back(newMonster);
	cout << endl << "[" << tempName << "] HP : " << tempHP << ", Damage : " << tempDamage << "�� �����Ͽ����ϴ�." << endl << endl;

	// HP�� ������� �������� ����
	sort(monsterVector->begin(), monsterVector->end(), CompareMonster);
}

// ���Ϳ� �ο�� �޼ҵ�
int MonsterManager::FightMonster()
{
	if (monsterVector->empty())
	{
		cout << endl << "���͸� ������ ���߽��ϴ�..." << endl << endl;
		return LIVE;
	}

	Monster * monster = *(monsterVector->begin());

	cout << endl << "[���� �߰�]" << endl;

	while (1)
	{
		cout << endl << "�̸� : " << monster->GetName() << endl;
		cout << "HP : " << monster->GetHP() << endl;
		cout << "Damage : " << monster->GetDamage() << endl;
		cout << endl << "1. �����Ѵ�." << endl;
		cout << "2. ����ģ��." << endl;
		cout << " >> ";
		int select;
		int monsterState = 0, playerState = 0;
		cin >> select;

		switch (select)
		{
		case 1:
			monsterState = player->Attack(monster);
			if (monsterState == LIVE)
			{
				playerState = monster->Attack(player);
				if (playerState == DEAD)
				{
					return DEAD;
				}
			}
			else
				return LIVE;
			break;
		case 2:
			return LIVE;
		}
	}
}

// ���� ���Ϳ��� ���͸� �����ϴ� �޼ҵ�
void MonsterManager::RemoveMonster(Monster * monster)
{
	vector<Monster*>::iterator iter = monsterVector->begin();

	for (; iter != monsterVector->end(); iter++)
	{
		if (*iter == monster)
		{
			cout << "���� " << (*iter)->GetName() << "��(��) �׾����ϴ�." << endl;
			delete *iter;
			monsterVector->erase(iter);
			return;
		}
	}

	cout << "���� " << monster->GetName() << "��(��) �߰ߵ��� �ʾҽ��ϴ�." << endl;
}