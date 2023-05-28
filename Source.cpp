#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::random_device seed;
std::default_random_engine e(seed());
std::normal_distribution<float> determineHealth(30, 5);
std::normal_distribution<float> determineStrength(5, 1);

std::string fileName{ "monsters.txt" };

struct Object
{
	std::string name;
	int strength{ std::max(5, (int)determineStrength(e))};
	int health{ std::max(10, (int)determineHealth(e)) };
	bool isDead{ false };
	//add comment
};

void loadMonsters(std::vector<Object>& enemies);
void displayBattle(Object& player, std::vector<Object>& enemies);
int attack(Object& object);
void defend(Object& object, const int& damage);
void playerAttack(Object& player, std::vector<Object>& enemies, int& deadMonsters);
void monsterAttack(Object& player, std::vector<Object>& enemies);
void heal(Object& object);

int main()
{
	Object player;
	std::cout << "Choose your name: ";
	std::getline(std::cin, player.name);
	
	std::vector<Object> monsters;

	loadMonsters(monsters);

	std::cout << "Prepare for a fight!\n";
	system("Pause");
	system("CLS");

	int deathCounter{ 0 };
	while (player.health > 0 && deathCounter < monsters.size())
	{
		displayBattle(player, monsters);
		playerAttack(player, monsters, deathCounter);
		monsterAttack(player, monsters);
		system("Pause");
		system("CLS");
	}

	if (player.health < 0)
	{
		std::cout << "The monsters bested you. A tragedy...\n";
	}
	else if (deathCounter >= monsters.size())
	{
		std::cout << "You overcame the monsters!\n";
	}
	else if (player.health < 0 && deathCounter >= monsters.size())
	{
		std::cout << "It was an epic battle, yet somehow you and the creatures destroyed eachother. Such will be your legacy.\n";
	}
}

void loadMonsters(std::vector<Object>& enemies)
{
	std::ifstream fin{ fileName };

	if (!fin)
	{
		std::cerr << "Something went wrong when trying to open the file!\n";
		return;
	}

	int monsterCount{};
	fin >> monsterCount;

	for (int i{ 1 }; i <= monsterCount; ++i)
	{
		Object monster;
		fin >> monster.name;
		fin >> monster.strength;
		fin >> monster.health;
		enemies.push_back(monster);

	}
}

void displayBattle(Object& player, std::vector<Object>& enemies)
{
	std::cout << player.name << ": " << player.health << "\n\n";

	for (Object& eachEnemy : enemies)
	{
		if (!eachEnemy.isDead)
		{
			std::cout << eachEnemy.name << ": " << eachEnemy.health << '\n';
		}
		else if (eachEnemy.isDead)
		{
			std::cout << "<DEAD>\n";
		}
	}
	std::cout << std::endl;
}

int attack(Object& object)
{
	std::normal_distribution<double> calculateDamage((double)object.strength, 2.0f);
	int damageDealt{ std::max(1, (int)calculateDamage(e)) };

	std::cout << object.name << " deals ";
	return damageDealt;
}

void defend(Object& object, const int& damage)
{
	object.health -= damage;
	std::cout << damage << " damage to " << object.name << "!!!" << std::endl;
}

void playerAttack(Object& player, std::vector<Object>& enemies, int& deadMonsters)
{
	bool isChoosing{ true };
	//Check to see if all monsters are dead
	deadMonsters = 0;
	for (Object& eachMon : enemies)
	{
		if (eachMon.isDead)
		{
			deadMonsters++;
		}
	}
	if (deadMonsters >= enemies.size())
	{
		return;
	}

	while (isChoosing)
	{
		std::cout << "Would you like to (a)ttack or (h)eal? ";
		char choice{ 'x' };
		std::cin >> choice;

		switch (choice)
		{
		case 'A':
		case 'a':
		{
			std::cout << "Choose a monster to attack: ";
			int monIndex{ -1 };
			bool inRange{ false };
			while (!inRange)
			{
				//monIndex = -1;
				std::cin >> monIndex;
				if (monIndex < 0 || monIndex > enemies.size())
				{
					std::cout << "No monster exist there! Choose again: ";
				}
				else if (!isdigit(monIndex))
				{
					std::cout << "Input a number please: ";
					std::cin.clear();
				}
				else if (enemies.at(monIndex - 1).isDead)
				{
					std::cout << "This monster is dead. Choose another! ";
				}
				else
				{
					inRange = true;
				}
			}
			defend(enemies.at(monIndex - 1), attack(player));

			if (enemies.at(monIndex - 1).health < 1)
			{
				std::cout << enemies.at(monIndex - 1).name << " has been slain!\n";
				enemies.at(monIndex - 1).isDead = true;
			}
			isChoosing = false;
			break;
		}

		case 'H':
		case 'h':
		{
			heal(player);
			isChoosing = false;
			break;
		}
		default:
			std::cout << "Choose a valid input.\n";
			break;
		}
	}
}

void monsterAttack(Object& player, std::vector<Object>& enemies)
{
	std::bernoulli_distribution hitChance(0.75);
	for (Object& eachMon : enemies)
	{
		if (!eachMon.isDead)
		{
			bool hitPlayer{ hitChance(e) };
			if (hitPlayer)
			{
				defend(player, attack(eachMon));
			}
			else
			{
				std::cout << eachMon.name << " missed!\n";
			}
		}
	}
}

void heal(Object& object)
{
	std::normal_distribution<double> healAmount((object.strength * 2), 3);
	int healed{ std::max(3, (int)healAmount(e)) };
	object.health += healed;
	std::cout << object.name << " heals by " << healed << "!\n";
}