#include <stdio.h>
#include "linear_sequence.h"

int main() {
    LSQ_HandleT array = LSQ_CreateSequence();
    printf("Size = %d\n", LSQ_GetSize(array));
    for (LSQ_BaseTypeT i = 0; i < 5; i++){
        LSQ_InsertRearElement(array, i + 1);
    }
    printf("Size = %d\nArray: ", LSQ_GetSize(array));
    for (LSQ_IntegerIndexT i = 0; i < LSQ_GetSize(array); i++){
        printf("%d ", *LSQ_DereferenceIterator(LSQ_GetElementByIndex(array, i)));
    }
    LSQ_IteratorT it = LSQ_GetElementByIndex(array, 0);
    printf("\n1st element = %d\n", *LSQ_DereferenceIterator(it));
    LSQ_AdvanceOneElement(it);
    printf("2st element = %d\n", *LSQ_DereferenceIterator(it));
    LSQ_AdvanceOneElement(it);
    LSQ_AdvanceOneElement(it);
    LSQ_AdvanceOneElement(it);
    printf("5st element = %d\n", *LSQ_DereferenceIterator(it));
    LSQ_AdvanceOneElement(it);
    printf("We have next element (0 - no/ 1 - yes) => %d\n", LSQ_IsIteratorDereferencable(it));
    printf("AFTERLAST ? => %d\n", LSQ_IsIteratorPastRear(it));
    LSQ_RewindOneElement(it);
    printf("AFTERLAST ? => %d\n", LSQ_IsIteratorPastRear(it));
    printf("Size = %d\n", LSQ_GetSize(array));
    for (LSQ_BaseTypeT i = LSQ_GetSize(array) - 1; i > 0; i--)
        LSQ_RewindOneElement(it);
    printf("BEFOREFIRST ? => %d\n", LSQ_IsIteratorBeforeFirst(it));
    LSQ_RewindOneElement(it);
    printf("BEFOREFIRST ? => %d\n", LSQ_IsIteratorBeforeFirst(it));
    LSQ_DeleteFrontElement(array);
    printf("Array : ");
    for (LSQ_IntegerIndexT i = 0; i < LSQ_GetSize(array); i++){
        printf("%d ", *LSQ_DereferenceIterator(LSQ_GetElementByIndex(array, i)));
    }
    LSQ_DeleteRearElement(array);
    printf("\nArray : ");
    for (LSQ_IntegerIndexT i = 0; i < LSQ_GetSize(array); i++){
        printf("%d ", *LSQ_DereferenceIterator(LSQ_GetElementByIndex(array, i)));
    }
    LSQ_DeleteRearElement(array);
    LSQ_DeleteFrontElement(array);
    LSQ_DeleteRearElement(array);
    printf("\nArray : ");
    if (LSQ_GetSize(array)) {
        for (LSQ_IntegerIndexT i = 0; i < LSQ_GetSize(array); i++) {
            printf("%d ", *LSQ_DereferenceIterator(LSQ_GetElementByIndex(array, i)));
        }
    } else
        printf("EMPTY\n");
    LSQ_InsertFrontElement(array, 7);
    printf("Array : ");
    if (LSQ_GetSize(array)) {
        for (LSQ_IntegerIndexT i = 0; i < LSQ_GetSize(array); i++) {
            printf("%d ", *LSQ_DereferenceIterator(LSQ_GetElementByIndex(array, i)));
        }
    } else
        printf("EMPTY\n");
    LSQ_IteratorT iterator = LSQ_GetElementByIndex(array, 0);
    LSQ_InsertFrontElement(array, 666);
    LSQ_InsertFrontElement(array, 69);
    LSQ_InsertFrontElement(array, 36);
    printf("\nArray: ");
    for (LSQ_IntegerIndexT i = 0; i < LSQ_GetSize(array); i++){
        printf("%d ", *LSQ_DereferenceIterator(LSQ_GetElementByIndex(array, i)));
    }
    LSQ_IteratorT iter = LSQ_GetPastRearElement(array);
    //LSQ_SetPosition(iterator, LSQ_GetSize(array) - 1);
    LSQ_InsertRearElement(array, 3);
    printf("!!! AFTER LAST ? %d ", (LSQ_IsIteratorPastRear(iter)));
    LSQ_SetPosition(iterator, 2);
    printf("\n2 position -> %d\n", *LSQ_DereferenceIterator(iterator));
    LSQ_DestroySequence(array);
    printf("Size of array = %d", LSQ_GetSize(array));
    return 0;
}