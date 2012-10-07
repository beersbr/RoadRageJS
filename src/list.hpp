#ifndef GLIST_HPP
#define GLIST_HPP

#include <string>

template <class T>
class Node
{
public:
	Node()
	{
		this->prev = NULL;
		this->next = NULL;
		hash = "";
	}

	~Node()
	{
		if(this->prev)
		{
			delete this->prev;
			this->prev = NULL;
		}

		if(this->next)
		{
			delete this->next;
			this->next = NULL;
		}

		hash = "";
	}

	T* entity;

private:
	Node<T>* next;
	Node<T>* prev;

	std::string hash;
};

template <class T>
class List
{
public:
	List()
	{
		skip_ptr = NULL;
		head = NULL;
		tail = NULL;
		length = 0;
		cache_size = 1;
	}

	~List()
	{
		length = 0;
		// TODO: iterate over and clean up nodes.
	}

	int push(T* item)
	{
		if(item == NULL)
		{
			return -1;
		}

		if(head = NULL)
		{
			skip_ptr = new Node<T>();
			skip_ptr->entity = &item;

			head = skip_ptr;
			tail = skip_ptr;
			length = 1;
			return length;
		}

		skip_ptr = new Node<T>();
		skip_ptr->entity = item;

		tail->next = skip_ptr;
		tail = tail->next;
		length += 1;
		return length;
	}

	int insert(T* item, int pos = 0)
	{
		if(item == NULL)
		{
			return -1;
		}

		if(pos == 0)
		{
			// TODO: use the data wrapper
			skip_ptr = head;
			head = item;
			head->next = skip_ptr;
		}

		if(pos > length)
		{
			skip_ptr = tail;
			tail = item;
		}
	}

	T* pop()
	{
		if(length == 0)
		{
			return NULL;
		}

		skip_ptr = tail;
		tail = tail->prev;
		tail->next = NULL;
		length -= 1;

		return skip_ptr;
	}

	T* remove(int pos = 0);

	T* get(T* entity);

private:
	int length;

	int cache_size;

	Node<T>* cache;
	Node<T>* skip_ptr;
	Node<T>* head;
	Node<T>* tail;
};

#endif