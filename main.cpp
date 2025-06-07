#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "test.h"

class Flower;

class CareAction {
public:
    virtual void apply_to(Flower &flower) = 0;
    virtual ~CareAction() = default;
};

class Flower {
    std::string name;
    double height;
    double health;
    std::string color;
    std::vector<CareAction *> care_plan;

    void clampHealth() {
        if (health > 100) health = 100;
        if (health < 0) health = 0;
    }

public:
    Flower(const std::string &_name, double _height, double _health, const std::string &_color, const std::vector<CareAction *> &_plan)
        : name(_name), height(_height), health(_health), color(_color), care_plan(_plan) {}

    virtual ~Flower() {
        for (auto action : care_plan) {
            delete action;
        }
    }

    void care() {
        for (auto action : care_plan) {
            action->apply_to(*this);
        }
    }

    const std::string &getName() const { return name; }
    double getHeight() const { return height; }
    void setHeight(double h) { height = h; }
    double getHealth() const { return health; }
    void setHealth(double h) { health = h; clampHealth(); }
    const std::string &getColor() const { return color; }
    void setColor(const std::string &c) { color = c; }
};

class Watering : public CareAction {
public:
    void apply_to(Flower &flower) override {
        flower.setHealth(flower.getHealth() + 10);
        flower.setHeight(flower.getHeight() + 2);
    }
};

class Fertilizing : public CareAction {
public:
    void apply_to(Flower &flower) override {
        flower.setHeight(flower.getHeight() + 5);
        flower.setHealth(flower.getHealth() + 5);
    }
};

class Pruning : public CareAction {
public:
    void apply_to(Flower &flower) override {
        flower.setHealth(flower.getHealth() + 8);
        flower.setColor("Brighter " + flower.getColor());
    }
};

class Sunlight : public CareAction {
    int hours;
public:
    Sunlight(int h) : hours(h) {}
    void apply_to(Flower &flower) override {
        double health_boost = hours * 1.5;
        flower.setHealth(flower.getHealth() + health_boost);
    }
};

class Repotting : public CareAction {
public:
    void apply_to(Flower &flower) override {
        flower.setHealth(flower.getHealth() + 12);
        flower.setHeight(flower.getHeight() + 1);
        flower.setColor("Refreshed " + flower.getColor());
    }
};

class Aromatizing : public CareAction {
public:
    void apply_to(Flower &flower) override {
        flower.setHealth(flower.getHealth() + 7);
        flower.setColor("Aromatic " + flower.getColor());
    }
};

class Misting : public CareAction {
public:
    void apply_to(Flower &flower) override {
        flower.setHealth(flower.getHealth() + 6);
        flower.setHeight(flower.getHeight() + 0.5);
    }
};

class Orchid : public Flower {
public:
    Orchid(const std::vector<CareAction *> &plan)
        : Flower("Orchid", 12.0, 65.0, "Purple", plan) {}
};

class Sunflower : public Flower {
public:
    Sunflower(const std::vector<CareAction *> &plan)
        : Flower("Sunflower", 20.0, 75.0, "Yellow", plan) {}
};

class Lily : public Flower {
public:
    Lily(const std::vector<CareAction *> &plan)
        : Flower("Lily", 18.0, 70.0, "White", plan) {}
};

class Daffodil : public Flower {
public:
    Daffodil(const std::vector<CareAction *> &plan)
        : Flower("Daffodil", 16.0, 68.0, "Golden", plan) {}
};

class Daisy : public Flower {
public:
    Daisy(const std::vector<CareAction *> &plan)
        : Flower("Daisy", 14.0, 67.0, "White-Yellow", plan) {}
};

TEST(FlowerTest, Initialization) {
    std::vector<CareAction *> plan;
    plan.push_back(new Watering());
    plan.push_back(new Fertilizing());
    plan.push_back(new Pruning());

    Flower flower("Rose", 15.0, 70.0, "Red", plan);

    ASSERT_EQ(flower.getName(), "Rose");
    ASSERT_EQ(flower.getHeight(), 15.0);
    ASSERT_EQ(flower.getHealth(), 70.0);
    ASSERT_EQ(flower.getColor(), "Red");
    return true;
}

TEST(FlowerTest, CareEffect) {
    std::vector<CareAction *> plan;
    plan.push_back(new Watering());
    plan.push_back(new Fertilizing());
    plan.push_back(new Pruning());
    plan.push_back(new Sunlight(4));
    plan.push_back(new Repotting());
    plan.push_back(new Aromatizing());
    plan.push_back(new Misting());

    Flower flower("Tulip", 10.0, 60.0, "Yellow", plan);
    flower.care();

    ASSERT_NEAR(flower.getHeight(), 10.0 + 2 + 5 + 1 + 0.5, 0.001);
    ASSERT_NEAR(flower.getHealth(), 100.0, 0.001);
    ASSERT_EQ(flower.getColor(), "Aromatic Refreshed Brighter Yellow");
    return true;
}

int main() {
    RUN_TEST(FlowerTest, Initialization);
    RUN_TEST(FlowerTest, CareEffect);
    return 0;
}
