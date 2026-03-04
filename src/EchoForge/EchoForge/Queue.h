#pragma once
#ifndef QUEUE_H
#define QUEUE_H
namespace Echo {

	template<class T>
	class Item {
	private:
		T data;
		Item* next;

	public:
		Item(T data);
		~Item();


	};

	template <class T>
	class QUEUE
	{

	private:
		Item<T>* back;
		Item<T>* front;

	public:
		QUEUE();
		~QUEUE();

		// Add a Item to the back of the queue
		const void push(T item);
		// Remove a Item from the front of the queue
		const T& pop();
		// Get the Item at the front of the queue
		const T& front();
		// Get the Item at the back of the queue
		const T& back();

		// Get if a queue is empty
		const bool empty();

		// Get the size of the queue
		const int size();

	private:
		const T getData(Item item);

		
	};



}

#endif // !QUEUE_H


