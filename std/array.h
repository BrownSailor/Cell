#ifndef ARRAY_H
#define ARRAY_H

#include "util.h"
#include "string.h"

typedef long unsigned int size_t;

namespace __built_in
{
    template <typename T>
    class array
    {
        public:
            array();
            array(size_t n);

            template <size_t n>
            array(T const (&rhs)[n]);
            array(const array<T> &rhs);
            ~array();

            array<T> &operator=(const array<T> &rhs);
            array<T> &operator+=(const T &val);

            T &operator[](size_t idx);

            const size_t size() const;
            void clear();

            void insert(size_t idx, const T &val);
            void pushb(const T &val);
            void pushf(const T &val);

            void remove(size_t idx);
            void popb();
            void popf();

        private:
            void resize();

            T *data;
            size_t cap;
            size_t num;
    };
}

template <typename T>
__built_in::array<T>::array()
{
    cap = 8;
    num = 0;
    data = new T[cap];
}

template <typename T>
__built_in::array<T>::array(size_t n)
{
    cap = n;
    num = n;
    data = new T[cap];
}

template <typename T>
template <size_t n>
__built_in::array<T>::array(T const (&rhs)[n])
{
    cap = n;
    num = n;

    data = new T[cap];
    for (int i = 0; i < cap; i++)
    {
        data[i] = rhs[i];
    }
}

template <typename T>
__built_in::array<T>::array(const array<T> &rhs)
{
    cap = rhs.cap;
    num = rhs.num;

    data = new T[cap];
    for (int i = 0; i < cap; i++)
    {
        data[i] = rhs.data[i];
    }
}

template <typename T>
__built_in::array<T>::~array()
{
    delete []data;
}

template <typename T>
__built_in::array<T> &__built_in::array<T>::operator=(const array<T> &rhs)
{
    cap = rhs.cap;
    num = rhs.num;

    if (data != nullptr) delete []data;

    data = new T[cap];
    for (int i = 0; i < cap; i++)
    {
        data[i] = rhs.data[i];
    }

    return *this;
}

template <typename T>
__built_in::array<T> &__built_in::array<T>::operator+=(const T &val)
{
    pushb(val);
    return *this;
}

template <typename T>
const size_t __built_in::array<T>::size() const
{
    return num;
}

template <typename T>
void __built_in::array<T>::clear()
{
    num = 0;
}

template <typename T>
void __built_in::array<T>::insert(size_t idx, const T &val)
{
    if (cap <= num)
    {
        resize();
    }

    for (int i = num; i > idx; i--)
    {
        data[i] = data[i - 1];
    }

    data[idx] = val;
    num++;
}

template <typename T>
void __built_in::array<T>::pushb(const T &val)
{
    insert(num, val);
}

template <typename T>
void __built_in::array<T>::pushf(const T &val)
{
    insert(0, val);
}

template <typename T>
void __built_in::array<T>::remove(size_t idx)
{
    num--;
    for (int i = idx; i < num; i++)
    {
        data[i] = data[i + 1];
    }
}

template <typename T>
void __built_in::array<T>::popb()
{
    remove(num - 1);
}

template <typename T>
void __built_in::array<T>::popf()
{
    remove(0);
}

template <typename T>
T &__built_in::array<T>::operator[](size_t idx)
{
    return data[idx];
}

template <typename T>
void __built_in::array<T>::resize()
{
    while (cap <= num)
    {
        cap *= 2;
    }
    T *new_data = new T[cap];

    for (int i = 0; i < num; i++)
    {
        new_data[i] = data[i];
    }

    delete []data;
    data = new_data;
}

#endif
