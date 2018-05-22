//https://www.cnblogs.com/chengjundu/p/8473564.html  C++常用设计模式
#include<iostream>
using namespace std;
//工厂
//1、简单工厂:需要在工厂类中做判断，从而创造相应的产品
//当增加新产品时，需要修改工厂类。
class Tank
{
public:
	virtual void message() = 0;
};

class Tank80 :public Tank
{
public:
	void message()
	{
		cout << "Tank80" << endl;
	}
};

class Tank99 :public Tank
{
public:
	void message()
	{
		cout << "Tank99" << endl;
	}
};

class TankFactory
{
public:
	Tank * createTank(int type)
	{
		switch (type)
		{
		case 1:
			return new Tank80();
		case 2:
			return new Tank99();
		default:
			return NULL;
		}
	}
};

//2、工厂方法模式：定义一个创建对象的接口，让子类决定实例化哪一个类，Factory Method使一个类的实例化延迟到其子类。
//每增加一种产品，就需要增加一个对象工厂。相比简单工厂模式，工厂方法模式需要定义更多的类
class TankFactory1
{
public:
	virtual Tank* createTank() = 0;
};

class Tank80Factory :public TankFactory1
{
public:
	Tank * createTank()
	{
		return new Tank80();
	}
};

class Tank99Factory :public TankFactory1
{
public:
	Tank * createTank()
	{
		return new Tank99();
	}
};

//3、抽象工厂：在一个工厂里聚合多个同类产品。
//产品族扩展非常困难，要增加一个系列的某一产品，既要在抽象的 Creator 里加代码，又要在具体的里面加代码。
class Plain
{
public:
	virtual void message() = 0;
};

class Plain80 : public Plain
{
public:
	void message()
	{
		cout << "Plain80" << endl;
	}
};

class Plain99 : public Plain
{
public:
	void message()
	{
		cout << "Plain99" << endl;
	}
};

class Factory
{
public:
	virtual Tank* createTank() = 0;
	virtual Plain* createPlain() = 0;
};

class Factory80 :public Factory
{
public:
	Tank * createTank()
	{
		return new Tank80();
	}
	Plain* createPlain()
	{
		return new Plain80();
	}
};

class Factory99 :public Factory
{
public:
	Tank * createTank()
	{
		return new Tank99();
	}
	Plain* createPlain()
	{
		return new Plain99();
	}
};
