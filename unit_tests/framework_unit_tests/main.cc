#include <iostream>
#include <vector>
#include <memory>


#include "gtest/gtest.h"
/*#include "framework/callback.h"
#include "framework/argtype.h"
#include "framework/meta.h"
#include "framework/notifications.h"
#include "framework/work_items.h"
#include "framework/workers.h"
#include "framework/work_queue.h"
#include "framework/worker_pool.h"*/
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
	virtual void do_work_impl() override {
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << "dum dum done" << std::endl;
	}
};

TEST(Worker, WorkerTest) {
	using WorkQueue = sd::framework::WorkQueue;
	std::shared_ptr<WorkQueue> work_queue = std::make_shared<WorkQueue>();
	std::shared_ptr<DummyWorkItem> work_item = std::make_shared<DummyWorkItem>(); std::shared_ptr<DummyWorkItem> work_item2 = std::make_shared<DummyWorkItem>();
	std::shared_ptr<sd::framework::workers::Worker> worker = std::make_shared<sd::framework::workers::Worker>(work_queue);
	std::shared_ptr<sd::framework::workers::Worker> worker2 = std::make_shared<sd::framework::workers::Worker>(work_queue);
	std::shared_ptr<sd::framework::work_items::ShutdownWorker> shutdown_worker = std::make_shared<sd::framework::work_items::ShutdownWorker>(worker);
	std::shared_ptr<sd::framework::work_items::ShutdownWorker> shutdown_worker2 = std::make_shared<sd::framework::work_items::ShutdownWorker>(worker2);
	worker->start();
	worker2->start();
	work_queue->push_work(work_item);
	work_queue->push_work(work_item);
	work_queue->push_work(work_item2);
	work_queue->push_work(shutdown_worker);
	work_queue->push_work(shutdown_worker2);
	worker->join();
	worker2->join();
	EXPECT_TRUE(work_item->complete());
	EXPECT_TRUE(work_item->complete());
	EXPECT_TRUE(work_queue->empty());
}

TEST(Worker, DelayedWorkItem) {
	using WorkQueue = sd::framework::WorkQueue;
	std::shared_ptr<WorkQueue> work_queue = std::make_shared<WorkQueue>();
}

TEST(Meta, HasTypeMember) {
	EXPECT_TRUE(sd::framework::meta::has_type_member<sd::framework::notifications::WorkCompleted>::value);
}
TEST(Meta, IsWorkerNotification) {
	auto same = std::is_same<sd::framework::notifications::WorkCompleted::type, sd::framework::notifications::WorkerNotification>::value;
	EXPECT_TRUE(same);
	EXPECT_FALSE(sd::framework::meta::is_worker_notification<int>::value);
	EXPECT_TRUE(sd::framework::meta::is_worker_notification<sd::framework::notifications::WorkerNotification>::value);
	EXPECT_TRUE(sd::framework::meta::is_worker_notification<sd::framework::notifications::WorkCompleted>::value);
	EXPECT_TRUE(sd::framework::meta::is_worker_notification<sd::framework::notifications::WorkStarted>::value);
	EXPECT_FALSE(sd::framework::meta::is_worker_notification<sd::framework::WorkerPool>::value);
	
}

TEST(WorkerPool, WorkerPoolTest) {
	auto hardware_concurrency = std::thread::hardware_concurrency();
	int worker_count = hardware_concurrency ? hardware_concurrency : 2;
	std::cout << "worker count: " << worker_count << std::endl;
	sd::framework::WorkerPool worker_pool(worker_count);
	std::shared_ptr<DummyWorkItem> work_item = std::make_shared<DummyWorkItem>(); std::shared_ptr<DummyWorkItem> work_item2 = std::make_shared<DummyWorkItem>();
	worker_pool.push_work(work_item);
	worker_pool.push_work(work_item2);
	worker_pool.shutdown();
	EXPECT_TRUE(worker_pool.queued_work_item_count() == 0);
}

TEST(WorkerPool, WorkerPoolTest2) {
	auto hardware_concurrency = std::thread::hardware_concurrency();
	int worker_count = hardware_concurrency ? hardware_concurrency : 2;
	std::cout << "worker count: " << worker_count << std::endl;
	sd::framework::WorkerPool worker_pool(worker_count);
	worker_pool.shutdown();
	EXPECT_TRUE(worker_pool.queued_work_item_count() == 0);
}

TEST(WorkerPool, WorkerPoolTest3) {
	auto hardware_concurrency = std::thread::hardware_concurrency();
	int worker_count = hardware_concurrency ? hardware_concurrency : 2;
	std::cout << "worker count: " << worker_count << std::endl;
	sd::framework::WorkerPool worker_pool(worker_count);
	std::shared_ptr<DummyWorkItem> work_item = std::make_shared<DummyWorkItem>();
	worker_pool.push_work(work_item);
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	worker_pool.shutdown();
	EXPECT_TRUE(worker_pool.queued_work_item_count() == 0);
}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}