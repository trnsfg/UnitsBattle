#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <cstring>
#include <cmath>

class Position {
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

    int Distance(const Position& other) const {
        return static_cast<int>(std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2)));
    }

    void MoveTowards(const Position& target) {
        if (x < target.x) x++;
        else if (x > target.x) x--;

        if (y < target.y) y++;
        else if (y > target.y) y--;
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

class Sword : public Weapon {
public:
    Sword(const char* name, int damage)
        : Weapon(name, damage)
    {
    }
    ~Sword() {}
};

class Bow : public Weapon {
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

    void Damaged(int value) {
        if (hp - value > 0) {
            hp -= value;
            std::cout << this->name << " damaged for " << value << " points." << std::endl;
        }
        else {
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

    int GetHP() const {
        return hp;
    }

    const char* GetName() const {
        return name;
    }

    Position& GetPosition() {
        return position;
    }

    void Attack(Unit& enemy) {
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

    void MoveTowards(Unit& enemy) {
        position.MoveTowards(enemy.GetPosition());
        std::cout << this->name << " moves towards " << enemy.GetName() << " to ";
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


class Archer : public Unit {
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

class Swordman : public Unit {
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

int main() {
    srand(time(nullptr));

    Sword sword1("Sword1", 20);
    Bow bow1("Bow1", 15);

    Archer archer1("Bob", 100, 20, 200, &bow1, Position(5, 5));
    Swordman swordman1("Rex", 100, 10, &sword1, Position(0, 0));

    archer1.PrintPosition();
    swordman1.PrintPosition();

    int distance = archer1.GetPosition().Distance(swordman1.GetPosition());
    std::cout << "Distance between аrcher and swordman: " << distance << std::endl;

    while (distance > 0) {
        swordman1.MoveTowards(archer1);
        distance = archer1.GetPosition().Distance(swordman1.GetPosition());
    }

    archer1.Attack(swordman1);
    swordman1.Attack(archer1);

    return 0;
}
