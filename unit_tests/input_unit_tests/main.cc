#include <vector>

#include "gtest/gtest.h"
#include "input/mouse.h"
#include "GLFW/glfw3.h"

class EmptyHistory :
  public ::testing::Test {
protected:
  virtual ~EmptyHistory() {}
  using MouseClickHistory = sd::input::mouse::MouseClickHistory;
  using MouseClickInfo = sd::input::mouse::MouseClickInfo;

  MouseClickHistory click_history;


};

class SingleClickHistory :
  public EmptyHistory {
protected:
  virtual ~SingleClickHistory() {}

  MouseClickInfo click_info;
  virtual void add_click_info(size_t count = 1) {
    for (size_t index = 0; index < count; ++index)
      click_history.push_back(click_info);
  }
  
  virtual void SetUp() override {
    add_click_info();
  }
};

class FullHistory :
  public SingleClickHistory {
protected:
  virtual ~FullHistory() {}

  virtual void SetUp() override {
    add_click_info(sd::input::mouse::MouseClickHistory::kMaxHistoryCapacity);
  }
};

class PartialHistory :
  public SingleClickHistory {
protected:
  MouseClickInfo click_info_1;
  MouseClickInfo click_info_2;
  virtual void SetUp() override {
    click_info_1.button = GLFW_MOUSE_BUTTON_LEFT;
    click_info_1.is_press = true;
    click_info_2.button = GLFW_MOUSE_BUTTON_MIDDLE;
    for (size_t index = 0; index < 10; ++index) {
      if (index % 2 == 0)
        add_click_info(click_info_1);
      else
        add_click_info(click_info_2);
    }
  }

  virtual void add_click_info(const MouseClickInfo& info, size_t count = 1) {
    for (size_t index = 0; index < count; ++index)
      click_history.push_back(info);
  }
};

class VarietyHistory :
  public SingleClickHistory {
protected:
  MouseClickInfo left_click(bool is_press = false) {
    MouseClickInfo info;
    info.button = GLFW_MOUSE_BUTTON_LEFT;
    info.is_press = is_press;
    return info;
  }
  MouseClickInfo right_click(bool is_press = false) {
    MouseClickInfo info;
    info.button = GLFW_MOUSE_BUTTON_RIGHT;
    info.is_press = is_press;
    return info;
  }
  MouseClickInfo middle_click(bool is_press = false) {
    MouseClickInfo info;
    info.button = GLFW_MOUSE_BUTTON_MIDDLE;
    info.is_press = is_press;
    return info;
  }

  virtual void add_clicks(const std::vector<MouseClickInfo>& clicks) {
    for (const auto& click : clicks)
      click_history.push_back(click);
  }

  virtual void SetUp() override {
  }
};

TEST_F(EmptyHistory, InitializationTest) {
  EXPECT_TRUE(click_history.empty());
  EXPECT_FALSE(click_history.full());
  EXPECT_EQ(static_cast<size_t>(0), click_history.size());
}

TEST_F(EmptyHistory, ClearTest) {
  click_history.clear();
  EXPECT_TRUE(click_history.empty());
  EXPECT_FALSE(click_history.full());
  EXPECT_EQ(static_cast<size_t>(0), click_history.size());
}

TEST_F(SingleClickHistory, InitializationTest) {
  EXPECT_FALSE(click_history.empty());
  EXPECT_FALSE(click_history.full());
  EXPECT_EQ(static_cast<size_t>(1), click_history.size());
}

TEST_F(SingleClickHistory, VectorViewTest) {
  auto view_info = click_history.view(1);
  EXPECT_EQ(1, view_info.size());
  EXPECT_EQ(click_info, view_info[0]);
}

TEST_F(SingleClickHistory, FixedArrayViewTest) {
  MouseClickInfo view_info[1];
  click_history.view(view_info);
  EXPECT_EQ(click_info, view_info[0]);
}

TEST_F(SingleClickHistory, ClearTest) {
  click_history.clear();
  EXPECT_TRUE(click_history.empty());
  EXPECT_FALSE(click_history.full());
  EXPECT_EQ(static_cast<size_t>(0), click_history.size());
}

TEST_F(FullHistory, InitializationTest) {
  EXPECT_FALSE(click_history.empty());
  EXPECT_TRUE(click_history.full());
  EXPECT_EQ(MouseClickHistory::kMaxHistoryCapacity, click_history.size());
}

TEST_F(FullHistory, ClearTest) {
  click_history.clear();
  EXPECT_TRUE(click_history.empty());
  EXPECT_FALSE(click_history.full());
  EXPECT_EQ(static_cast<size_t>(0), click_history.size());
}

TEST_F(FullHistory, VectorViewTest) {
  auto view_info = click_history.view(click_history.size());
  EXPECT_EQ(MouseClickHistory::kMaxHistoryCapacity, view_info.size());
  for (const auto& info : view_info)
    EXPECT_EQ(click_info, info);
}

TEST_F(FullHistory, FixedArrayViewTest) {
  MouseClickInfo view_info[500];
  click_history.view(view_info);
  for (size_t index = 0; index < 500; ++index)
    EXPECT_EQ(click_info, view_info[index]);
}

TEST_F(FullHistory, RandomAccessTest) {
  MouseClickInfo new_click_info;
  new_click_info.button = GLFW_MOUSE_BUTTON_LEFT;
  click_history.push_back(new_click_info);
  EXPECT_EQ(click_history[0], new_click_info);
  EXPECT_EQ(click_history[1], click_info);
  EXPECT_NE(click_history[1], new_click_info);
  click_history.clear();
  click_history.push_back(new_click_info);
  add_click_info(9);
  EXPECT_EQ(10, click_history.size());
  EXPECT_EQ(click_history[click_history.size() - 1], new_click_info);
  EXPECT_EQ(click_history[click_history.size() - 2], click_info);
}

TEST_F(PartialHistory, InitializationTest) {
  EXPECT_FALSE(click_history.empty());
  EXPECT_FALSE(click_history.full());
  EXPECT_EQ(static_cast<size_t>(10), click_history.size());
}

TEST_F(PartialHistory, ViewTest) {
  click_history.clear();
  click_history.push_back(click_info_1); // 0
  click_history.push_back(click_info_2); // 1
  click_history.push_back(click_info_1); // 2
  click_history.push_back(click_info_2); // 3
  click_history.push_back(click_info_1); // 4
  auto view_info = click_history.view(2, 2);
  EXPECT_EQ(2, view_info.size());
  EXPECT_EQ(view_info[0], click_info_1);
  EXPECT_EQ(view_info[1], click_info_2);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}