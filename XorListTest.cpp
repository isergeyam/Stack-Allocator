#include "XorList.hpp"
#include <gtest/gtest.h>
#include <iterator>
#include <list>
#include <random>
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
TEST(XorListTest, PushBackandPopBack) {
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
TEST(XorListTest, PushFrontandPopFront) {
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
  while (!mlist.empty())
    mlist.pop_front();
  ASSERT_EQ(mlist.size(), 0);
}
class XorListTestGeneral : public ::testing::TestWithParam<int> {};
TEST_P(XorListTestGeneral, General) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> num_distr(std::numeric_limits<int>::min());
  std::uniform_int_distribution<int> op_distr(0, 3);
  XorList<int> mlist1;
  std::list<int> mlist2;
  enum OperationTp { PushBack, PushFront, PopBack, PopFront };
  for (int i = 0; i < GetParam(); ++i) {
    OperationTp op = static_cast<OperationTp>(op_distr(gen));
    if (mlist2.empty() && (op == PopBack || op == PopFront)) {
      --i;
      continue;
    }
    switch (op) {
    case PushBack: {
      int cur = num_distr(gen);
      mlist1.push_back(cur);
      mlist2.push_back(cur);
      break;
    }
    case PushFront: {
      int cur = num_distr(gen);
      mlist1.push_front(cur);
      mlist2.push_front(cur);
      break;
    }
    case PopBack: {
      mlist1.pop_back();
      mlist2.pop_back();
      break;
    }
    case PopFront: {
      mlist1.pop_front();
      mlist2.pop_front();
      break;
    }
    }
  }
  ASSERT_TRUE(std::equal(mlist1.begin(), mlist1.end(), mlist2.begin()));
}
INSTANTIATE_TEST_CASE_P(INST_HUMAN_SWORDSMAN, XorListTestGeneral,
                        ::testing::Values(10000));
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
