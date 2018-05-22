typedef struct node {
	int data;
	node *next;
}NODE;

class LinkList
{
private:
	NODE * head;
public:
	LinkList() { head = nullptr; }
	~LinkList();
	bool isEmpty() { return head == nullptr; }
	int Length();
	bool GetElem(int i, int *e);
	int LocateElem(int e);
	bool Insert(int i, int e);
	bool Delete(int i, int *e);
	NODE * Reverse();
};
