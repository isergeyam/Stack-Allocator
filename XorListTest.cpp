#include "XorList.hpp"
#include <gtest/gtest.h>
#include <iterator>
using __sg_lib::XorList;
TEST(XorListTest, DefaultConstructor) {
  XorList<int> mlist;
  ASSERT_TRUE(mlist.empty());
}
/*void print_list(XorList<int> &mlist) {
  for (auto it = mlist.begin(); it != mlist.end(); ++it)
    std::cout << *it << " ";
  std::cout << std::endl;
}*/
TEST(XorListTest, PushBack) {
  XorList<int> mlist;
  for (int i = 0; i < 10; ++i) {
    mlist.push_back(i);
    // print_list(mlist);
  }
  for (auto it = mlist.begin(); it != mlist.end(); ++it) {
    static int i = 0;
    ASSERT_EQ(*it, i);
    ++i;
  }
}
TEST(XorListTest, PushFront) {
  XorList<int> mlist;
  for (int i = 0; i < 10; ++i) {
    if (i % 2)
      mlist.push_back(i);
    else
      mlist.push_front(i);
  }
  std::vector<int> test_vec{8, 6, 4, 2, 0, 1, 3, 5, 7, 9};
  for (auto it = mlist.begin(); it != mlist.end(); ++it) {
    static int i = 0;
    ASSERT_EQ(test_vec[i], *it);
    ++i;
  }
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
