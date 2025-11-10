#include <gtest.h>
#include <utility>

#define count 1000
template <class T>
class louvainFixture :public ::testing::Test {
public:
	using Tkey = typename T::first_type;
	using Tvalue = typename T::second_type;
	
protected:
	int COUNT = 20;
	void SetUp() override {
	}
};

using MyTypes = ::testing::Types<std::pair<int, double>>;
TYPED_TEST_CASE(louvainFixture, MyTypes);

TEST(louvainAlgorithm, can_insert_one_element)
{
	EXPECT_EQ(0, 0);
}
T