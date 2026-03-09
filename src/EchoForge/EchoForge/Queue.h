#pragma once
#include "Utils.h"


#ifndef QUEUE_H
#define QUEUE_H
namespace ef {

	template<typename T>
	class Item {
	private:
		T m_data;

	public:
		Item* m_next;

		Item(T data) : m_data(data), m_next(nullptr) {};

		~Item() {

		};
		T getData() {
			return m_data;
		}


	};

	template <typename T>
	class Queue
	{

	private:
		Item<T>* m_back;
		Item<T>* m_front;
		//const T getData(Item item) {};

	public:

		Queue();

		~Queue();

		// Add a Item to the back of the queue
		const void push(const T& item);

		// Remove a Item from the front of the queue
		const T pop();


		// Get the Item at the front of the queue
		const T& front();


		// Get the Item at the back of the queue
		const T& back();

		// Get if a queue is empty
		const bool isEmpty();

		// Get the size of the queue
		const int size();

	
		

		
	};



	template<typename T>
	Queue<T>::Queue() : m_back(nullptr), m_front(nullptr) {};


	template<typename T>
	Queue<T>::~Queue() {
		while (!isEmpty())
		{
			pop();
		}
	};


	template<typename T>
	const void Queue<T>::push(const T& item) {
		Item<T>* it = new Item<T>(item);
		if (isEmpty()) {
			m_front = it;
			m_back = it;
		}
		else
		{
			m_back->m_next = it;
			m_back = it;
		}

	};



	template<typename T>
	const T Queue<T>::pop() {
		if (m_front == nullptr) {
			throw Error::UNDEFINED;
		}

		Item<T>* temp = m_front;

		m_front = m_front->m_next;
		if (m_front == nullptr) {
			m_back = nullptr;
		}
		else {
		}
		T value = temp->getData();
		delete temp;
		return value;

	};

	template<typename T>
	const T& Queue<T>::front() {
		if (isEmpty()) {
			throw Error::UNDEFINED;
		}
		return m_front->getData();
	};


	template<typename T>
	const T& Queue<T>::back() {
		if (isEmpty()) {
			throw Error::UNDEFINED;
		}
	};

	template<typename T>
	const bool Queue<T>::isEmpty() { return m_front == nullptr; };

	template<typename T>
	const int Queue<T>::size() {
		int count = 0;

		Item<T>* curr = m_front;
		while (curr != nullptr)
		{
			count++;
			curr = curr->m_next;

		}

		return count;
	}

}

#endif // !QUEUE_H

