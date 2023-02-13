#ifndef CPP_ORDERBOOK_MY_LIST_H
#define CPP_ORDERBOOK_MY_LIST_H

namespace my
{
    template <typename T>
    class ListNode {
    public:
        T data;
        ListNode<T> *prev;
        ListNode<T> *next;

        explicit ListNode(const T &d) : data(d), prev(nullptr), next(nullptr) {}
    };

    template<typename T>
    class DoublyLinkedList
    {
    public:
        ListNode<T>* m_head;
        ListNode<T>* m_tail;
        std::size_t m_size;

        DoublyLinkedList() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

        ~DoublyLinkedList() {
            ListNode<T> *current = m_head;
            while (current != nullptr) {
                ListNode<T> *next = current->next;
                delete current;
                current = next;
            }
        }

        [[nodiscard]] std::size_t size() const
        {
            return m_size;
        }

        void insert(const T &data) {
            auto *newNode = new ListNode<T>(data);

            if (m_head == nullptr)
            {
                m_head = m_tail = newNode;
            }
            else
            {
                newNode->prev = m_tail;
                m_tail->next = newNode;
                m_tail = newNode;
            }
            m_size++;
        }

        ListNode<T>* front() const {
            return m_head;
        }

        ListNode<T>* back() const {
            return m_tail;
        }

        void pop_front() {
            if (m_head == nullptr)
                return;

            ListNode<T> *temp = m_head;
            m_head = m_head->next;

            if (m_head == nullptr) {
                m_tail = nullptr;
            } else {
                m_head->prev = nullptr;
            }

            delete temp;
            m_size--;
        }

        void erase(ListNode<T> *node) {
            if (!node) {
                return;
            }

            if (node == m_head) {
                m_head = node->next;
            } else {
                node->prev->next = node->next;
            }

            if (node == m_tail) {
                m_tail = node->prev;
            } else {
                node->next->prev = node->prev;
            }

            m_size--;
            delete node;
        }
    };
}

#endif //CPP_ORDERBOOK_MY_LIST_H
