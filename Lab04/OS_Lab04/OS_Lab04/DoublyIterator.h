#ifndef _DOUBLYITERATOR_H
#define _DOUBLYITERATOR_H


template<typename T>
struct DoublyNode;
template<typename T>
class DoublyLinkedList;

/*
*	���Ḯ��Ʈ���� ���̴� Iterator�� Ŭ����.
*/
template <typename T>
class DoublyIterator
{
	friend class DoublyLinkedList<T>;
public:
	/**
	*	����Ʈ ������.
	*/
	DoublyIterator(const DoublyLinkedList<T>& list) : m_List(list), m_pCurPointer(list.m_pFirst)
	{}; ///< DoublySortedLinkedList�� ģ�� Ŭ����.


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
	DoublyNode<T> GetCurrentNode();
private:
	const DoublyLinkedList<T>& m_List;
	DoublyNode<T>* m_pCurPointer;
};

template <typename T>
bool DoublyIterator<T> ::NotNull()
{
	if (m_pCurPointer == NULL)
		return false;
	else
		return true;
}

template <typename T>
bool DoublyIterator<T> ::NextNotNull()
{
	if (m_pCurPointer->next == NULL)
		return false;
	else
		return true;
}

template <typename T>
T DoublyIterator<T> ::First()
{
	return m_List.m_pFirst->data;
}

template <typename T>
T DoublyIterator<T> ::Next()
{
	m_pCurPointer = m_pCurPointer->next;
	return m_pCurPointer->data;
}

template <typename T>
DoublyNode<T> DoublyIterator<T> ::GetCurrentNode()
{
	return *m_pCurPointer;
}

#endif _DOUBLYITERATOR_H