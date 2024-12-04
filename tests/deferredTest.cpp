#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <deferred.h>

TEST(DeferredTest, SizeCheck)
{
    EXPECT_TRUE(sizeof(deferred<uint8_t>) == sizeof(uint8_t));
    EXPECT_TRUE(sizeof(deferred<uint16_t>) == sizeof(uint16_t));
    EXPECT_TRUE(sizeof(deferred<uint32_t>) == sizeof(uint32_t));
    EXPECT_TRUE(sizeof(deferred<uint64_t>) == sizeof(uint64_t));

    EXPECT_TRUE(sizeof(deferred<float>) == sizeof(float));
    EXPECT_TRUE(sizeof(deferred<double>) == sizeof(double));

    EXPECT_TRUE(sizeof(deferred<char[42]>) == sizeof(char[42]));
}

TEST(DeferredTest, Access)
{
    deferred<std::string> obj { "Hello" };
    EXPECT_EQ(*obj, "Hello");
    EXPECT_EQ(obj->size(), 5);
    EXPECT_EQ(obj.get()[0], 'H');

    std::string& ref = obj;
    ref += " World";
    EXPECT_EQ(obj->size(), 11);
}

TEST(DeferredTest, Assign)
{
    deferred<std::string> obj { "Hello" };
    EXPECT_EQ(*obj, "Hello");

    obj = std::string("World");
    EXPECT_EQ(*obj, "World");
}


struct NoDefaultCtor {
    int value;

    NoDefaultCtor() = delete;
    NoDefaultCtor(int v) : value(v) {}
};

TEST(DeferredNoDefaultCtorTest, MustBeCreated)
{
    deferred<NoDefaultCtor> obj;
}

TEST(DeferredNoDefaultCtorTest, ValueMustBeAssigned)
{
    deferred<NoDefaultCtor> obj;

    obj = NoDefaultCtor(42);
    EXPECT_EQ(obj->value, 42);
}
