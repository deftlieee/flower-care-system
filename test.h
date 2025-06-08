#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <cmath>

#define ASSERT_EQ(val1, val2) \
    if ((val1) != (val2)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": error: Expected " << #val1 << " == " << #val2 << " (" << (val1) << " vs " << (val2) << ")\n"; \
        return false; \
    }

#define ASSERT_NEAR(val1, val2, eps) \
    if (std::abs((val1) - (val2)) > (eps)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": error: Expected " << #val1 << " ≈ " << #val2 << " (" << (val1) << " vs " << (val2) << ") with eps=" << eps << "\n"; \
        return false; \
    }

#define ASSERT_TRUE(val) \
    if (!(val)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": error: Expected " << #val << " to be true\n"; \
        return false; \
    }

#define TEST(suite_name, test_name) \
    bool suite_name##_##test_name()

#define RUN_TEST(suite_name, test_name) \
    if (!suite_name##_##test_name()) { \
        std::cerr << #suite_name "_" #test_name " failed.\n"; \
    } else { \
        std::cout << #suite_name "_" #test_name " passed.\n"; \
    }

#endif 
