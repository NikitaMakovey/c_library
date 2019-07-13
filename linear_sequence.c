#include <string.h>
#include <stdio.h>
#include "linear_sequence.h"

/* Неинициализированное значение итератора */
#define LSQ_IteratorInvalid NULL

typedef struct {
    LSQ_BaseTypeT* data;
    LSQ_IntegerIndexT size;
    LSQ_IntegerIndexT memory_factor;
} Container;

typedef enum {
    IS_DEREFERENCABLE,
    IS_BEFOREFIRST,
    IS_AFTERLAST
} IteratorState;

typedef struct {
    IteratorState state;
    Container* container;
    LSQ_IntegerIndexT index;
} Iterator;

/* Функция, создающая пустой контейнер. Возвращает назначенный ему дескриптор */
LSQ_HandleT LSQ_CreateSequence(void) {
    Container* container = (Container*) malloc(sizeof(Container));
    if (container == LSQ_HandleInvalid)
        return LSQ_HandleInvalid;
    else {
        container->data = NULL;
        container->size = 0;
        container->memory_factor = 0;
    }
    return container;
}

/* Функция, уничтожающая контейнер с заданным дескриптором. Освобождает принадлежащую ему память */
void LSQ_DestroySequence(LSQ_HandleT handle) {
    if (handle != LSQ_HandleInvalid) {
        free(((Container *) handle)->data);
        ((Container*) handle)->size = 0;
        ((Container*) handle)->memory_factor = 0;
    }
    free(handle);
}

/* Функция, возвращающая текущее количество элементов в контейнере */
LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle) {
    if (handle == LSQ_HandleInvalid)
        return 0;
    return ((Container*) handle)->size;
}

/* Функция, определяющая, может ли данный итератор быть разыменован */
int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator) {
    if (iterator == LSQ_IteratorInvalid)
        return 0;
    return ((Iterator*) iterator)->state == IS_DEREFERENCABLE;
}

/* Функция, определяющая, указывает ли данный итератор на элемент, следующий за последним в контейнере */
int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator) {
    if (iterator == LSQ_IteratorInvalid)
        return 0;
    return ((Iterator*) iterator)->state == IS_AFTERLAST;
}

/* Функция, определяющая, указывает ли данный итератор на элемент, предшествующий первому в контейнере */
int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator) {
    if (iterator == LSQ_IteratorInvalid)
        return 0;
    return ((Iterator*) iterator)->state == IS_BEFOREFIRST;
}

/* Функция разыменовывающая итератор. Возвращает указатель на элемент, на который ссылается данный итератор */
LSQ_BaseTypeT* LSQ_DereferenceIterator(LSQ_IteratorT iterator) {
    if (iterator == LSQ_IteratorInvalid)
        return LSQ_IteratorInvalid;
    return ((Iterator*) iterator)->container->data + ((Iterator*) iterator)->index;
}

/* Функция для создания итератора */
Iterator* CreateIterator(Container* container) {
    Iterator* iterator = (Iterator*) malloc(sizeof(Iterator));
    if (iterator == LSQ_IteratorInvalid)
        return LSQ_IteratorInvalid;
    iterator->container = (Container*) container;
    return iterator;
}

/* Функция для инициализирования итератора */
void InitIterator(Iterator* iterator) {
    if (iterator->index < 0) {
        iterator->state = IS_BEFOREFIRST;
        iterator->index = -1;
    } else if (iterator->index >= iterator->container->size) {
        iterator->state = IS_AFTERLAST;
        iterator->index = iterator->container->size;
    } else
        iterator->state = IS_DEREFERENCABLE;
}

/* Следующие три функции создают итератор в памяти и возвращают его дескриптор */
/* Функция, возвращающая итератор, ссылающийся на элемент с указанным индексом */
LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index) {
    if (handle == LSQ_HandleInvalid)
        return LSQ_HandleInvalid;
    Iterator* iterator = CreateIterator(handle);
    if (iterator == LSQ_IteratorInvalid)
        return LSQ_IteratorInvalid;
    iterator->index = index;
    InitIterator(iterator);
    return iterator;
}

/* Функция, возвращающая итератор, ссылающийся на первый элемент контейнера */
LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle) {
    if (handle == LSQ_HandleInvalid)
        return LSQ_HandleInvalid;
    return LSQ_GetElementByIndex(handle, 0);
}
/* Функция, возвращающая итератор, ссылающийся на последний элемент контейнера */
LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle) {
    if (handle == LSQ_HandleInvalid)
        return LSQ_HandleInvalid;
    return LSQ_GetElementByIndex(handle, ((Container*) handle)->size);
}

/* Функция, уничтожающая итератор с заданным дескриптором и освобождающая принадлежащую ему память */
void LSQ_DestroyIterator(LSQ_IteratorT iterator) {
    free(iterator);
}

/* Функция, перемещающая итератор на один элемент вперед */
void LSQ_AdvanceOneElement(LSQ_IteratorT iterator) {
    LSQ_ShiftPosition(iterator, 1);
}

/* Функция, перемещающая итератор на один элемент назад */
void LSQ_RewindOneElement(LSQ_IteratorT iterator) {
    LSQ_ShiftPosition(iterator, -1);
}

/* Функция, перемещающая итератор на заданное смещение со знаком */
void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift) {
    if (iterator == LSQ_IteratorInvalid)
        return;
    ((Iterator*) iterator)->index += shift;
    InitIterator(iterator);
}

/* Функция, устанавливающая итератор на элемент с указанным номером */
void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos) {
    if (iterator == LSQ_IteratorInvalid)
        return;
    ((Iterator*) iterator)->index = pos;
    InitIterator(iterator);
}

/* Функция, увеличивающая выделенную память */
static void ReallocMemory(Container* container) {
    if (container->memory_factor == 0) {
        container->memory_factor++;
        container->memory_factor += 2 * container->size;
    } else
        container->memory_factor = 2 * container->size;
    LSQ_BaseTypeT* memory = realloc(container->data, sizeof(LSQ_BaseTypeT) * container->memory_factor);
    if (memory != NULL)
        container->data = memory;
}

/* Функция, добавляющая элемент в начало контейнера */
void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    LSQ_IteratorT iterator = LSQ_GetFrontElement(handle);
    LSQ_InsertElementBeforeGiven(iterator, element);
    LSQ_DestroyIterator(iterator);
}

/* Функция, добавляющая элемент в конец контейнера */
void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
    LSQ_IteratorT iterator = LSQ_GetPastRearElement(handle);
    LSQ_InsertElementBeforeGiven(iterator, element);
    LSQ_DestroyIterator(iterator);
}

/* Функция, добавляющая элемент в контейнер на позицию, указываемую в данный момент итератором. Элемент, на который  *
 * указывает итератор, а также все последующие, сдвигается на одну позицию в конец.                                  */
void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement) {
    if (iterator == LSQ_IteratorInvalid)
        return;
    if (LSQ_IsIteratorBeforeFirst(iterator))
        return;
    if (((Iterator*) iterator)->container->size <= ((Iterator*) iterator)->container->memory_factor)
        ReallocMemory(((Iterator*) iterator)->container);
    LSQ_BaseTypeT* tmp = ((((Iterator*) iterator)->container)->data + ((Iterator*) iterator)->index);
    LSQ_BaseTypeT shift = ((((Iterator*) iterator)->container)->size - ((Iterator*) iterator)->index);
    memmove(tmp + 1, tmp, sizeof(LSQ_BaseTypeT) * shift);
    (((Iterator*) iterator)->container)->size++;
    *tmp = newElement;
}

/* Функция, удаляющая первый элемент контейнера */
void LSQ_DeleteFrontElement(LSQ_HandleT handle) {
    LSQ_IteratorT iterator = LSQ_GetFrontElement(handle);
    LSQ_DeleteGivenElement(iterator);
    LSQ_DestroyIterator(iterator);
}

/* Функция, удаляющая последний элемент контейнера */
void LSQ_DeleteRearElement(LSQ_HandleT handle) {
    LSQ_IteratorT iterator = LSQ_GetPastRearElement(handle);
    LSQ_RewindOneElement(iterator);
    LSQ_DeleteGivenElement(iterator);
    LSQ_DestroyIterator(iterator);
}

/* Функция, удаляющая элемент контейнера, указываемый заданным итератором. Все последующие элементы смещаются на     *
 * одну позицию в сторону начала.                                                                                    */
void LSQ_DeleteGivenElement(LSQ_IteratorT iterator) {
    if (iterator == LSQ_IteratorInvalid)
        return;
    if (!LSQ_IsIteratorDereferencable(iterator))
        return;
    ((Iterator*) iterator)->container->size--;
    LSQ_BaseTypeT *tmp = ((Iterator*) iterator)->container->data + ((Iterator*) iterator)->index;
    LSQ_BaseTypeT shift = ((((Iterator*) iterator)->container)->size - ((Iterator*) iterator)->index);
    memmove(tmp, tmp + 1, sizeof(LSQ_BaseTypeT) * shift);
}




