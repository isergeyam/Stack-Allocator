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
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
