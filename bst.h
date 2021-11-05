/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *
 *    __    ___     ___    _   __
 *   /_ |  / _ \   / _ \  (_) / /
 *    | | | | | | | | | |    / /
 *    | | | | | | | | | |   / /
 *    | | | |_| | | |_| |  / / _
 *    |_|  \___/   \___/  /_/ (_)
 *
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

namespace custom
{

    /*****************************************************************
     * BINARY SEARCH TREE
     * Create a Binary Search Tree
     *****************************************************************/
    template <typename T>
    class BST
    {
    public:
        //
        // Construct
        //

        BST();
        BST(const BST& rhs);
        BST(BST&& rhs);
        BST(const std::initializer_list<T>& il);
        ~BST();

        //
        // Assign
        //

        BST& operator = (const BST& rhs);
        BST& operator = (BST&& rhs);
        BST& operator = (const std::initializer_list<T>& il);
        void swap(BST& rhs);

        //
        // Iterator
        //

        class iterator;
        iterator   begin() const noexcept;
        iterator   end()   const noexcept { return iterator(nullptr); }

        //
        // Access
        //

        iterator find(const T& t);

        // 
        // Insert
        //

        std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
        std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);

        //
        // Remove
        // 

        iterator erase(iterator& it);
        void   clear() noexcept;

        // 
        // Status
        //

        bool   empty() const noexcept { return size() == 0; }
        size_t size()  const noexcept { return numElements; }


#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif

        class BNode;

        // utility functions which need to be done recursively
        void deleteNode(BNode*& pDelete, bool toRight);
        void deleteBinaryTree(BNode*& pDelete) noexcept;
        void copyBinaryTree(const BNode* pSrc, BNode*& pDest);

        BNode* root;               // root node of the binary search tree
        size_t numElements;        // number of elements currently in the tree
    };


    /*****************************************************************
     * BINARY NODE
     * A single node in a binary tree. Note that the node does not know
     * anything about the properties of the tree so no validation can be done.
     *****************************************************************/
    template <typename T>
    class BST <T> ::BNode
    {
    public:
        // 
        // Construct
        //
        BNode()
            : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data()
        {
        }
        BNode(const T& t)
            : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(t)
        {
        }
        BNode(T&& t)
            : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(std::move(t))
        {
        }

        //
        // Insert
        //
        void addLeft(BNode* pNode);
        void addRight(BNode* pNode);
        void addLeft(const T& t);
        void addRight(const T& t);
        void addLeft(T&& t);
        void addRight(T&& t);

        // 
        // Status
        //
        bool isRightChild(BNode* pNode) const { return pRight == pNode; }
        bool isLeftChild(BNode* pNode) const { return pLeft == pNode; }

        //
        // Data
        //
        T data;                // Actual data stored in the BNode
        BNode* pLeft;          // Left child - smaller
        BNode* pRight;         // Right child - larger
        BNode* pParent;        // Parent
    };

    /**********************************************************
     * BINARY SEARCH TREE ITERATOR
     * Forward and reverse iterator through a BST
     *********************************************************/
    template <typename T>
    class BST <T> ::iterator
    {
    public:
        // constructors and assignment
        iterator(BNode* p = nullptr)
        {
            pNode = p;
        }
        iterator(const iterator& rhs)
        {
            pNode = rhs.pNode;
        }
        iterator& operator = (const iterator& rhs)
        {
            pNode = rhs.pNode;
            return *this;
        }

        // compare
        bool operator == (const iterator& rhs) const
        {
            // only need to compare the leaf node 
            return rhs.pNode == pNode;
        }
        bool operator != (const iterator& rhs) const
        {
            // only need to compare the leaf node 
            return rhs.pNode != pNode;
        }

        // de-reference. Cannot change because it will invalidate the BST
        const T& operator * () const
        {
            return pNode->data;
        }

        // increment and decrement
        iterator& operator ++ ();
        iterator   operator ++ (int postfix)
        {
            iterator itReturn = *this;
            ++(*this);
            return itReturn;
        }
        iterator& operator -- ();
        iterator   operator -- (int postfix)
        {
            iterator itReturn = *this;
            --(*this);
            return itReturn;
        }

        // must give friend status to remove so it can call getNode() from it
        friend BST <T> ::iterator BST <T> ::erase(iterator& it);

#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif

        // the node
        BNode* pNode;
    };


    /*********************************************
     *********************************************
     *********************************************
     ******************** BST ********************
     *********************************************
     *********************************************
     *********************************************/


     /*********************************************
      * BST :: DEFAULT CONSTRUCTOR
      ********************************************/
    template <typename T>
    BST <T> ::BST()
        : root(nullptr), numElements(0)
    {
    }

    /*********************************************
     * BST :: COPY CONSTRUCTOR
     * Copy one tree to another
     ********************************************/
    template <typename T>
    BST <T> ::BST(const BST<T>& rhs)
        : root(nullptr), numElements(0)
    {
        // call the assignment operator
        *this = rhs;
    }

    /*********************************************
     * BST :: MOVE CONSTRUCTOR
     * Move one tree to another
     ********************************************/
    template <typename T>
    BST <T> ::BST(BST <T>&& rhs)
        : root(nullptr), numElements(0)
    {
        // move the nodes and set the RHS to empty
        root = rhs.root;
        rhs.root = nullptr;

        // move the number of elements and set the RHS to empty
        numElements = rhs.numElements;
        rhs.numElements = 0;
    }

    /*********************************************
     * BST :: DESTRUCTOR
     ********************************************/
    template <typename T>
    BST <T> :: ~BST()
    {
        clear();
    }


    /*********************************************
     * BST :: ASSIGNMENT OPERATOR
     * Copy one tree to another
     ********************************************/
    template <typename T>
    BST <T>& BST <T> :: operator = (const BST <T>& rhs)
    {
        copyBinaryTree(rhs.root, this->root);
        assert(nullptr == this->root || this->root->pParent == nullptr);
        this->numElements = rhs.numElements;

        return *this;
    }

    /*********************************************
     * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
     * Copy nodes onto a BTree
     ********************************************/
    template <typename T>
    BST <T>& BST <T> :: operator = (const std::initializer_list<T>& il)
    {
        // we cannot preserve the nodes so we must start from scratch
        deleteBinaryTree(root);
        numElements = 0;

        // for each element in the initializer list, add into the BST using insert()
        for (auto&& element : il)
            insert(element);

        return *this;
    }

    /*********************************************
     * BST :: ASSIGN-MOVE OPERATOR
     * Move one tree to another
     ********************************************/
    template <typename T>
    BST <T>& BST <T> :: operator = (BST <T>&& rhs)
    {
        // clear the old bst
        clear();

        // move the nodes and set the RHS to empty
        root = rhs.root;
        rhs.root = nullptr;

        // move the number of elements and set the RHS to empty
        numElements = rhs.numElements;
        rhs.numElements = 0;

        return *this;
    }

    /*********************************************
     * BST :: SWAP
     * Swap two trees
     ********************************************/
    template <typename T>
    void BST <T> ::swap(BST <T>& rhs)
    {
        BNode* pTemp = rhs.root;
        rhs.root = root;
        root = pTemp;

        size_t numTemp = rhs.numElements;
        rhs.numElements = numElements;
        numElements = numTemp;
    }

    /*****************************************************
     * BST :: INSERT
     * Insert a node at a given location in the tree
     ****************************************************/
    template <typename T>
    std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(const T& t, bool keepUnique)
    {
        std::pair<iterator, bool> pairReturn(end(), false);
        try
        {
            // if we are at a trivial state (empty tree), then create a new root
            if (root == nullptr)
            {
                assert(numElements == 0);
                root = new BNode(t);
                numElements = 1;
                pairReturn.first = iterator(root);
                pairReturn.second = true;
                return pairReturn;
            }

            // otherwise, go a searching for the correct spot
            BNode* node = root;
            bool done = false;
            while (!done)
            {
                // if the node is a match, then do nothing
                if (keepUnique && t == node->data)
                {
                    pairReturn.first = iterator(node);
                    pairReturn.second = false;
                    return pairReturn;
                }

                // if the center node is larger, go left
                if ((t < node->data))
                {
                    // if there is a node to the left, follow it
                    if (node->pLeft)
                        node = node->pLeft;
                    // if we are at the leaf, then create a new node
                    else
                    {
                        node->addLeft(t);
                        done = true;
                        pairReturn.first = iterator(node->pLeft);
                        pairReturn.second = true;
                    }
                }

                // if the center node is smaller, go right
                else
                {
                    // if there is a node to the right, follow it
                    if (node->pRight)
                        node = node->pRight;
                    // if we are at the left, then create a new node.
                    else
                    {
                        node->addRight(t);
                        done = true;
                        pairReturn.first = iterator(node->pRight);
                        pairReturn.second = true;
                    }
                }
            }
            // we just inserted something!
            assert(root != nullptr);
            numElements++;

            // if the root moved out from under us, find it again.
            while (root->pParent != nullptr)
                root = root->pParent;
            assert(root->pParent == nullptr);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
        return pairReturn;
    }

    template <typename T>
    std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T&& t, bool keepUnique)
    {
        std::pair<iterator, bool> pairReturn(end(), false);
        try
        {
            // if we are at a trivial state (empty tree), then create a new root
            if (root == nullptr)
            {
                assert(numElements == 0);
                root = new BNode(std::move(t));
                numElements = 1;
                pairReturn.first = iterator(root);
                pairReturn.second = true;
                return pairReturn;
            }

            // otherwise, go a searching for the correct spot
            BNode* node = root;
            bool done = false;
            while (!done)
            {
                // if the node is a match, then do nothing
                if (keepUnique && t == node->data)
                {
                    pairReturn.first = iterator(node);
                    pairReturn.second = false;
                    return pairReturn;
                }

                // if the center node is larger, go left
                if (t < node->data)
                {
                    // if there is a node to the left, follow it
                    if (node->pLeft)
                        node = node->pLeft;
                    // if we are at the leaf, then create a new node
                    else
                    {
                        node->addLeft(std::move(t));
                        done = true;
                        pairReturn.first = iterator(node->pLeft);
                        pairReturn.second = true;
                    }
                }

                // if the center node is smaller, go right
                else
                {
                    // if there is a node to the right, follow it
                    if (node->pRight)
                        node = node->pRight;
                    // if we are at the left, then create a new node.
                    else
                    {
                        node->addRight(std::move(t));
                        done = true;
                        pairReturn.first = iterator(node->pRight);
                        pairReturn.second = true;
                    }
                }
            }
            // we just inserted something!
            assert(root != nullptr);
            numElements++;

            // if the root moved out from under us, find it again.
            while (root->pParent != nullptr)
                root = root->pParent;
            assert(root->pParent == nullptr);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
        return pairReturn;
    }

    /*************************************************
     * BST :: ERASE
     * Remove a given node as specified by the iterator
     ************************************************/
    template <typename T>
    typename BST <T> ::iterator BST <T> ::erase(iterator& it)
    {
        // do nothing if there is nothing to do
        if (it == end())
            return end();

        // remember where we were
        iterator itNext = it;
        BNode* pDelete = it.pNode;

        // if there is only one child (right) or no children (how sad!)
        if (pDelete->pLeft == nullptr)
        {
            ++itNext;
            deleteNode(pDelete, true /* goRight */);
        }

        // if there is only one child (left)
        else if (pDelete->pRight == nullptr)
        {
            ++itNext;
            deleteNode(pDelete, false /* goRight */);
        }

        // otherwise, swap places with the in-order successor
        else
        {
            // find the in-order successor (IOS)
            BNode* pIOS = pDelete->pRight;
            while (pIOS->pLeft != nullptr)
                pIOS = pIOS->pLeft;

            // the IOS must not have a right node. Now it will take pDelete's place.
            assert(pIOS->pLeft == nullptr);
            pIOS->pLeft = pDelete->pLeft;
            if (pDelete->pLeft)
                pDelete->pLeft->pParent = pIOS;

            // if the IOS is not direct right sibling, then put it in the place of pDelete
            if (pDelete->pRight != pIOS)
            {
                // if the IOS has a right sibling, then it takes his place
                if (pIOS->pRight)
                    pIOS->pRight->pParent = pIOS->pParent;
                pIOS->pParent->pLeft = pIOS->pRight;

                // make IOS's right child pDelete's right child
                assert(pDelete->pRight != nullptr);
                pIOS->pRight = pDelete->pRight;
                pDelete->pRight->pParent = pIOS;
            }

            // hook up pIOS's successor
            pIOS->pParent = pDelete->pParent;
            if (pDelete->pParent && pDelete->pParent->pLeft == pDelete)
                pDelete->pParent->pLeft = pIOS;
            if (pDelete->pParent && pDelete->pParent->pRight == pDelete)
                pDelete->pParent->pRight = pIOS;

            // what if that was the root?!?!
            if (root == pDelete)
                root = pIOS;

            itNext = iterator(pIOS);
        }

        numElements--;
        delete pDelete;
        return itNext;
    }

    /*****************************************************
     * BST :: CLEAR
     * Removes all the BNodes from a tree
     ****************************************************/
    template <typename T>
    void BST <T> ::clear() noexcept
    {
        if (root)
            deleteBinaryTree(root);
        numElements = 0;
    }

    /*****************************************************
     * BST :: BEGIN
     * Return the first node (left-most) in a binary search tree
     ****************************************************/
    template <typename T>
    typename BST <T> ::iterator custom::BST <T> ::begin() const noexcept
    {
        // if the BST is empty, return the nullptr iterator. 
        if (root == nullptr)
            return end();

        // otherwise, find the left-most node;
        BNode* p = root;
        while (p->pLeft)
            p = p->pLeft;
        return iterator(p);
    }

    /****************************************************
     * BST :: FIND
     * Return the node corresponding to a given value
     ****************************************************/
    template <typename T>
    typename BST <T> ::iterator BST<T> ::find(const T& t)
    {
        // perform a binary search using a non-recursive solution
        for (BNode* p = root; p != nullptr; p = (t < p->data ? p->pLeft : p->pRight))
            if (p->data == t)
                return iterator(p);

        // nothing was found so return the nullptr iterator
        return end();
    }

    /*****************************************************
     * DELETE BINARY TREE
     * Delete all the nodes below pThis including pThis
     * using postfix traverse: LRV
     ****************************************************/
    template <typename T>
    void BST <T> ::deleteBinaryTree(BNode*& pDelete) noexcept
    {
        if (pDelete == nullptr)
            return;
        deleteBinaryTree(pDelete->pLeft);
        deleteBinaryTree(pDelete->pRight);

        delete pDelete;
        pDelete = nullptr;
    }

    /**********************************************
     * COPY BINARY TREE
     * Copy pSrc->pRight to pDest->pRight and
     * pSrc->pLeft onto pDest->pLeft
     *********************************************/
    template <typename T>
    void BST <T> ::copyBinaryTree(const BNode* pSrc, BNode*& pDest)
    {
        // if there is no node in pSrc, then do nothing
        if (nullptr == pSrc)
        {
            deleteBinaryTree(pDest);
            return;
        }

        assert(pSrc);
        // if there is not a node for pDest, then allocate one
        try
        {
            if (nullptr == pDest)
                pDest = new BST::BNode(pSrc->data);
            // otherwise, assign the data over
            else
                pDest->data = pSrc->data;
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
        assert(pDest != nullptr);

        // handle the children to the right and left
        copyBinaryTree(pSrc->pRight, pDest->pRight);
        copyBinaryTree(pSrc->pLeft, pDest->pLeft);
        if (pDest->pRight)
            pDest->pRight->pParent = pDest;
        if (pSrc->pLeft)
            pDest->pLeft->pParent = pDest;
    }

    /****************************************************
     * DELETE NODE
     * Delete a single node (pDelete) from the tree indicated
     * by a node (pDelete).
     *    pDelete     the node to be deleted
     *    toRight     should the right branch inherit our place?
     ****************************************************/
    template <typename T>
    void BST <T> ::deleteNode(BNode*& pDelete, bool toRight)
    {
        // shift everything up
        BNode* pNext = (toRight ? pDelete->pRight : pDelete->pLeft);

        // if we are not the parent, hook ourselves into the existing tree
        if (pDelete != root)
        {
            if (pDelete->pParent->pLeft == pDelete)
            {
                pDelete->pParent->pLeft = nullptr;
                pDelete->pParent->addLeft(pNext);
            }
            else
            {
                pDelete->pParent->pRight = nullptr;
                pDelete->pParent->addRight(pNext);
            }
        }

        // otherwise, the pNext is the new root
        else
        {
            root = pNext;
            pNext->pParent = nullptr;
        }
    }

    /******************************************************
     ******************************************************
     ******************************************************
     *********************** B NODE ***********************
     ******************************************************
     ******************************************************
     ******************************************************/


     /******************************************************
      * BINARY NODE :: ADD LEFT
      * Add a node to the left of the current node
      ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addLeft(BNode* pNode)
    {
        pLeft = pNode;
        if (pNode)
            pNode->pParent = this;
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addRight(BNode* pNode)
    {
        pRight = pNode;
        if (pNode)
            pNode->pParent = this;
    }

    /******************************************************
     * BINARY NODE :: ADD LEFT
     * Add a node to the left of the current node
     ******************************************************/
    template <typename T>
    void BST<T> ::BNode::addLeft(const T& t)
    {
        assert(pLeft == nullptr);

        try
        {
            BNode* pNode = new BNode(t);
            addLeft(pNode);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
    }

    /******************************************************
     * BINARY NODE :: ADD LEFT
     * Add a node to the left of the current node
     ******************************************************/
    template <typename T>
    void BST<T> ::BNode::addLeft(T&& t)
    {
        assert(pLeft == nullptr);

        try
        {
            BNode* pNode = new BNode(std::move(t));
            addLeft(pNode);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addRight(const T& t)
    {
        assert(pRight == nullptr);

        try
        {
            BNode* pNode = new BNode(t);
            addRight(pNode);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
    }

    /******************************************************
     * BINARY NODE :: ADD RIGHT
     * Add a node to the right of the current node
     ******************************************************/
    template <typename T>
    void BST <T> ::BNode::addRight(T&& t)
    {
        assert(pRight == nullptr);

        try
        {
            BNode* pNode = new BNode(std::move(t));
            addRight(pNode);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
    }


    /*************************************************
     *************************************************
     *************************************************
     ****************** ITERATOR *********************
     *************************************************
     *************************************************
     *************************************************/

     /**************************************************
      * BST ITERATOR :: INCREMENT PREFIX
      * advance by one
      *************************************************/
    template <typename T>
    typename BST <T> ::iterator& BST <T> ::iterator :: operator ++ ()
    {
        // do nothing if we have nothing
        if (nullptr == pNode)
            return *this;

        // if there is a right node, take it
        if (nullptr != pNode->pRight)
        {
            // go right
            pNode = pNode->pRight;

            // jig left - there might be more left-most children
            while (pNode->pLeft)
                pNode = pNode->pLeft;
            return *this;
        }

        // there are no right children, the left are done
        assert(nullptr == pNode->pRight);
        const BNode* pSave = pNode;

        // go up...
        pNode = pNode->pParent;

        // if the parent is the nullptr, we are done!
        if (nullptr == pNode)
            return *this;

        // if we are the left-child, got to the parent.
        if (pSave == pNode->pLeft)
            return *this;

        // we are the right-child, go up as long as we are the right child!
        while (nullptr != pNode && pSave == pNode->pRight)
        {
            pSave = pNode;
            pNode = pNode->pParent;
        }

        return *this;
    }

    /**************************************************
     * BST ITERATOR :: DECREMENT PREFIX
     * advance by one
     *************************************************/
    template <typename T>
    typename BST <T> ::iterator& BST <T> ::iterator :: operator -- ()
    {
        // do nothing if we have nothing
        if (nullptr == pNode)
            return *this;

        // if there is a left node, take it
        if (nullptr != pNode->pLeft)
        {
            // go left
            pNode = pNode->pLeft;

            // jig right - there might be more right-most children
            while (pNode->pRight)
                pNode = pNode->pRight;
            return *this;
        }

        // there are no left children, the right are done
        assert(nullptr == pNode->pLeft);
        const BNode* pSave = pNode;

        // go up
        pNode = pNode->pParent;

        // if the parent is the nullptr, we are done!
        if (nullptr == pNode)
            return *this;

        // if we are the right-child, got to the parent.
        if (pSave == pNode->pRight)
            return *this;

        // we are the left-child, go up as long as we are the left child!
        while (nullptr != pNode && pSave == pNode->pLeft)
        {
            pSave = pNode;
            pNode = pNode->pParent;
        }

        return *this;
    }


} // namespace custom


