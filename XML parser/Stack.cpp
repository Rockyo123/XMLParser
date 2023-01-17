//  Adapated from Frank M. Carrano and Timothy M. Henry.

/** ADT Stack implementation.
    @file Stack.cpp */

#include <iostream>
#include <cstddef>
#include "Stack.hpp"
#include "Node.hpp"

template<class ItemType>
Stack<ItemType>::Stack() : headPtr(nullptr), currentSize(0)
{
	headPtr = nullptr;
}  // end default constructor

template<class ItemType>
Stack<ItemType>::~Stack()
{
	clear();
} 

template<class ItemType>
bool Stack<ItemType>::isEmpty() const
{
	if (currentSize == 0)
	{
		return true;
	}
	return false;
}  // end isEmpty

template<class ItemType>
int Stack<ItemType>::size() const
{
	return currentSize;
}  // end size

template<class ItemType>
bool Stack<ItemType>::push(const ItemType& newItem)
{
	if(isEmpty())
	{
		Node <ItemType>* newNodePtr = new Node<ItemType>();
		newNodePtr -> setItem(newItem);
		newNodePtr -> setNext(headPtr);
		headPtr = newNodePtr;
		newNodePtr = nullptr;
		currentSize++;
		return true;
	}
	else
	{
		Node<ItemType>* curPtr = headPtr;
		Node<ItemType>* newNodePtr = new Node<ItemType>();
		newNodePtr -> setItem(newItem);

		while(curPtr->getNext() != nullptr)
		{
			curPtr = curPtr -> getNext();
		}

		curPtr -> setNext(newNodePtr);
		newNodePtr -> setNext(nullptr);
		newNodePtr = nullptr;
   		curPtr = nullptr;
		currentSize++;
		return true;
	}
	return false;
}  // end push

template<class ItemType>
ItemType Stack<ItemType>::peek() const throw(logic_error)
{
	if(currentSize == 0)
	{
		throw logic_error("stack is empty");
	}
	else
	{
		ItemType returnItem;
		Node<ItemType>* nodePtr = headPtr;
		while(nodePtr->getNext() != nullptr)
		{
			nodePtr = nodePtr -> getNext();
		}
		returnItem = nodePtr ->getItem();
		nodePtr = nullptr;
		return returnItem;
	}
}  // end peek

template<class ItemType>
bool Stack<ItemType>::pop() 
{
	if(currentSize == 1)
	{
		Node<ItemType>* nodetoDeletePtr = headPtr;
   		nodetoDeletePtr = headPtr;
   		headPtr = headPtr->getNext();
  		nodetoDeletePtr->setNext(nullptr);
  		delete nodetoDeletePtr;
  		currentSize--;
  		return true;
	}
	if(currentSize > 1)
	{
		Node<ItemType>* curPtr = headPtr;
		Node<ItemType>* nodeToRemovePtr = new Node<ItemType>();
		for(int i = 1; i < currentSize-1; i++)
		{
			curPtr = curPtr -> getNext();
		}
		nodeToRemovePtr = curPtr->getNext();
   		curPtr -> setNext(nullptr);
    	delete nodeToRemovePtr;
    	nodeToRemovePtr = nullptr;
    	curPtr = nullptr;
    	currentSize--;
    	return true;
	}
	return false;
}  // end pop

template<class ItemType>
void Stack<ItemType>::clear()
{
	Node<ItemType>* nodeToDeletePtr = headPtr;
	while(headPtr != nullptr)
	{
		nodeToDeletePtr = headPtr;
		headPtr = headPtr -> getNext();
		nodeToDeletePtr -> setNext(nullptr);
		delete nodeToDeletePtr;
	}
	nodeToDeletePtr = nullptr;
	currentSize = 0;
}

