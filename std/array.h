#ifndef ARRAY_H
#define ARRAY_H

#include "util.h"

typedef long unsigned int size_t;

template <typename T>
class __array__
{
    public:
        __array__();
        __array__(T val);

        template <size_t n>
        __array__(T const (&rhs)[n]);
        __array__(const __array__<T> &rhs);
        ~__array__();

        __array__<T> &operator=(const __array__<T> &rhs);
        __array__<T> &operator+=(const T &val);

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

template <typename T>
__array__<T>::__array__()
{
    cap = 8;
    num = 0;
    data = new T[cap];
}

template <typename T>
template <size_t n>
__array__<T>::__array__(T const (&rhs)[n])
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
__array__<T>::__array__(const __array__<T> &rhs)
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
__array__<T>::~__array__()
{
    delete []data;
}

template <typename T>
__array__<T> &__array__<T>::operator=(const __array__<T> &rhs)
{
    cap = rhs.cap;
    num = rhs.num;

    if (data != NULL) delete []data;

    data = new T[cap];
    for (int i = 0; i < cap; i++)
    {
        data[i] = rhs.data[i];
    }

    return *this;
}

template <typename T>
__array__<T> &__array__<T>::operator+=(const T &val)
{
    pushb(val);
    return *this;
}

template <typename T>
const size_t __array__<T>::size() const
{
    return num;
}

template <typename T>
void __array__<T>::clear()
{
    num = 0;
}

template <typename T>
void __array__<T>::insert(size_t idx, const T &val)
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
void __array__<T>::pushb(const T &val)
{
    insert(num, val);
}

template <typename T>
void __array__<T>::pushf(const T &val)
{
    insert(0, val);
}

template <typename T>
void __array__<T>::remove(size_t idx)
{
    num--;
    for (int i = idx; i < num; i++)
    {
        data[i] = data[i + 1];
    }
}

template <typename T>
void __array__<T>::popb()
{
    remove(num - 1);
}

template <typename T>
void __array__<T>::popf()
{
    remove(0);
}

template <typename T>
T &__array__<T>::operator[](size_t idx)
{
    return data[idx];
}

template <typename T>
void __array__<T>::resize()
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
