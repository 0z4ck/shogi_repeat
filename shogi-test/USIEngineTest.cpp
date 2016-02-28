#include <gtest/gtest.h>

#include <iostream>
#include <thread>
#include <mutex>


#include "USIEngine.h"

using namespace std;


TEST(USIEngineTest, test)
{
	void testThread();

	thread th1(testThread);


	th1.join();
}


void testThread()
{
	this_thread::sleep_for(chrono::milliseconds(1000));
	cout << "test thread" << endl;
}

class hoge
{
public:
	hoge() {
		std::cout << "hoge" << std::endl;
	}

	~hoge()
	{
		std::cout << "~hoge" << std::endl;
	}

};

TEST(USIEngineTest, test2)
{
	std::unique_ptr<hoge> foo = std::make_unique<hoge>();

	foo = nullptr; // �����Ŕj��������

	foo = std::make_unique<hoge>();
}


TEST(USIEngineTest, InitTest)
{
	USIEngine engine;

#ifdef _WIN32
	ASSERT_EQ(engine.Initialize("tchild.exe"), true);
#else
	ASSERT_EQ(engine.Initialize("./tchild.out"), true);
#endif
	engine.WriteLine("usi");

	std::string str;
	
	ASSERT_EQ(engine.ReadLine(str, -1), STRING_QUEUE_ERR::OK);
	ASSERT_EQ(engine.ReadLine(str, -1), STRING_QUEUE_ERR::OK);

	engine.WriteLine("quit");
	engine.Terminate();

}

TEST(USIEngineTest, InitTest2)
{
	USIEngine engine;
#ifdef _WIN32
	ASSERT_EQ(engine.Initialize("tchild.exe"), true);
	// quit�𑗂�Ȃ��̂Ń^�C���A�E�g�Ńv���Z�X�����ʁE�E�E�͂�
	engine.Terminate();
#else
	// Linux�ł̓^�C���A�E�g������
#endif
}

TEST(USIEngineTest, InitTest3)
{
	USIEngine engine;

#ifdef _WIN32
	ASSERT_EQ(engine.Initialize("tchild.exe"), true);
#else
	ASSERT_EQ(engine.Initialize("./tchild.out"), true);
#endif
	engine.WriteLine("usi");


	engine.WriteLine("quit");
	engine.Terminate();

}