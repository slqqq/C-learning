// 策略模式:是指定义一系列的算法，把它们一个个封装起来，并且使它们可以互相替换。
//使得算法可以独立于使用它的客户而变化，也就是说这些算法所完成的功能是一样的，对外接口是一样的，只是各自现实上存在差异。
//何时使用：一个系统有许多许多类，而区分它们的只是他们直接的行为。
//如何解决：将这些算法封装成一个一个的类，任意地替换。

#include<iostream>

using std::cout;
//传统策略模式实现
class Hurt
{
public:
	virtual void redBuff() = 0;
};

class AdcHurt :public Hurt
{
public:
	void redBuff()
	{
		cout << "Adc hurt" << endl;
	}
};

class ApcHurt :public Hurt
{
public:
	void redBuff()
	{
		cout << "Apc hurt" << endl;
	}
};
//方法1：传入一个指针参数  虚函数基类指针会自动调用相应对象的方法
class Soldier
{
public:
	Soldier(Hurt* hurt) :m_hurt(hurt)
	{
	}
	~Soldier()
	{
	}
	void beInjured()
	{
		m_hurt->redBuff();
	}
private:
	Hurt * m_hurt;
};

//方法2：使用模板类
template <class T>
class Tank
{
public:
	void beInjured()
	{
		m_hurt.redBuff();
	}
private:
	T m_hurt;
};
