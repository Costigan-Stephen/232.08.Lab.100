/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    Stephen Costigan, Alexander Dohms, Jonathan Colwell, Shaun Crook
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less


namespace custom
{

    /************************************************
     * SET
     * A class that represents a Set
     ***********************************************/
    template <typename T>
    class set
    {
    public:

        // 
        // Construct
        //
        set() : bst() {} // Default Constructor
        set(const set& rhs) : bst(rhs.bst) {

        } // Move constructor

        set(set&& rhs) : bst(std::move(rhs.bst)) {}                        // Copy Constructor
        set(const std::initializer_list <T>& il) { *this = il; }           // Initilizer List and Range constructor. (Needs tweaked to allow successive insertions)
        template <class Iterator>
        set(Iterator first, Iterator last)
        {
            for (auto c = first; c != last; c++)                           // This will likely work when insert works.
                insert(*c);
        }
        ~set() {}

        //
        // Assign
        //

        set& operator = (const set& rhs)
        {
            bst = rhs.bst;
            if (bst.root == nullptr)
                clear();
            return *this;
        }

        set& operator = (set&& rhs)
        {
            clear();
            swap(rhs);
            return *this;
        }
        set& operator = (const std::initializer_list <T>& il)
        {
            clear();
            for (T t : il)
                insert(t);

            return *this;
        }
        void swap(set& rhs) noexcept
        {
            auto tempRoot = rhs.bst;
            rhs.bst = bst;
            bst = tempRoot;
        }

        //
        // Iterator
        //

        class iterator;
        iterator begin() const noexcept
        {
            return iterator(bst.begin());
        }
        iterator end() const noexcept
        {
            return iterator(bst.end());
        }

        //
        // Access
        //
        iterator find(const T& t)
        {
            return bst.find(t);
        }

        //
        // Status
        //
        bool   empty() const noexcept
        {
            return bst.empty();
        }
        size_t size() const noexcept
        {
            return bst.size();
        }

        //
        // Insert
        //
        std::pair<iterator, bool> insert(const T& t)
        {
            auto bst_pair = bst.insert(t, true);
            return bst_pair;
        }
        std::pair<iterator, bool> insert(T&& t)
        {
            auto bst_pair = bst.insert(std::move(t), true);
            return bst_pair;
        }
        void insert(const std::initializer_list <T>& il) 
        {
            for (T t : il)
                insert(t);
        }
        template <class Iterator> // Shaun DONE
        void insert(Iterator first, Iterator last)
        {
            for (auto c = first; c != last; c++)
                insert(*c);
        }


        //
        // Remove
        //
        void clear() noexcept
        {
            bst.clear();
        }
        iterator erase(iterator& it) // Steve
        {
            return iterator(bst.erase(it.it));
        }
        size_t erase(const T& t) // Jon
        {
            iterator it = bst.find(t);
            if (it == end()) 
                return 0;

            erase(it);
            return 1;
        }
        iterator erase(iterator& itBegin, iterator& itEnd) // Alex
        {
            while (itBegin != itEnd)
                itBegin = erase(itBegin);
            return itEnd; 
        }

#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif

        custom::BST <T> bst;
    };


    /**************************************************
     * SET ITERATOR
     * An iterator through Set
     *************************************************/
    template <typename T>
    class set <T> ::iterator
    {
    public:
        // constructors, destructors, and assignment operator
        iterator() { it = nullptr; }
        iterator(const typename custom::BST<T>::iterator& itRHS) { it = itRHS; }
        iterator(const iterator& rhs) { it = rhs.it; }
        iterator& operator = (const iterator& rhs)
        {
            it = rhs.it;
            return *this;
        }


        // equals, not equals operator
        bool operator != (const iterator& rhs) const
        {
            return it != rhs.it;
        }
        bool operator == (const iterator& rhs) const // lowers the % for some reason
        {
            return it == rhs.it;
        }

        // dereference operator: by-reference so we can modify the Set
        const T& operator * () const
        {
            return *it;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            it++;
            return *this;
        }

        // postfix increment
        iterator operator++ (int postfix)
        {
            auto oldit = it;
            it++;
            return oldit;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            it--;
            return *this;
        }

        // postfix decrement
        iterator operator-- (int postfix)
        {
            auto oldit = it;
            it--;
            return oldit;
        }

#ifdef DEBUG // make this visible to the unit tests
    public:
#else
    private:
#endif

        typename custom::BST<T>::iterator it;
    };

    /***********************************************
     * SET : EQUIVALENCE
     * See if two sets are the same size
     ***********************************************/
    template <typename T>
    bool operator == (const set <T>& lhs, const set <T>& rhs)
    {
        return lhs == rhs;
    }

    template <typename T>
    inline bool operator != (const set <T>& lhs, const set <T>& rhs)
    {
        return lhs != rhs;
    }

    /***********************************************
     * SET : RELATIVE COMPARISON
     * See if one set is lexicographically before the second
     ***********************************************/
    template <typename T>
    bool operator < (const set <T>& lhs, const set <T>& rhs)
    {
        return lhs < rhs;
    }

    template <typename T>
    inline bool operator > (const set <T>& lhs, const set <T>& rhs)
    {
        return lhs > rhs;
    }

}; // namespace custom
