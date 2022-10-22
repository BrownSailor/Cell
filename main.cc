#include "std/util.h"
#include "std/array.h"
#include "std/string.h"
int64_t size(__built_in::array<int64_t>& arr) {
return arr.size();
}
void insertionSort(__built_in::array<int64_t>& arr) {
for (int64_t i = (size(arr))-(1);(i)>(0);(i)--) {
int64_t currIdx = -(1);
int64_t currMax = -(1);
for (int64_t j = 0;(j)<=(i);(j)++) {
if ((arr[j])>(currMax)) {
currMax = arr[j];
currIdx = j;
}
}
int64_t tmp = arr[i];
arr[i] = currMax;
arr[currIdx] = tmp;
}
}
int64_t binarySearch(__built_in::array<int64_t>& arr, int64_t start, int64_t end, int64_t target) {
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
int32_t main(int32_t argc, char** argv) {
__built_in::array<int64_t> arr = __built_in::array<int64_t>({20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1});
insertionSort(arr);
printf("%lld\n", arr[binarySearch(arr,0,20,binarySearch(arr,0,20,15))]);
return 0;
}
