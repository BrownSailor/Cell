#include "std/util.h"
#include "std/array.h"
#include "std/string.h"
template <typename T>
struct node {
T val;
node<T>* next;
};
template <typename T>
struct list {
node<T>* front;
node<T>* back;
uint64_t size;
};
template <typename T>
struct queue {
list<T>* data;
};
template <typename T>
struct stack {
__array__<T> data;
};
uint64_t size(__array__<char>& arr) {
return arr.size();
}
void pushb(__array__<char>& arr, char val) {
arr.pushb(val);
}
char popb(__array__<char>& arr) {
char val = arr[(size(arr))-(1)];
arr.popb();
return val;
}
uint64_t size(stack<char>* stk) {
return size(stk->data);
}
void push(stack<char>* stk, char val) {
pushb(stk->data,val);
}
char pop(stack<char>* stk) {
return popb(stk->data);
}
char top(stack<char>* stk) {
return stk->data;
}
template <typename K,typename V>
struct pair {
K key;
V val;
};
template <typename K,typename V>
struct map {
__array__<pair<K,V>*> data;
};
int8_t solve() {
stack<char>* s = new stack<char>;
__string__ t = "()()()()()";
for (int64_t i = 0;(i)<((size(t))-(1));(i)++) {
if ((t)==('(')) {
push(s,t);
}
else if ((pop(s))!=('(')) {
return fls;
}
}
return (size(s))==(0);
}
int32_t main(int32_t argc, char** argv) {
printf("%lld\n", (long long)(solve()));
}
