#ifndef STRING_H
#define STRING_H

#include "util.h"

typedef long unsigned int size_t;

namespace __built_in
{
    class string
    {
        public:
            string();
            string(char val);
            string(const char *str);
            string(const string &str);
            ~string();

            string &operator=(char val);
            string &operator=(const char *str);
            string &operator=(const string &str);

            string operator+(char val);
            string operator+(const char *str);
            string operator+(const string &str);

            string &operator+=(char val);
            string &operator+=(const char *str);
            string &operator+=(const string &str);

            bool operator==(const char *str);
            bool operator==(const string &str);

            bool operator!=(const char *str);
            bool operator!=(const string &str);

            void insert(size_t idx, char val);
            void insert(size_t idx, const char *str);
            void insert(size_t idx, const string &str);

            void pushb(char val);
            void pushb(const char *str);
            void pushb(const string &str);

            void pushf(char val);
            void pushf(const char *str);
            void pushf(const string &str);

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
}

__built_in::string::string()
{
    num = 0;
    cap = 4;
    data = new char[cap];
    data[0] = '\0';
}

__built_in::string::string(char val)
{
    num = 1;
    cap = 4;
    data = new char[cap];
    data[0] = val;
    data[1] = '\0';
}

__built_in::string::string(const char *str)
{
    num = strlen(str);
    cap = num + 1;
    data = new char[cap];
    memcpy(data, str, cap);
}

__built_in::string::string(const string &str)
{
    num = str.size();
    cap = num + 1;
    data = new char[cap];
    memcpy(data, str.data, cap);
}

__built_in::string::~string()
{
    if (data != nullptr) delete []data;
}

const size_t __built_in::string::size() const
{
    return num;
}

__built_in::string &__built_in::string::operator=(char val)
{
    if (data != nullptr) delete []data;

    num = 1;
    cap = 4;
    data = new char[cap];
    data[0] = val;
    data[1] = '\0';

    return *this;
}

__built_in::string &__built_in::string::operator=(const char *str)
{
    if (data != nullptr) delete []data;

    num = strlen(str);
    cap = num + 1;
    data = new char[cap];
    memcpy(data, str, cap);

    return *this;
}

__built_in::string &__built_in::string::operator=(const string &rhs)
{
    if (data != nullptr) delete []data;

    num = rhs.size();
    cap = num + 1;
    data = new char[cap];
    memcpy(data, rhs.data, cap);

    return *this;
}

__built_in::string __built_in::string::operator+(char val)
{
    string sum;

    sum.num = size() + 1;
    sum.cap = sum.num + 1;
    sum.data = new char[sum.cap];

    memcpy(sum.data, data, size());
    sum.data[size()] = val;
    sum.data[sum.size()] = '\0';

    return sum;
}

__built_in::string __built_in::string::operator+(const char *str)
{
    string sum;

    size_t str_size = strlen(str);

    sum.num = size() + str_size;
    sum.cap = sum.num + 1;
    sum.data = new char[sum.cap];

    memcpy(sum.data, data, size());
    memcpy(sum.data + size(), str, str_size);
    sum.data[sum.size()] = '\0';

    return sum;
}

__built_in::string __built_in::string::operator+(const string &str)
{
    string sum;

    sum.num = size() + str.size();
    sum.cap = sum.num + 1;
    sum.data = new char[sum.cap];

    memcpy(sum.data, data, size());
    memcpy(sum.data + size(), str.data, str.size());
    sum.data[sum.size()] = '\0';

    return sum;
}

__built_in::string &__built_in::string::operator+=(char val)
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

__built_in::string &__built_in::string::operator+=(const char *str)
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

__built_in::string &__built_in::string::operator+=(const string &str)
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

bool __built_in::string::operator==(const char *str)
{
    return strcmp(data, str) == 0;
}

bool __built_in::string::operator==(const string &str)
{
    return strcmp(data, str.data) == 0;
}

bool __built_in::string::operator!=(const char *str)
{
    return strcmp(data, str) != 0;
}

bool __built_in::string::operator!=(const string &str)
{
    return strcmp(data, str.data) != 0;
}

char &__built_in::string::operator[](size_t idx)
{
    return data[idx];
}

void __built_in::string::insert(size_t idx, char val)
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

void __built_in::string::insert(size_t idx, const char *str)
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

void __built_in::string::insert(size_t idx, const string &str)
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

void __built_in::string::pushb(char val)
{
    insert(num, val);
}

void __built_in::string::pushb(const char *str)
{
    insert(num, str);
}

void __built_in::string::pushb(const string &str)
{
    insert(num, str);
}

void __built_in::string::pushf(char val)
{
    insert(0, val);
}

void __built_in::string::pushf(const char *str)
{
    insert(0, str);
}

void __built_in::string::pushf(const string &str)
{
    insert(0, str);
}

void __built_in::string::remove(size_t idx, size_t len)
{
    memmove(data + idx, data + idx + len, num - idx);
    num -= len;
    data[num] = '\0';
}

void __built_in::string::popb()
{
    remove(num - 1);
}

void __built_in::string::popf()
{
    remove(0);
}

const char *__built_in::string::to_string() const
{
    return data;
}

void __built_in::string::resize()
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
