/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Alexander Dohms, Stephen Costigan, Shaun Crook, Jonathan Colwell
 ************************************************************************/

#pragma once 

#include <iostream>  // for OFSTREAM
#include <cassert>
#include "spy.h"

// Steve added this since we need allocators
#include <memory>    // For alloc

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
    // 
    // Construct
    //
	BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(T()) {}				// Default Constructor
	BNode(const T& t) : pParent(nullptr), pLeft(nullptr), pRight(nullptr), data(t) {}		// Copy Constructor
	BNode(T&& t) : pLeft(nullptr), pRight(nullptr), data(std::move(t)) {}					// Move Constructor

    //
    // Data
    //
    BNode <T> * pLeft;
    BNode <T> * pRight;
    BNode <T> * pParent;
    T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T, typename A = std::allocator<BNode <T>>>
inline size_t size(const BNode <T> * p)
{
	if (!p)
		return 0;
	return size(p->pLeft) + 1 + size(p->pRight);
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T, typename A = std::allocator<BNode <T>>>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
	if (pAdd)
		pAdd->pParent = pNode;
	pNode->pLeft = pAdd;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T, typename A = std::allocator<BNode <T>>>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{
	if (pAdd)
		pAdd->pParent = pNode;
	pNode->pRight = pAdd;
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T, typename A = std::allocator<BNode <T>>>
inline void addLeft (BNode <T> * pNode, const T & t) 
{
	BNode<T>* pAdd = new BNode<T>(t);
	pAdd->pParent = pNode;
	pNode->pLeft = pAdd;
}

template <class T, typename A = std::allocator<BNode <T>>>
inline void addLeft(BNode <T>* pNode, T && t)
{
	A alloc;
	BNode<T>* pAdd = alloc.allocate(1);
	alloc.construct(pAdd, std::move(t));
	pAdd->pParent = pNode;
	pNode->pLeft = pAdd; 
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T, typename A = std::allocator<BNode <T>>>
void addRight (BNode <T> * pNode, const T & t)
{
	BNode<T>* pAdd = new BNode<T>(t);
	pAdd->pParent = pNode;
	pNode->pRight = pAdd;
}

template <class T, typename A = std::allocator<BNode <T>>>
void addRight(BNode <T>* pNode, T && t)
{
	A alloc;
	BNode<T>* pAdd = alloc.allocate(1);
	alloc.construct(pAdd, std::move(t));
	pAdd->pParent = pNode;
	pNode->pRight = pAdd;
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T, typename A = std::allocator<BNode <T>>>
void clear(BNode <T> * & pThis)
{
	if (!pThis)
		return;

	A alloc;
	alloc.destroy(pThis + 0);

	clear(pThis->pLeft);
	clear(pThis->pRight);
	pThis = NULL;
	delete pThis;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T, typename A = std::allocator<BNode <T>>>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
	BNode<T>* tempHead = pRHS;
	pRHS = pLHS;
	pLHS = tempHead;
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T, typename A = std::allocator<BNode <T>>>
BNode <T> * copy(const BNode <T> * pSrc) 
{
	if (!pSrc) 
		return NULL;

	BNode<T>* destination = new BNode<T>(pSrc->data);

	destination->pLeft = copy(pSrc->pLeft);
	if (destination->pLeft != nullptr)
		destination->pLeft->pParent = destination;

	destination->pRight = copy(pSrc->pRight);
	if (destination->pRight != nullptr)
		destination->pRight->pParent = destination;

	return destination;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T, typename A = std::allocator<BNode <T>>>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
	// Source is Empty
	if (!pSrc) {
		clear(pDest);
		return;
	}

	// Neither the Source nor Destination are Empty
	if (pDest && pSrc) {
		pDest->data = pSrc->data;
		assign(pDest->pRight, pSrc->pRight);
		assign(pDest->pLeft, pSrc->pLeft);
	}

	// Destination is Empty
	if (!pDest && pSrc) {
		pDest = new BNode<T>(pSrc->data);
		assign(pDest->pRight, pSrc->pRight);
		assign(pDest->pLeft, pSrc->pLeft);
	}

	// Setting parent values
	if (pDest->pRight)
		pDest->pRight->pParent = pDest;
	if (pDest->pLeft)
		pDest->pLeft->pParent = pDest;
}