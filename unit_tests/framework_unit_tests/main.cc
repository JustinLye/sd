#include <iostream>
#include <vector>
#include <memory>


#include "gtest/gtest.h"
#include "framework/framework.h"

struct cb_args {
	int a;
	int b;
};

static int add(const cb_args& args) {
	return args.a + args.b;
}

class CallBackTest :
	public ::testing::Test {};



TEST_F(CallBackTest, TrivalTest) {
	auto args = cb_args{ 2,2 };
	auto cb = sd::framework::Callback<int, cb_args>();
	cb.set_callback(add);
	EXPECT_EQ(4, cb(args));
}

TEST_F(CallBackTest, TrivalTest2) {
	auto args = cb_args{ 2, 2 };
	auto cb = sd::framework::Callback<int, cb_args>(add);
	EXPECT_NE(2, cb(args));
}

TEST_F(CallBackTest, TrivalTest3) {
	std::vector<sd::framework::Callback<int, cb_args>> cb_vec;
	auto cb_count = 3;
	for (auto i = 0; i < cb_count; ++i)
		cb_vec.push_back(sd::framework::Callback<int, cb_args>(add));
	auto acc_ttl = 0;
	auto args = cb_args{ 2,2 };
	for (auto i : cb_vec)
		acc_ttl += i(args);
	EXPECT_EQ(cb_count * 4, acc_ttl);

}

class ArgTypeTest :
	public ::testing::Test {
public:
	sd::framework::argtype::argtype ArgType;
};



struct Args {
	static const sd::framework::argtype::argtype ArgType = sd::framework::argtype::argtype::INT_ARG;
};

#include <type_traits>
template <typename T, typename = int>
struct HasArgType : std::false_type {};

template <typename T>
struct HasArgType <T, decltype((void)T::ArgType, 0)> : std::true_type {};

#include <map>

class Callbacks {
public:
	template<class T,
		typename std::enable_if<HasArgType<T>::value, T>::type* = nullptr>
		void add_callback(T& callback) {

	}
};

template <typename T, typename = int>
struct GetArgType {
	static const sd::framework::argtype::argtype value = sd::framework::argtype::argtype::INVALID;
};

template <typename T>
struct GetArgType <T, decltype((void)T::ArgType, 0)> {
	static const sd::framework::argtype::argtype value = T::ArgType;
};

struct IntArgs {
	static const sd::framework::argtype::argtype ArgType = sd::framework::argtype::argtype::INT_ARG;
};

TEST_F(ArgTypeTest, TrivalTest) {
	EXPECT_TRUE(HasArgType<ArgTypeTest>::value);
}

TEST_F(ArgTypeTest, TrivalTest2) {
	EXPECT_TRUE(HasArgType<Args>::value);
	EXPECT_EQ(Args::ArgType, sd::framework::argtype::argtype::INT_ARG);
}

TEST_F(ArgTypeTest, TrivalTest3) {
	EXPECT_EQ(GetArgType<IntArgs>::value, sd::framework::argtype::argtype::INT_ARG);
}

class DummyWorkItem :
	public sd::framework::work_items::BaseWorkItem {
protected:
	void do_work_impl() override {
		std::cout << "dummy work item" << std::endl;
	}
public:
	DummyWorkItem() :
		sd::framework::work_items::BaseWorkItem() {}
};

TEST(WorkerPool, WorkQueue) {
	using namespace sd::framework;
	auto queue = std::shared_ptr<containers::WorkQueue>(new containers::WorkQueue());
	sd::framework::workers::Worker worker(queue);
	worker.start();
	auto work_item = std::shared_ptr<work_items::NoOp>(new work_items::NoOp());
	auto work_item1 = std::shared_ptr<DummyWorkItem>(new DummyWorkItem());
	auto f = queue->push_work(work_item);
	auto f1 = queue->push_work(work_item1);
	f.wait();
	f1.wait();
	EXPECT_EQ(f.get(), work_items::ItemStatusCode::kComplete);
	EXPECT_EQ(f1.get(), work_items::ItemStatusCode::kComplete);
	EXPECT_TRUE(queue->empty());
	worker.stop();
	queue->unblock();
	worker.join();


}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}