#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>

class Flower;

class CareAction {
public:
    virtual void apply_to(Flower &flower) = 0;
    virtual ~CareAction() = default;
};

class Flower {
protected:
    std::string name;
    std::string color;
    double height;
    double health;
    std::vector<std::unique_ptr<CareAction>> care_plan;

    void clampHealth() {
        if (health > 100) health = 100;
        if (health < 0) health = 0;
    }

public:
    Flower(std::string name, std::string color, double height, double health)
        : name(std::move(name)), color(std::move(color)), height(height), health(health) {}

    virtual ~Flower() = default;

    void addCareAction(CareAction* action) {
        care_plan.emplace_back(action);
    }

    void care() {
        for (auto &action : care_plan) {
            action->apply_to(*this);
        }
    }

    void printInfo() const {
        std::cout << "Flower: " << name << ", Color: " << color
                  << ", Height: " << height << " cm, Health: " << health << "%\n";
    }

    double getHeight() const { return height; }
    void setHeight(double h) { height = h; }

    double getHealth() const { return health; }
    void setHealth(double h) { health = h; clampHealth(); }

    std::string getColor() const { return color; }
    void setColor(const std::string& c) { color = c; }

    std::string getName() const { return name; }
};

class Orchid : public Flower {
public:
    Orchid() : Flower("Orchid", "Purple", 12.0, 65.0) {}
};

class Sunflower : public Flower {
public:
    Sunflower() : Flower("Sunflower", "Yellow", 20.0, 75.0) {}
};

class Lily : public Flower {
public:
    Lily() : Flower("Lily", "White", 18.0, 70.0) {}
};

class Daffodil : public Flower {
public:
    Daffodil() : Flower("Daffodil", "Golden", 16.0, 68.0) {}
};

class Daisy : public Flower {
public:
    Daisy() : Flower("Daisy", "White-Yellow", 14.0, 67.0) {}
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
    explicit Sunlight(int h) : hours(h) {}
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

#define TEST(suiteName, testName) bool suiteName##_##testName()
#define RUN_TEST(suiteName, testName) \
    if (suiteName##_##testName()) std::cout << #suiteName "." #testName " passed\n"; \
    else std::cout << #suiteName "." #testName " failed\n";

#define ASSERT_EQ(a, b) if ((a) != (b)) return false;
#define ASSERT_NEAR(a, b, eps) if (std::fabs((a) - (b)) > (eps)) return false;

TEST(FlowerTest, Initialization) {
    auto flower = Flower("Rose", "Red", 15.0, 70.0);
    flower.addCareAction(new Watering());
    flower.addCareAction(new Fertilizing());
    flower.addCareAction(new Pruning());

    ASSERT_EQ(flower.getName(), "Rose");
    ASSERT_EQ(flower.getHeight(), 15.0);
    ASSERT_EQ(flower.getHealth(), 70.0);
    ASSERT_EQ(flower.getColor(), "Red");
    return true;
}

TEST(FlowerTest, CareEffect) {
    auto flower = Flower("Tulip", "Yellow", 10.0, 60.0);
    flower.addCareAction(new Watering());
    flower.addCareAction(new Fertilizing());
    flower.addCareAction(new Pruning());
    flower.addCareAction(new Sunlight(4));
    flower.addCareAction(new Repotting());
    flower.addCareAction(new Aromatizing());
    flower.addCareAction(new Misting());

    flower.care();

    ASSERT_NEAR(flower.getHeight(), 10.0 + 2 + 5 + 1 + 0.5, 0.001);
    ASSERT_NEAR(flower.getHealth(), 100.0, 0.001);
    ASSERT_EQ(flower.getColor(), "Aromatic Refreshed Brighter Yellow");
    return true;
}

TEST(FlowerTest, ColorChangeSequence) {
    auto flower = Flower("ColorSeq", "White", 10.0, 50.0);
    flower.addCareAction(new Aromatizing());
    flower.addCareAction(new Pruning());
    flower.addCareAction(new Repotting());
    flower.care();

    ASSERT_EQ(flower.getColor(), "Refreshed Brighter Aromatic White");
    return true;
}

TEST(FlowerTest, HeightIncreaseOnly) {
    auto flower = Flower("HeightOnly", "Green", 8.0, 40.0);
    flower.addCareAction(new Watering());
    flower.addCareAction(new Fertilizing());
    flower.addCareAction(new Misting());
    flower.care();

    ASSERT_NEAR(flower.getHeight(), 8.0 + 2 + 5 + 0.5, 0.001);
    ASSERT_NEAR(flower.getHealth(), 40.0 + 10 + 5 + 6, 0.001);
    ASSERT_EQ(flower.getColor(), "Green");
    return true;
}

TEST(FlowerTest, HealthClampUpper) {
    auto flower = Flower("ClampUp", "Blue", 5.0, 95.0);
    flower.addCareAction(new Watering());
    flower.addCareAction(new Fertilizing());
    flower.addCareAction(new Repotting());
    flower.care();

    ASSERT_NEAR(flower.getHealth(), 100.0, 0.001);
    return true;
}

TEST(FlowerTest, HealthClampLower) {
    auto flower = Flower("ClampDown", "Orange", 7.0, 5.0);
    class Damage : public CareAction {
    public:
        void apply_to(Flower &flower) override {
            flower.setHealth(flower.getHealth() - 10);
        }
    };
    flower.addCareAction(new Damage());
    flower.care();

    ASSERT_NEAR(flower.getHealth(), 0.0, 0.001);
    return true;
}

TEST(FlowerTest, NoCareActions) {
    auto flower = Flower("NoCare", "Pink", 12.0, 50.0);
    flower.care();

    ASSERT_EQ(flower.getName(), "NoCare");
    ASSERT_EQ(flower.getColor(), "Pink");
    ASSERT_NEAR(flower.getHeight(), 12.0, 0.001);
    ASSERT_NEAR(flower.getHealth(), 50.0, 0.001);
    return true;
}

int main() {
    RUN_TEST(FlowerTest, Initialization);
    RUN_TEST(FlowerTest, CareEffect);
    RUN_TEST(FlowerTest, ColorChangeSequence);
    RUN_TEST(FlowerTest, HeightIncreaseOnly);
    RUN_TEST(FlowerTest, HealthClampUpper);
    RUN_TEST(FlowerTest, HealthClampLower);
    RUN_TEST(FlowerTest, NoCareActions);
    return 0;
}
