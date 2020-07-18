#pragma once

template<typename T>
class LinkedQueueIterator;

template<typename T>
struct QueueNode {
	T value;
	QueueNode* next;
	QueueNode* prev;
};

template <typename T>
class LinkedQueue {
	friend class LinkedQueueIterator<T>;
private:
	int Q_size;									//���� Queue��  ������
	QueueNode<T>* Q_front;						//���� ���� �߰��� ������ �κ�
	QueueNode<T>* Q_rear;						//���� ���߿� �߰��� ������ �κ�

public:
	LinkedQueue() {						//������
		Q_front = new QueueNode<T>;
		Q_rear = new QueueNode<T>;

		Q_rear->next = Q_front;				//�հ� ���� node�� ������ ����, ���θ� ����Ű�� ��
		Q_rear->prev = nullptr;
		Q_front->prev = Q_rear;
		Q_front->next = nullptr;
		Q_size = 0;
	}
	~LinkedQueue() {					//�Ҹ���
		MakeEmpty();
		delete Q_front;
		delete Q_rear;
	}

	/*
	@brief: Queue�� ������ �߰�
	@pre: set data for add
	@post: Queue�� rear�� �����Ͱ� �߰���
	*/
	int EnQueue(const T& data);

	/*
	@brief: Queue���� ������ ���� �� ��ȯ
	@pre: Queue is not empty
	@post: Queue�� front ��Ҹ� reference�� ��ȯ �� ����
	*/
	int Dequeue(T& data);

	/*
	@brief: Queue�� ������ return
	@pre: none
	@post: none
	@return: return size of queue
	*/
	int GetSize();

	/*
	@brief: queue�� ����ִ��� ����
	@pre: none
	@post: none
	@return: if queue is empty, return 1, else return 0
	*/
	bool IsEmpty();

	/*
	@brief: Queue�� rear data�� ��ȯ 
	@pre: queue is not empty
	@post: queue�� rear data�� reference�� ��ȯ
	*/
	int Rear(T& data);

	/*
	@brief: Queue�� front data�� ��ȯ
	@pre: queue is not empty
	@post: queue�� front data�� reference�� ��ȯ
	*/
	int Front(T& data);

	/*
	@brief: Queue�� ��� ����
	@pre: none
	@post: Queue�� ��ҵ��� ������
	*/
	void MakeEmpty();

	/*
	@brief: �ش� �����͸� ã��
	@pre: data is exist
	@post: �ش��ϴ� �����͸� ã�� reference�� ��ȯ��
	@param: if find, return 1, otherwise return 0
	*/
	int Get(T& data);

	/*
	@brief: �ش� �����͸� ������
	@pre: data is exist
	@post: �ش��ϴ� �����͸� �����ϰ� reference�� ��ȯ��
	@param: if delete, return 1, otherwise return 0
	*/
	int Delete(T& data);

	/*
	@brief: �ش� ����� ���� ��ü��
	@pre: data is exist
	@post: �ش��ϴ� �����͸� ã�� value���� ������
	@param: if replace, return 1, otherwise return 0
	*/
	int Replace(const T& data);
};

template<typename T>
int LinkedQueue<T>::EnQueue(const T& data) {
	QueueNode<T>* temp = new QueueNode<T>;
	temp->value = data;
	temp->next = Q_rear->next;
	temp->prev = Q_rear;
	Q_rear->next->prev = temp;
	Q_rear->next = temp;
	Q_size++;
	return 1;
}

template<typename T>
int LinkedQueue<T>::Dequeue(T& data) {
	if (IsEmpty()) {
		return 0;
	}
	QueueNode<T>* temp = new QueueNode<T>;
	temp = Q_front->prev;
	data = temp->value;
	temp->prev->next = Q_front;
	Q_front->prev = temp->prev;
	delete temp;
	Q_size--;
	return 1;
}

template<typename T>
int LinkedQueue<T>::GetSize() {
	return Q_size;
}

template<typename T>
bool LinkedQueue<T>::IsEmpty() {
	if (Q_size == 0) {
		return true;
	}
	else {
		return false;
	}
}

template<typename T>
int LinkedQueue<T>::Rear(T& data) {
	if (IsEmpty()) {
		return 0;
	}
	QueueNode<T>* temp = new QueueNode<T>;
	temp = Q_rear->next;
	data = temp->value;
	return 1;
}

template<typename T>
int LinkedQueue<T>::Front(T& data) {
	if (IsEmpty()) {
		return 0;
	}
	QueueNode<T>* temp = new QueueNode<T>;
	temp = Q_front->prev;
	data = temp->value;
	return 1;
}

template<typename T>
void LinkedQueue<T>::MakeEmpty() {
	QueueNode<T>* pItem;
	LinkedQueueIterator<T> itor(*this);
	itor.Next(); // �������� �̵�.

	while (itor.NextNotNull())
	{
		pItem = itor.L_pointer;
		itor.Next(); // ���� �����͸� �������� �̵�.
		delete pItem;
	}

	Q_rear->next = Q_front;
	Q_rear->prev = nullptr;
	Q_front->prev = Q_rear;
	Q_front->next = nullptr;

	return;
}

template<typename T>
int LinkedQueue<T>::Get(T& data) {
	if (IsEmpty()) {
		return 0;
	}
	LinkedQueueIterator<T> itor(*this);
	QueueNode<T>* temp = new QueueNode<T>;
	itor.Next();
	while (itor.NextNotNull()) {
		temp = itor.L_pointer;
		if (temp->value == data) {
			data = temp->value;
			return 1;
		}
		itor.Next();
	}
	return 0;
}

template<typename T>
int LinkedQueue<T>::Delete(T& data) {
	if (IsEmpty()) {
		return 0;
	}
	LinkedQueueIterator<T> itor(*this);
	QueueNode<T>* temp = new QueueNode<T>;
	itor.Next();
	while (itor.NextNotNull()) {
		temp = itor.L_pointer;
		if (temp->value == data) {
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			data = temp->value;
			delete temp;
			Q_size--;
			return 1;
		}
		itor.Next();
	}
	return 0;
}

template<typename T>
int LinkedQueue<T>::Replace(const T& data) {
	if (IsEmpty()) {
		return 0;
	}
	LinkedQueueIterator<T> itor(*this);
	QueueNode<T>* temp = new QueueNode<T>;
	itor.Next();
	while (itor.NextNotNull()) {
		temp = itor.L_pointer;
		if (temp->value == data) {
			temp->value = data;
			return 1;
		}
		itor.Next();
	}
	return 0;
}