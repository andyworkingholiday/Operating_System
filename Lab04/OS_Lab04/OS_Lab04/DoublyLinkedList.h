#ifndef _DOUBLYLINKEDLIST_H
#define _DOUBLYLINKEDLIST_H

template<typename T>
class DoublyIterator;

/**
*	���Ḯ��Ʈ���� ���̴� NodeType�� ����ü
*/
template <typename T>
struct DoublyNode
{
	T data; ///< �� ����� ������.
	DoublyNode* prev; ///< ����� ���� ����Ű�� ������.
	DoublyNode* next; ///< ����� ������ ����Ű�� ������.
};

/**
*	���ĵ� �����Ḯ��Ʈ Ŭ����
*/
template <typename T>
class DoublyLinkedList
{
	friend class DoublyIterator<T>; ///< DoublyIterator�� ģ�� Ŭ����.
public:
	DoublyLinkedList();
	~DoublyLinkedList();

	/*
	@brief	����Ʈ�� ������� �ƴ��� �˻��Ѵ�.
	@pre	m_nLength�� ���� ������ �־�� �Ѵ�.
	@post	������� �ƴ����� �˷��ش�.
	@return	��������� true, �ƴϸ� false�� ��ȯ.
	*/
	bool IsEmpty();

	/*
	@brief	����Ʈ�� ����.
	@pre	����.
	@post	m_pFirst�� m_pLast�� ������ ��� �����͸� �����.
	*/
	void MakeEmpty();

	/*
	@brief	����Ʈ�� ���̸� ��ȯ�Ѵ�.
	@pre	����.
	@post	����Ʈ�� ���̸� ��ȯ.
	@return	m_nLength ����Ʈ�� ���� ��ȯ.
	*/
	int GetLength() const;

	/*
	@brief	���ο� �������� ����Ʈ�� �߰��Ѵ�.
	@pre	item�� �Է¹޴´�.
	@post	�������� ����Ʈ�� �߰��Ѵ�.
	@return	���� �������� ������ 0�� ��ȯ�ϰ�, �Է¿� �����ϸ� 1�� ��ȯ.
	*/
	int Add(T& item);

	/*
	@brief	�Է¹��� �������� ����Ʈ���� �����Ѵ�.
	@pre	item�� �Է¹޴´�.
	@post	�Է¹��� �������� ����Ʈ���� ã�� �����Ѵ�.
	*/
	int Delete(T& item);

	/*
	@brief	�Է¹��� ���������� ������ �ٲ۴�.
	@pre	item�� �Է¹޴´�.
	@post	���ο� ������ ��ü�ȴ�.
	*/
	int Replace(const T& item);

	/*
	@brief	�Է¹��� �������� ������ ���Ͽ� ���� ����Ʈ�� �������� �����´�.
	@pre	item�� �Է¹޴´�.
	@post	�˻��� �����͸� �����ۿ� �ִ´�.
	@return	��ġ�ϴ� �����͸� ã���� 1, �׷��� ������ 0�� ��ȯ.
	*/
	int Get(T& item);

	T pop_back();


private:
	DoublyNode<T>* m_pFirst; ///< �ּҰ��� ������ ����Ʈ�� �� ó��.
	DoublyNode<T>* m_pLast; ///< �ִ��� ������ ����Ʈ�� �� ��.
	int m_nLength; ///< ����Ʈ�� ����.
};

// ����Ʈ�� �� ó���� ���� ���θ� ����Ű�� �����ϰ� ����Ʈ�� ���̸� 0���� �ʱ�ȭ���ѳ��´�.
template <typename T>
DoublyLinkedList<T> ::DoublyLinkedList()
{
	m_pFirst = new DoublyNode<T>;
	m_pLast = new DoublyNode<T>;

	m_pFirst->next = m_pLast; // ���� ó���� ���θ� ����Ű�� �ʱ�ȭ.
	m_pFirst->prev = NULL; // ó��.

	m_pLast->next = NULL; // ��.
	m_pLast->prev = m_pFirst; // ���� ó���� ���θ� ����Ű�� �ʱ�ȭ.

	m_nLength = 0; // ���̴� 0.
}

// �Ҹ���.
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	MakeEmpty();
	delete m_pFirst;
	delete m_pLast;
}

// ����Ʈ�� ������� �˻��Ѵ�.
template <typename T>
bool DoublyLinkedList<T>::IsEmpty()
{
	if (m_nLength == 0)
		return true;
	else
		return false;
}

// ����Ʈ�� ����.
template <typename T>
void DoublyLinkedList<T>::MakeEmpty()
{
	DoublyNode<T>* pItem;
	DoublyIterator<T> itor(*this);
	itor.Next(); // �������� �̵�.

	while (itor.NextNotNull())
	{
		pItem = itor.m_pCurPointer;
		itor.Next(); // ���� �����͸� �������� �̵�.
		delete pItem;
	}

	m_pFirst->next = m_pLast;
	m_pFirst->prev = NULL;
	m_pLast->prev = m_pFirst;
	m_pLast->next = NULL;

	return;
}

// ����Ʈ�� ���̸� ��ȯ�Ѵ�.
template <typename T>
int DoublyLinkedList<T>::GetLength() const
{
	return m_nLength;
}

// �������� �Է¹޾� ����Ʈ�� �´� �ڸ��� ã�� �����Ѵ�.
template <typename T>
int DoublyLinkedList<T>::Add(T& item)
{
	DoublyIterator<T> itor(*this);
	itor.Next(); // �������� �̵�.

	if (IsEmpty()) // ó�� ������ ��
	{
		DoublyNode<T>* pItem = new DoublyNode<T>;
		pItem->data = item;
		m_pFirst->next = pItem;
		pItem->prev = m_pFirst;
		pItem->next = m_pLast;
		m_pLast->prev = pItem; // ó���� �� ���̿� ����.
		m_nLength++;
		return 1;
	}
	else // ó���� �ƴ� ��
	{
		DoublyNode<T>* pItem = new DoublyNode<T>;
		pItem->data = item;
		pItem->next = m_pLast;
		pItem->prev = m_pLast->prev;
		m_pLast->prev->next = pItem;
		m_pLast->prev = pItem;
		m_nLength++;
		return 1;
	}
}

// �Է¹��� �������� �����Ϳ��� ã�Ƴ��� �����Ѵ�.
template <typename T>
int DoublyLinkedList<T>::Delete(T& item)
{
	DoublyIterator<T> itor(*this);
	itor.Next(); // �������� �̵�.

	while (itor.m_pCurPointer != m_pLast)
	{
		if (itor.m_pCurPointer->data == item) // ��ġ�ϴ� �����͸� ã�´�.
		{
			DoublyNode<T>* pItem = new DoublyNode<T>;
			pItem = itor.m_pCurPointer;
			itor.Next();
			pItem->prev->next = itor.m_pCurPointer;
			itor.m_pCurPointer->prev = pItem->prev; // �����ϴ� ����� �հ� �ڸ� ���� �̾��ش�.
			delete pItem; // ����.
			m_nLength--; // ���� ����.
			return 1;
		}
		else
			itor.Next();
	}
	return 0;
}

// �Է¹��� �������� ������ ��ü�Ѵ�.
template <typename T>
int DoublyLinkedList<T>::Replace(const T& item)
{
	DoublyIterator<T> itor(*this);
	itor.Next(); // �������� �̵�.

	while (itor.m_pCurPointer != m_pLast)
	{
		if (itor.m_pCurPointer->data == item)
		{
			itor.m_pCurPointer->data = item; // ������ ��ü.
			return 1;
		}
		else
			itor.Next();
	}

	return 0;
}

// �Է¹��� �����۰� ��ġ�ϴ� �������� ����Ʈ���� ã�� �����´�.
template <typename T>
int DoublyLinkedList<T>::Get(T& item)
{
	DoublyIterator<T> itor(*this);
	itor.Next();

	while (itor.m_pCurPointer != m_pLast)
	{
		if (itor.m_pCurPointer->data == item)
		{
			item = itor.m_pCurPointer->data;
			return 1; // ��ġ�ϸ� 1�� ��ȯ.
		}
		else
			itor.Next(); // �������� �̵�.
	}

	return 0; // ��ġ���� ������ 0�� ��ȯ.
}

template<typename T>
T DoublyLinkedList<T>::pop_back()
{
	if (IsEmpty()) {
		return 0;
	}
	T temp;

	DoublyNode<T>* pItem = new DoublyNode<T>;
	pItem = m_pLast->prev;
	temp = pItem->data;
	m_pLast->prev->prev->next = m_pLast;
	m_pLast->prev = m_pLast->prev->prev;
	delete pItem; // ����.
	m_nLength--; // ���� ����.
	return temp;
}

#endif