#include "array.h"
#include "string.h"
#include <cstdio>

void test_string()
{
    printf("=====================\nSTART OF STRING TESTS\n======================\n\n");

    __built_in::string str;
    __built_in::string str1("str1");
    __built_in::string str2 = "str2";

    printf("Str1: %s\n", str1.to_string());
    printf("Str2: %s\n", str2.to_string());

    __built_in::string str3 = str1 + '3';
    printf("Str3: %s\n", str3.to_string());

    __built_in::string str4 = str1 + str2;
    printf("Str4: %s\n", str4.to_string());

    __built_in::string str5 = str1 + " " + str2;
    printf("Str5: %s\n", str5.to_string());

    str1 += " is ";
    printf("Str1: %s\n", str1.to_string());

    str2 += '!';
    printf("Str2: %s\n", str2.to_string());

    str += str1 + str2;
    printf("Str: %s\n", str.to_string());

    str.insert(4, "insertion!");
    printf("Str: %s\n", str.to_string());

    str1.insert(5, str);
    printf("Str1: %s\n", str1.to_string());

    __built_in::string str6 = "str6";
    str6.pushb(' ');
    str6.pushb("is not ");
    str6.pushb(str3);
    printf("Str6: %s\n", str6.to_string());

    str6.remove(0, 4);
    str6.popf();
    printf("Str6: %s\n", str6.to_string());

    printf("\n===================\nEND OF STRING TESTS\n====================\n\n");
}

void test_array()
{
    printf("====================\nSTART OF ARRAY TESTS\n=====================\n\n");

    __built_in::array<int> arr1;
    __built_in::array<int> arr2(10);
    printf("%ld\n", arr2.size());

    for (int i = 0; i < arr2.size(); i++)
    {
        arr2[i] = 10 - i;
    }

    for (int i = 0; i < arr2.size(); i++)
    {
        printf("%ld ", arr2[i]);
    }
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        arr1.pushf(i);
    }

    for (int i = 0; i < arr1.size(); i++)
    {
        printf("%ld ", arr1[i]);
    }
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        printf("%ld ", arr1[arr1.size() - 1]);
        arr1.popb();
    }
    printf("\n");
    printf("%ld\n", arr1.size());

    __built_in::array<int> arr3 = __built_in::array<int>({1, 2, 3, 4, 5});
    printf("%ld\n", arr3.size());

    for (int i = 0; i < arr3.size(); i++)
    {
        printf("%ld ", arr3[i]);
    }
    printf("\n");

    for (int i = 0; i < arr3.size(); i++)
    {
        arr3[i] = i * 10;
    }

    for (int i = 0; i < arr3.size(); i++)
    {
        printf("%ld ", arr3[i]);
    }
    printf("\n");

    __built_in::array<__built_in::array<char>> arr4 = __built_in::array<__built_in::array<char>>({ __built_in::array<char>({ 'a', 'b', 'c' }), __built_in::array<char>({ 'd', 'e', 'f' }) });
    for (int i = 0; i < arr4.size(); i++)
    {
        for (int j = 0; j < arr4[0].size(); j++)
        {
            printf("%c ", arr4[i][j]);
        }
        printf("\n");
    }

    arr4.insert(0, __built_in::array<char>({ '0', '1', '2' }));
    for (int i = 0; i < arr4.size(); i++)
    {
        for (int j = 0; j < arr4[0].size(); j++)
        {
            printf("%c ", arr4[i][j]);
        }
        printf("\n");
    }

    arr4.remove(1);
    for (int i = 0; i < arr4.size(); i++)
    {
        for (int j = 0; j < arr4[0].size(); j++)
        {
            printf("%c ", arr4[i][j]);
        }
        printf("\n");
    }

    printf("\n==================\nEND OF ARRAY TESTS\n===================\n\n");
}

int main()
{
    test_string();
    test_array();

    return 0;
}
