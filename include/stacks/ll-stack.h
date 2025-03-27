#ifndef LINKED_LIST_STACK_H_
#define LINKED_LIST_STACK_H_

#include <iostream>
#include <stdexcept>

/*
 * Stack
 * ATTRIBUTES:
 * - Node* head -> pointer to top of stack
 * METHODS:
 * - void add(T data) -> add a new node to the top of the stack
 * - T pop() -> returns the data at the head, deleted and updated head
 * - print() -> displays all elements of the stack
 * - operator<< displays all elements of the stack
 * 
 * Node
 * ATTRIBUTES:
 * - T data -> the data stored at the node
 * - Node<T>* next -> the address of the next node
 * METHODS:
 * - Node<T>* getNext() -> returns the address of the next node
 * - void setNext(Node<T>* next) -> sets the address of the next node
 * - T getData() -> returns the data from a specific node
 * - ~Node() -> destroy a node
 */

 namespace ll_stack {

template <typename T> struct Node {

    private:
        T m_data;
        Node<T>* m_next{nullptr};

    public:
        Node(T data): m_data{data}, m_next{nullptr} {}
        Node<T>* getNext() { return m_next; }
        void setNext(Node<T>* next) {
            Node<T>* tmp = m_next;
            while (m_next) {
                tmp = m_next;
                m_next = m_next->getNext();
                delete tmp;
            }
            m_next = next; 
        }
        T getData() { return m_data; }
        ~Node() {}
};

template <typename T> class Stack {

    private:
        Node<T>* m_head{nullptr};

    public:
        Stack() {}
        void add(T&& data) {
            Node<T>* node = new Node(std::forward<T>(data));
            node->setNext(m_head);
            m_head = node;
        }
        T pop() {
            if (!m_head) {throw std::out_of_range("stack.pop() from an empty stack."); }
            T data = std::move(m_head->getData());
            Node<T>* first = m_head;
            m_head = first->getNext();
            delete first;
            return data;
        }
        void print() {
            std::cout << "[";
            Node<T>* current = m_head;
            while (current) {
                std::cout << current->getData();
                current = current->getNext();
                if (current) {std::cout << ", "; }
            }
            std::cout << "]\n";
        }
        ~Stack() {
            Node<T>* current = m_head;
            while (m_head) {
                current = m_head;
                m_head = m_head->getNext();
                delete current;
            }
        }

};

 }

#endif