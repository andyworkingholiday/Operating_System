#ifndef _DOUBLYITERATOR_H
#define _DOUBLYITERATOR_H


template<typename T>
struct DoublyNode;
template<typename T>
class DoublyLinkedList;

/*
*	연결리스트에서 쓰이는 Iterator의 클래스.
*/
template <typename T>
class DoublyIterator
{
	friend class DoublyLinkedList<T>;
public:
	/**
	*	디폴트 생성자.
	*/
	DoublyIterator(const DoublyLinkedList<T>& list) : m_List(list), m_pCurPointer(list.m_pFirst)
	{}; ///< DoublySortedLinkedList와 친구 클래스.


	/*
	@brief: 현재 포인터가 nullptr인지 확인
	@pre: none
	@post: none
	@param: return true if current pointer is not null, otherwise return false
	*/
	bool NotNull();

	/*
	@brief: 다음 포인터가 nullptr인지 확인
	@pre: none
	@post: none
	@param: return true if next pointer is not null, otherwise return false
	*/
	bool NextNotNull();

	/*
	@brief: 첫 아이템을 반환한다.
	@pre: least one element is already exist
	@post: none
	*/
	T First();

	/*
	@brief: 포인터를 다음 노드로 바꾼다.
	@pre: none
	@post: none
	*/
	T Next();

	/*
	@brief: 현재 포인터가 가르키는 노드를 반환한다.
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