#include "linklist.h"
#include<iostream>

//析构函数
LinkList::~LinkList()//和清空一样
{
	NODE *p = head;
	while (head)
	{
		p = head;
		head = head->next;
		delete(p);
	}
}
//获取链表长度
int LinkList::Length()
{
	NODE *p = head;
	int len = 0;
	while (p != nullptr)
	{
		len++;
		p = p->next;
	}
	return len;
}
//获取指定位置的元素
bool LinkList::GetElem(int i, int *e)
//*e是返回的元素
{
	NODE *p = head;
	int j = 0;
	while (p&&j < i)
	{
		p = p->next;
		j++;
	}
	if (p == nullptr) return false;
	*e = p->data;
	return true;
}
//查找元素e在链表什么位置（下标位置，从0开始）
int LinkList::LocateElem(int e)
{
	int i = 0;
	NODE *p = head;
	while (p != nullptr)
	{
		if (p->data == e)
			return i;
		else 
			p = p->next;
		i++;
	}
	std::cout << "表中不存在指定元素" << std::endl;
	return -1;
}
//在指定位置插入元素e
bool LinkList::Insert(int i, int e)
{
	NODE *p = head, *s;
	int j = 0;
	if (i == 0)
	{
		s = (NODE *)new NODE[1];
		s->data = e;
		s->next = p;
		head = s;
		return true;
	}
	while (p&&j < i - 1)
	{
		p = p->next;
		j++;
	}
	if (p == NULL)
		return false;//到队尾了
	s = (NODE *)new NODE[1];
	s->data = e;
	//注意顺序
	s->next = p->next;
	p->next = s;
	return true;
}
//删除指定位置的元素，并把删除的元素赋给*e
bool LinkList::Delete(int i, int *e)
{
	NODE *p = head, *s;
	if (p == NULL) return false;
	int j = 0;
	if (i == 0)
	{
		head = head->next;
		*e = p->data;
		delete p;
		//指针释放后置空
		p = NULL;
		return true;
	}
	while (p&&j < i - 1)
	{
		j++;
		p = p->next;
	}
	if (p == NULL)
		return false;
	s = p->next;
	p->next = p->next->next;
	*e = s->data;
	delete s;
	s = NULL;
	return true;
}
