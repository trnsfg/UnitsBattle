#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

class Position {
private:
    int x, y;

public:
    Position(int x = 0, int y = 0) : x(x), y(y) {}

    void Move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    void Print() const {
        std::cout << "Position: (" << x << ", " << y << ")" << std::endl;
    }
};

class Weapon {
private:
    char* name;
    int damage;
public:
    Weapon(const char* name, int damage)
        : damage(damage > 0 ? damage : 5)
    {
        int len = std::strlen(name) + 1;
        this->name = new char[len];
        std::strcpy(this->name, name);
    }
    int GetDamage() const
    {
        return damage;
    }
    ~Weapon() {
        delete[] name;
    }
};

class Sword : public Weapon
{
public:
    Sword(const char* name, int damage)
        : Weapon(name, damage)
    {
    }
    ~Sword() {}
};

class Bow : public Weapon
{
public:
    Bow(const char* name, int damage)
        : Weapon(name, damage)
    {
    }
    ~Bow() {}
};

class Unit {
protected:
    char* name;
    int hp;
    Weapon* weapon;
    Position position;

    void Damaged(int value)
    {
        if (hp - value > 0)
        {
            hp -= value;
            std::cout << this->name << " damaged for " << value << " points." << std::endl;
        }
        else
        {
            hp = 0;
            std::cout << this->name << " is killed." << std::endl;
        }
    }

public:
    Unit(const char* name, int hp, Weapon* weapon, Position position = Position())
        : hp(hp > 0 ? hp : 100), weapon(weapon), position(position)
    {
        int len = std::strlen(name) + 1;
        this->name = new char[len];
        std::strcpy(this->name, name);
    }

    int GetHP() const
    {
        return hp;
    }

    const char* GetName() const {
        return name;
    }

    void Attack(Unit& enemy)
    {
        if (&enemy == this) {
            return;
        }
        int damage = weapon->GetDamage();
        enemy.Damaged(damage);
        std::cout << this->name << " attacks " << enemy.name << " for " << damage << " damage." << std::endl;
    }

    void Move(int dx, int dy) {
        position.Move(dx, dy);
        std::cout << this->name << " moved to ";
        position.Print();
    }

    void PrintPosition() const {
        std::cout << this->name << " ";
        position.Print();
    }

    ~Unit() {
        delete[] name;
    }
};

class Archer : public Unit
{
private:
    int range;
    int arrows;
public:
    Archer(const char* name, int hp, int range, int arrows, Bow* bow, Position position = Position())
        : Unit(name, hp, bow, position),
        range(range > 0 ? range : 10),
        arrows(arrows > 0 ? arrows : 10)
    {
    }

    ~Archer() {}
};

class Swordman : public Unit
{
private:
    int power;
public:
    Swordman(const char* name, int hp, int power, Sword* sword, Position position = Position())
        : Unit(name, hp, sword, position),
        power(power)
    {
    }

    ~Swordman() {}
};

class Team
{
private:
    Unit** units;
    int size;

public:
    int RandomTarget(Team& units)
    {
        return rand() % units.size;
    }
    Team() {
        units = nullptr;
        size = 0;
    }
    Team(Unit** units, int size)
    {
        this->units = units;
        this->size = size;
        units = nullptr;
    }
    Team(const Team& other) = delete;
    Team& operator = (const Team& other) = delete;
    void Add(Unit* new_unit)
    {
        Unit** temp = units;
        int new_size = size + 1;
        units = new Unit * [new_size];
        for (int i = 0; i < size; i++)
        {
            units[i] = temp[i];
        }
        units[new_size - 1] = new_unit;
        size = new_size;
        delete[] temp;
    }

    void Attack(Team& enemy_team)
    {
        if (!(size > 0 && enemy_team.size > 0))
        {
            return;
        }
        if (size < enemy_team.size)
        {
            int target;
            for (int i = 0; i < size; i++)
            {
                int target = RandomTarget(enemy_team);
                units[i]->Attack(*enemy_team.units[target]);
            }
        }
    }
};

int main()
{
    srand(time(nullptr));
    Sword Sword1("Sword1", 20);
    Bow Bow1("Bow1", 15);

    Archer archer1("Bob", 100, 20, 200, &Bow1, Position(5, 5));
    Swordman swordman1("Rex", 100, 10, &Sword1, Position(0, 0));

    archer1.PrintPosition();
    swordman1.PrintPosition();

    archer1.Move(2, 3);
    swordman1.Move(-1, 1);

    archer1.Attack(swordman1);
    swordman1.Attack(archer1);

    return 0;
}
