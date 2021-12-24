#ifndef CELL_VALUE_H
#define CELL_VALUE_H

typedef int Value;

typedef struct ValueArray
{
    int size;
    int capacity;
    Value *values;
} ValueArray;

void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);
void printValue(Value value);

#endif