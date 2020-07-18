#pragma once

template<typename T>
struct QueueNode;
template <typename T>
class LinkedQueue;

template<typename T>
class LinkedQueueIterator {
	friend class LinkedQueue<T>;
private:
	const LinkedQueue<T>& L_List;
	QueueNode<T>* L_pointer;

public:
	LinkedQueueIterator(const LinkedQueue<T>& queue) : L_List(queue), L_pointer(queue.Q_rear) {}

	/*
	@brief: �����͸� ���� ���� �ٲ۴�.
	@pre: none
	@post: none
	*/
	T Next();

	/*
	@brief: ���� �����Ͱ� ����Ű�� ��带 ��ȯ�Ѵ�.
	@pre: none
	@post: none
	*/
	QueueNode<T> GetCurrentNode();

	/*
	@brief: ���� �����Ͱ� nullptr���� Ȯ��
	@pre: none
	@post: none
	@param: return true if current pointer is not null, otherwise return false
	*/
	bool NotNull();

	/*
	@brief: ���� �����Ͱ� nullptr���� Ȯ��
	@pre: none
	@post: none
	@param: return true if next pointer is not null, otherwise return false
	*/
	bool NextNotNull();

	/*
	@brief: ù �������� ��ȯ�Ѵ�.
	@pre: least one element is already exist
	@post: none
	*/
	T First();
};

template<typename T>
T LinkedQueueIterator<T>::Next() {
	L_pointer = L_pointer->next;
	return L_pointer->value;
}

template<typename T>
QueueNode<T> LinkedQueueIterator<T>::GetCurrentNode() {
	return *L_pointer;
}

template<typename T>
bool LinkedQueueIterator<T> ::NotNull() {
	if (L_pointer == nullptr) {
		return false;
	}
	else {
		return true;
	}
}

template<typename T>
bool LinkedQueueIterator<T> ::NextNotNull() {
	if (L_pointer->next == nullptr) {
		return false;
	}
	else {
		return true;
	}
}

template<typename T>
T LinkedQueueIterator<T> ::First() {
	return L_List.Q_rear->next->data;
}