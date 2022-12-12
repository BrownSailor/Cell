#ifndef STRING_H
#define STRING_H

#include "util.h"
#include "array.h"

typedef long unsigned int size_t;

class __string__
{
    public:
        __string__();
        __string__(const char *str);
        __string__(const __string__ &str);
        __string__(__array__<char> str);
        ~__string__();

        __string__ &operator=(const char *str);
        __string__ &operator=(const __string__ &str);
        __string__ &operator=(__array__<char> str);

        __string__ operator+(char val);
        __string__ operator+(const char *str);
        __string__ operator+(const __string__ &str);

        __string__ &operator+=(char val);
        __string__ &operator+=(const char *str);
        __string__ &operator+=(const __string__ &str);

        bool operator==(const char *str);
        bool operator==(const __string__ &str);

        bool operator!=(const char *str);
        bool operator!=(const __string__ &str);

        void insert(size_t idx, char val);
        void insert(size_t idx, const char *str);
        void insert(size_t idx, const __string__ &str);

        void pushb(char val);
        void pushb(const char *str);
        void pushb(const __string__ &str);

        void pushf(char val);
        void pushf(const char *str);
        void pushf(const __string__ &str);

        void remove(size_t idx, size_t len = 1);
        void popb();
        void popf();

        char &operator[](size_t idx);

        const size_t size() const;
        const char *to_string() const;

    private:
        void resize();

        char *data;
        size_t num;
        size_t cap;
};

__string__::__string__()
{
    num = 0;
    cap = 4;
    data = new char[cap];
    data[0] = '\0';
}

__string__::__string__(const char *str)
{
    num = strlen(str);
    cap = num + 1;
    data = new char[cap];
    memcpy(data, str, cap);
}

__string__::__string__(const __string__ &str)
{
    num = str.size();
    cap = num + 1;
    data = new char[cap];
    memcpy(data, str.data, cap);
}

__string__::__string__(__array__<char> str)
{
    num = str.size();
    cap = num + 1;
    data = new char[cap];

    for (int i = 0; i < num; i++)
    {
        data[i] = str[i];
    }
}

__string__::~__string__()
{
    if (data != NULL) delete []data;
}

const size_t __string__::size() const
{
    return num;
}

__string__ &__string__::operator=(const char *str)
{
    if (data != NULL) delete []data;

    num = strlen(str);
    cap = num + 1;
    data = new char[cap];
    memcpy(data, str, cap);

    return *this;
}

__string__ &__string__::operator=(const __string__ &rhs)
{
    if (data != NULL) delete []data;

    num = rhs.size();
    cap = num + 1;
    data = new char[cap];
    memcpy(data, rhs.data, cap);

    return *this;
}

__string__ &__string__::operator=(__array__<char> rhs)
{
    if (data != NULL) delete []data;

    num = rhs.size();
    cap = num + 1;
    data = new char[cap];

    for (int i = 0; i < num; i++)
    {
        data[i] = rhs[i];
    }

    return *this;
}

__string__ __string__::operator+(char val)
{
    __string__ sum;

    sum.num = size() + 1;
    sum.cap = sum.num + 1;
    sum.data = new char[sum.cap];

    memcpy(sum.data, data, size());
    sum.data[size()] = val;
    sum.data[sum.size()] = '\0';

    return sum;
}

__string__ __string__::operator+(const char *str)
{
    __string__ sum;

    size_t str_size = strlen(str);

    sum.num = size() + str_size;
    sum.cap = sum.num + 1;
    sum.data = new char[sum.cap];

    memcpy(sum.data, data, size());
    memcpy(sum.data + size(), str, str_size);
    sum.data[sum.size()] = '\0';

    return sum;
}

__string__ __string__::operator+(const __string__ &str)
{
    __string__ sum;

    sum.num = size() + str.size();
    sum.cap = sum.num + 1;
    sum.data = new char[sum.cap];

    memcpy(sum.data, data, size());
    memcpy(sum.data + size(), str.data, str.size());
    sum.data[sum.size()] = '\0';

    return sum;
}

__string__ &__string__::operator+=(char val)
{
    num++;
    if (num >= cap - 1)
    {
        resize();
    }

    data[num - 1] = val;
    data[num] = '\0';

    return *this;
}

__string__ &__string__::operator+=(const char *str)
{
    size_t str_size = strlen(str);
    size_t old_size = num;

    num += str_size;

    if (num >= cap - 1)
    {
        resize();
    }

    memcpy(data + old_size, str, str_size);
    data[num] = '\0';

    return *this;
}

__string__ &__string__::operator+=(const __string__ &str)
{
    size_t old_size = num;
    num += str.size();

    if (num >= cap - 1)
    {
        resize();
    }

    memcpy(data + old_size, str.data, str.size());
    data[num] = '\0';

    return *this;
}

bool __string__::operator==(const char *str)
{
    return strcmp(data, str) == 0;
}

bool __string__::operator==(const __string__ &str)
{
    return strcmp(data, str.data) == 0;
}

bool __string__::operator!=(const char *str)
{
    return strcmp(data, str) != 0;
}

bool __string__::operator!=(const __string__ &str)
{
    return strcmp(data, str.data) != 0;
}

char &__string__::operator[](size_t idx)
{
    return data[idx];
}

void __string__::insert(size_t idx, char val)
{
    num++;
    if (num >= cap - 1)
    {
        resize();
    }

    memmove(data + idx + 1, data + idx, 1);
    data[idx] = val;
    data[num] = '\0';
}

void __string__::insert(size_t idx, const char *str)
{
    size_t str_size = strlen(str);
    num += str_size;
    if (num >= cap - 1)
    {
        resize();
    }

    memmove(data + idx + str_size, data + idx, num - str_size - idx);
    memcpy(data + idx, str, str_size);
    data[num] = '\0';
}

void __string__::insert(size_t idx, const __string__ &str)
{
    num += str.size();
    if (num >= cap - 1)
    {
        resize();
    }

    memmove(data + idx + str.size(), data + idx, num - str.size() - idx);
    memcpy(data + idx, str.data, str.size());
    data[num] = '\0';
}

void __string__::pushb(char val)
{
    insert(num, val);
}

void __string__::pushb(const char *str)
{
    insert(num, str);
}

void __string__::pushb(const __string__ &str)
{
    insert(num, str);
}

void __string__::pushf(char val)
{
    insert(0, val);
}

void __string__::pushf(const char *str)
{
    insert(0, str);
}

void __string__::pushf(const __string__ &str)
{
    insert(0, str);
}

void __string__::remove(size_t idx, size_t len)
{
    memmove(data + idx, data + idx + len, num - idx);
    num -= len;
    data[num] = '\0';
}

void __string__::popb()
{
    remove(num - 1);
}

void __string__::popf()
{
    remove(0);
}

const char *__string__::to_string() const
{
    return data;
}

void __string__::resize()
{
    while (num >= cap - 1)
    {
        cap *= 2;
    }

    char *new_data = new char[cap];
    memcpy(new_data, data, num);
    new_data[num] = '\0';

    delete []data;
    data = new_data;
}
#endif
