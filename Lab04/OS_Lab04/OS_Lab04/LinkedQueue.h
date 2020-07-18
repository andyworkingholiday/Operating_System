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
	int Q_size;									//현재 Queue의  사이즈
	QueueNode<T>* Q_front;						//제일 먼저 추가된 데이터 부분
	QueueNode<T>* Q_rear;						//제일 나중에 추가된 데이터 부분

public:
	LinkedQueue() {						//생성자
		Q_front = new QueueNode<T>;
		Q_rear = new QueueNode<T>;

		Q_rear->next = Q_front;				//앞과 꼬리 node를 설정해 놓고, 서로를 가르키게 함
		Q_rear->prev = nullptr;
		Q_front->prev = Q_rear;
		Q_front->next = nullptr;
		Q_size = 0;
	}
	~LinkedQueue() {					//소멸자
		MakeEmpty();
		delete Q_front;
		delete Q_rear;
	}

	/*
	@brief: Queue에 데이터 추가
	@pre: set data for add
	@post: Queue의 rear에 데이터가 추가됨
	*/
	int EnQueue(const T& data);

	/*
	@brief: Queue에서 데이터 삭제 후 반환
	@pre: Queue is not empty
	@post: Queue의 front 요소를 reference로 반환 후 삭제
	*/
	int Dequeue(T& data);

	/*
	@brief: Queue의 사이즈 return
	@pre: none
	@post: none
	@return: return size of queue
	*/
	int GetSize();

	/*
	@brief: queue가 비어있는지 결정
	@pre: none
	@post: none
	@return: if queue is empty, return 1, else return 0
	*/
	bool IsEmpty();

	/*
	@brief: Queue의 rear data를 반환 
	@pre: queue is not empty
	@post: queue의 rear data가 reference로 반환
	*/
	int Rear(T& data);

	/*
	@brief: Queue의 front data를 반환
	@pre: queue is not empty
	@post: queue의 front data가 reference로 반환
	*/
	int Front(T& data);

	/*
	@brief: Queue를 비게 만듦
	@pre: none
	@post: Queue의 요소들이 삭제됨
	*/
	void MakeEmpty();

	/*
	@brief: 해당 데이터를 찾음
	@pre: data is exist
	@post: 해당하는 데이터를 찾고 reference로 반환함
	@param: if find, return 1, otherwise return 0
	*/
	int Get(T& data);

	/*
	@brief: 해당 데이터를 삭제함
	@pre: data is exist
	@post: 해당하는 데이터를 삭제하고 reference로 반환함
	@param: if delete, return 1, otherwise return 0
	*/
	int Delete(T& data);

	/*
	@brief: 해당 요소의 값을 교체함
	@pre: data is exist
	@post: 해당하는 데이터를 찾고 value값을 갱신함
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
	itor.Next(); // 다음으로 이동.

	while (itor.NextNotNull())
	{
		pItem = itor.L_pointer;
		itor.Next(); // 현재 포인터를 다음으로 이동.
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