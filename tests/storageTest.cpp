#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <deferred.h>

template <typename T>
struct MockStorage : storage_base<MockStorage<T>> {
    MOCK_METHOD(void, setImpl, (T), ());
    MOCK_METHOD(T&, getImpl, (), ());
    MOCK_METHOD(const T&, getImpl, (), (const));

    static bool destructCalled;
    static void destructImpl(MockStorage* self) noexcept
    {
        destructCalled = true;
    }
};
template <typename T>
bool MockStorage<T>::destructCalled;

TEST(StorageBaseTest, set_CallsImpl)
{
    MockStorage<int> mock;
    EXPECT_CALL(mock, setImpl(42));

    mock.set(42);
}

TEST(StorageBaseTest, get_CallsImpl)
{
    MockStorage<int> mock;
    int value = 10;
    EXPECT_CALL(mock, getImpl())
            .WillOnce(::testing::ReturnRef(value));

    int& ref = mock.get();
    EXPECT_EQ(ref, 10);
}

TEST(StorageBaseTest, get_const_CallsImpl)
{
    const MockStorage<int> mock;
    int value = 20;
    EXPECT_CALL(mock, getImpl())
            .WillOnce(::testing::ReturnRef(value));

    const int& ref = mock.get();
    EXPECT_EQ(ref, 20);
}

TEST(StorageBaseTest, DestructorCalls_destructImpl)
{
    MockStorage<int>::destructCalled = false;
    {
        MockStorage<int> mock;
        EXPECT_FALSE(MockStorage<int>::destructCalled);
    }

    EXPECT_TRUE(MockStorage<int>::destructCalled);
}
