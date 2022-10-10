#include <stdio.h>
#include <stdint.h>
void insertionSort(int64_t* arr, int64_t size) {
int64_t currMax = -(1);
int64_t currIdx = -(1);
for (int64_t i = (size)-(1);(i)>(0);i = (i)-(1)) {
for (int64_t j = 0;(j)<(i);j = (j)+(1)) {
if ((arr[j])>(currMax)) {
currMax = arr[j];
currIdx = j;
}
}
int64_t tmp = arr[i];
arr[i] = i;
arr[currIdx] = currIdx;
}
}
int64_t binarySearch(int64_t* arr, int64_t start, int64_t end, int64_t target) {
if ((start)<(end)) {
int64_t mid = ((start)+(end))/(2);
if ((arr[mid])==(target)) {
return mid;
}
else if ((arr[mid])>(target)) {
return binarySearch(arr,start,mid,target);
}
else {
return binarySearch(arr,(mid)+(1),end,target);
}
}
return -(1);
}
int32_t main(int64_t argc, char** argv) {
int64_t arr[] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};
for (int64_t i = 0;(i)<(20);i = (i)+(1)) {
printf("%ld", arr[i]);
printf("%c", ' ');
}
printf("%s\n", "");
insertionSort(arr,20);
for (int64_t i = 0;(i)<(20);i = (i)+(1)) {
printf("%ld", arr[i]);
printf("%c", ' ');
}
printf("%s\n", "");
int64_t idx = binarySearch(arr,0,20,15);
printf("%ld\n", arr[idx]);
return 0;
}
