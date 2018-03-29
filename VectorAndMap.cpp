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

	// 대미지를 받고, hp값을 반환
	int ReceiveDamage(int _damage);
	// 몬스터가 플레이어에게 공격, 죽으면 DEAD, 살면 LIVE반환
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

	// 대미지를 받고, hp값을 반환
	int ReceiveDamage(int _damage);
	// 플레이어가 몬스터에게 공격, 죽으면 DEAD, 살면 LIVE반환
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
	// 메뉴 출현
	void ShowMenu();
	// 몬스터 만드는 메소드
	void CreateMonster();
	// 몬스터와 싸우는 메소드
	int FightMonster();
	// 몬스터를 제거하는 메소드
	void RemoveMonster(Monster * monster);
};
// MonsterManager static 변수 초기화
MonsterManager * MonsterManager::instance = NULL;

int main(void)
{
	MonsterManager * manager = MonsterManager::GetInstance();

	// 맵 사용법
	map<string, Monster*> monsterMap;

	// Map에 3개의 몬스터 정보를 넣는다.
	monsterMap.insert(make_pair("Rabbit", new Monster("Rabbit", 20, 10)));
	monsterMap.insert(make_pair("Onnie", new Monster("Onnie", 50, 20)));
	monsterMap.insert(make_pair("Boss", new Monster("Boss", 100, 40)));

	// Map에 3개의 몬스터 정보를 출력 후 삭제한다.
	cout << "Rabbit 정보" << endl;
	cout << "HP : " << monsterMap.find("Rabbit")->second->GetHP() << endl;
	cout << "Damage : " << monsterMap.find("Rabbit")->second->GetDamage() << endl;
	delete monsterMap.find("Rabbit")->second;

	cout << "Onnie 정보" << endl;
	cout << "HP : " << monsterMap.find("Onnie")->second->GetHP() << endl;
	cout << "Damage : " << monsterMap.find("Onnie")->second->GetDamage() << endl;
	delete monsterMap.find("Onnie")->second;

	cout << "Boss 정보" << endl;
	cout << "HP : " << monsterMap.find("Boss")->second->GetHP() << endl;
	cout << "Damage : " << monsterMap.find("Boss")->second->GetDamage() << endl;
	delete monsterMap.find("Boss")->second;
	

	delete manager;
}

// 몬스터 생성자
Monster::Monster(const char * _name, int _hp, int _damage) : hp(_hp), damage(_damage)
{
	strcpy_s(name, NAME_LENGTH, _name);
	manager = MonsterManager::GetInstance();
}

// 몬스터가 대미지를 받는 메소드
int Monster::ReceiveDamage(int _damage)
{
	cout << endl << name << "이(가) " << _damage << " 대미지를 입었다!" << endl;
	hp -= _damage;

	return hp;
}

// 몬스터가 플레이어를 공격하는 메소드
int Monster::Attack(Player * player)
{
	if (player->ReceiveDamage(damage) <= 0)
	{
		cout << "플레이어 사망..." << endl;
		return DEAD;
	}

	return LIVE;
}


// 플레이어 생성자
Player::Player(int _hp, int _damage) : hp(_hp), damage(_damage)
{
	manager = MonsterManager::GetInstance();
}

// 플레이어가 데미지를 받는 메소드
int Player::ReceiveDamage(int _damage)
{
	cout << endl << "플레이어가 " << _damage << " 대미지를 입었다!" << endl;
	hp -= _damage;

	return hp;
}

// 플레이어가 몬스터를 공격하는 메소드
int Player::Attack(Monster * monster)
{
	// 몬스터의 hp가 0이 되었을 경우
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

	cout << "[게임 실행]" << endl;
	cout << "플레이어 HP >> ";
	cin >> hp;
	cout << "플레이어 Damage >> ";
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

// 몬스터 벡터 삭제
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

// 플레이어 기본 메뉴 출력 메소드
void MonsterManager::ShowMenu()
{
	cout << "[플레이어는 모험을 시작하였다...]" << endl;
	cout << "1. 몬스터를 생성한다." << endl;
	cout << "2. 몬스터와 싸운다." << endl;
	cout << "0. 자살하고 게임을 끝낸다." << endl << endl;
	cout << " >> ";
}

// 몬스터 정렬용 비교함수
bool CompareMonster(Monster * monster1, Monster * monster2)
{
	return monster1->GetHP() < monster2->GetHP();
}

// 몬스터 생성 메소드
void MonsterManager::CreateMonster()
{
	char tempName[NAME_LENGTH];
	int tempHP, tempDamage;
	Monster * newMonster;

	cout << "[플레이어가 심심한 나머지 몬스터를 만든다...]" << endl;
	cout << "몬스터의 이름 >> ";
	cin >> tempName;
	cout << "몬스터의 HP >> ";
	cin >> tempHP;
	cout << "몬스터의 Damage >> ";
	cin >> tempDamage;

	newMonster = new Monster(tempName, tempHP, tempDamage);
	monsterVector->push_back(newMonster);
	cout << endl << "[" << tempName << "] HP : " << tempHP << ", Damage : " << tempDamage << "를 생성하였습니다." << endl << endl;

	// HP를 대상으로 오름차순 정렬
	sort(monsterVector->begin(), monsterVector->end(), CompareMonster);
}

// 몬스터와 싸우는 메소드
int MonsterManager::FightMonster()
{
	if (monsterVector->empty())
	{
		cout << endl << "몬스터를 만나지 못했습니다..." << endl << endl;
		return LIVE;
	}

	Monster * monster = *(monsterVector->begin());

	cout << endl << "[몬스터 발견]" << endl;

	while (1)
	{
		cout << endl << "이름 : " << monster->GetName() << endl;
		cout << "HP : " << monster->GetHP() << endl;
		cout << "Damage : " << monster->GetDamage() << endl;
		cout << endl << "1. 공격한다." << endl;
		cout << "2. 도망친다." << endl;
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

// 몬스터 벡터에서 몬스터를 제거하는 메소드
void MonsterManager::RemoveMonster(Monster * monster)
{
	vector<Monster*>::iterator iter = monsterVector->begin();

	for (; iter != monsterVector->end(); iter++)
	{
		if (*iter == monster)
		{
			cout << "몬스터 " << (*iter)->GetName() << "이(가) 죽었습니다." << endl;
			delete *iter;
			monsterVector->erase(iter);
			return;
		}
	}

	cout << "몬스터 " << monster->GetName() << "이(가) 발견되지 않았습니다." << endl;
}