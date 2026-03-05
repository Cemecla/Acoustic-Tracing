#pragma once
#include "Error.h"

#ifndef QUEUE_H
#define QUEUE_H
namespace Echo {

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

		Queue() : m_back(nullptr), m_front(nullptr) {};

		~Queue() {
			while (!isEmpty())
			{
				pop();
			}
		};

		// Add a Item to the back of the queue
		const void push(const T& item) {
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

		// Remove a Item from the front of the queue
		const T pop() {
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
		// Get the Item at the front of the queue
		const T& front() {
			if (isEmpty()) {
				throw Error::UNDEFINED;
			}
			return m_front->getData();
		};
		// Get the Item at the back of the queue
		const T& back() {
			if (isEmpty()) {
				throw Error::UNDEFINED;
			}
		};

		// Get if a queue is empty
		const bool isEmpty() { return m_front == nullptr; };

		// Get the size of the queue
		const int size();

	
		

		
	};



}

#endif // !QUEUE_H


