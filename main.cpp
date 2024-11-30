#define _CRT_SECURE_NO_WARNINGS  
#include <iostream>
#include <cstring>
#include <cmath>
#include <initializer_list>


class IPrint {
public:
    virtual void print() const = 0;
    virtual ~IPrint() = default;
};

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
        : damage(damage > 0 ? damage : 5) {
        int len = std::strlen(name) + 1;
        this->name = new char[len];
        std::strcpy(this->name, name);
    }

    int GetDamage() const {
        return damage;
    }

    ~Weapon() {
        delete[] name;
    }
};

class Sword : public Weapon {
public:
    Sword(const char* name, int damage)
        : Weapon(name, damage) {
    }
};

class Bow : public Weapon {
public:
    Bow(const char* name, int damage)
        : Weapon(name, damage) {
    }
};

class Unit : public IPrint {
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
        : hp(hp > 0 ? hp : 100), weapon(weapon), position(position) {
        int len = std::strlen(name) + 1;
        this->name = new char[len];
        std::strcpy(this->name, name);
    }

    virtual ~Unit() {
        delete[] name;
    }

    Position& GetPosition() {
        return position;
    }

    const char* GetName() const {
        return name;
    }

    int GetHP() const {
        return hp;
    }

    void Attack(Unit& enemy) {
        if (&enemy == this) return;
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

    virtual void print() const override = 0;
};

class Archer : public Unit {
private:
    int range;
    int arrows;

public:
    Archer(const char* name, int hp, int range, int arrows, Bow* bow, Position position = Position())
        : Unit(name, hp, bow, position),
        range(range > 0 ? range : 10),
        arrows(arrows > 0 ? arrows : 10) {
    }

    void print() const override {
        std::cout << "Archer: " << name << ", HP: " << hp
            << ", Range: " << range << ", Arrows: " << arrows << std::endl;
        position.Print();
    }
};

class Swordman : public Unit {
private:
    int power;

public:
    Swordman(const char* name, int hp, int power, Sword* sword, Position position = Position())
        : Unit(name, hp, sword, position),
        power(power) {
    }

    void print() const override {
        std::cout << "Swordman: " << name << ", HP: " << hp << ", Power: " << power << std::endl;
        position.Print();
    }
};

template<typename T>
class Array : public IPrint {
private:
    const size_t GROWTH_FACTOR = 2;
    T* data;
    size_t size;
    size_t capacity;

public:
    Array() : size(0), capacity(10) {
        data = new T[capacity];
    }

    Array(std::initializer_list<T> init_list)
        : size(init_list.size()), capacity(init_list.size()* GROWTH_FACTOR) {
        data = new T[capacity];
        const T* begin = init_list.begin();
        for (size_t i = 0; i < size; i++) {
            data[i] = begin[i];
        }
    }

    ~Array() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (size == capacity) {
            capacity *= GROWTH_FACTOR;
            T* newData = new T[capacity];
            for (size_t i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size++] = value;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    void print() const override {
        for (size_t i = 0; i < size; i++) {
            data[i]->print();
        }
    }

    size_t get_size() const {
        return size;
    }
};

class Team : public IPrint {
private:
    Array<Unit*> units;

public:
    void AddUnit(Unit* unit) {
        units.push_back(unit);
    }

    void print() const override {
        std::cout << "Team:" << std::endl;
        units.print();
    }
};

int main() {
    Sword sword1("Sword1", 20);
    Bow bow1("Bow1", 15);

    Archer* archer1 = new Archer("Bob", 100, 20, 200, &bow1, Position(5, 5));
    Swordman* swordman1 = new Swordman("Rex", 100, 10, &sword1, Position(0, 0));

    Team team;
    team.AddUnit(archer1);
    team.AddUnit(swordman1);

    team.print();

    archer1->Attack(*swordman1);

    delete archer1;
    delete swordman1;

    return 0;
}
