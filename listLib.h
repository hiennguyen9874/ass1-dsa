/*
* =========================================================================================
* Name        : listLib.h
* Author      : Duc Dung Nguyen
* Email       : nddung@hcmut.edu.vn
* Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
* Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
* =========================================================================================
*/

#ifndef A01_LISTLIB_H
#define A01_LISTLIB_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class DSAException {
	int     _error;
	string  _text;
public:

	DSAException() : _error(0), _text("Success") {}
	DSAException(int err) : _error(err), _text("Unknown Error") {}
	DSAException(int err, const char* text) : _error(err), _text(text) {}

	int getError() { return _error; }
	string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
	T data;
	L1Item<T> *pNext;
	L1Item() : pNext(NULL) {}
	L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List {
	L1Item<T>   *_pHead;// The head pointer of linked list
	L1Item<T>   *_pTail;
	size_t      _size;// number of elements in this list
public:
	L1List() : _pHead(NULL), _size(0) {}
	~L1List() {};

	bool    isEmpty() {
		return _pHead == NULL;
	}
	size_t  getSize() {
		return _size;
	}

	T&      operator[](int i);

	bool    find(T& a, int& idx);

	int     push_back(T& a);
	int     insertHead(T& a);

	int     removeHead();
	int     removeLast();


	void    traverse(void(*op)(T&)) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data);
			p = p->pNext;
		}
	}
	void    traverse(void(*op)(T&, void*), void* pParam) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data, pParam);
			p = p->pNext;
		}
	}

	void removeItiem(T &a);


	L1Item<T> *get_Head()
	{
		return _pHead;
	}

	L1Item<T> *get_Tail()
	{
		return _pTail;
	}

	void Destroy();
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
	if (_pHead == NULL) {
		_pHead = _pTail = new L1Item<T>(a);
	}
	else {
		L1Item<T>   *p = new L1Item<T>(a);
		_pTail->pNext = p;
		_pTail = p;
	}

	_size++;
	return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
	L1Item<T>   *p = new L1Item<T>(a);
	p->pNext = _pHead;
	_pHead = p;
	_size++;
	return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
	if (_pHead) {
		L1Item<T>* p = _pHead;
		_pHead = p->pNext;
		delete p;
		_size--;
		return 0;
	}
	return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
	if (_pHead) {
		if (_pHead->pNext) {
			L1Item<T>* prev = _pHead;
			L1Item<T>* pcur = prev->pNext;
			while (pcur->pNext) {
				prev = pcur;
				pcur = pcur->pNext;
			}
			delete pcur;
			prev->pNext = NULL;
		}
		else {
			delete _pHead;
			_pHead = NULL;
		}
		_size--;
		return 0;
	}
	return -1;
}


template <class T>
T& L1List<T>::operator[](int i)
{
	L1Item<T> *p = _pHead;
	while (p && i > 0)
	{
		p = p->pNext;
		i--;
	}
	return p->data;
}


template <class T>
bool L1List<T>::find(T& a, int& idx)
{
	int idxx = 0;
	L1Item<T> *p = _pHead;
	while (p)
	{
		if (p->data == a)
		{
			idx = idxx;
			return true;
		}
		p = p->pNext;
		idxx++;
	}
	return false;
}


template<class T>
void L1List<T>::Destroy()
{
	while (_pHead)
	{
		L1Item<T>*p = _pHead;
		_pHead = _pHead->pNext;
		delete p;
	}
}


template <class T>
void L1List<T>::removeItiem(T &a)
{
	L1Item<T> *pCur = _pHead,
		*pPre = NULL;
	while (pCur)
	{
		if (pCur->data != a)
		{
			if (pCur->pNext == NULL)
				break;
			else
			{
				pPre = pCur;
				pCur = pCur->pNext;
			}
		}
		else
		{
			if (pCur == _pHead)
			{
				L1Item<T> *p = _pHead;
				_pHead = _pHead->pNext;
				pCur = _pHead;
				_size--;
				delete p;
			}
			else if (pCur->pNext == NULL)
			{
				L1Item<T> *p = pCur;
				_pTail = pPre;
				pPre->pNext = pCur->pNext;
				delete p;
				_size--;
				break;
			}
			else
			{
				L1Item<T> *p = pCur;
				pPre->pNext = pCur->pNext;
				pCur = pPre->pNext;
				delete p;
				_size--;
			}
		}
	}
}

#endif //A01_LISTLIB_H
