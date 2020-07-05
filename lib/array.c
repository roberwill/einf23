#include "array.h"

ArrayClass array_build(){
    ArrayClass obj;
    
    obj.init = array_init;
    obj.destroy = array_destroy;
    obj.is_empty = array_is_empty;
    obj.len = array_len;
    obj.unshift = array_unshift;
    obj.push = array_push;
    obj.shift = array_shift;
    obj.pop = array_pop;
    
    return obj;
}

OArray array_init(ArrayType type) {
    OArray array = (OArray)malloc(sizeof(_array));
    // set the array type
    array->type = type;
    // The head and tail of the array point to nil
    array->start = array->end = nil;
    // The length o f the array is set to 0
    array->len = 0;

    return array;
}

void array_print(OArray array) {
    if(array_is_empty(array)) {
        printf("[]\n");
        return;
    }

    Node node = array->start; 
    int len = array_len(array);


    for (int i = 0; i < len; i++) {
        if (i == 0)
            printf("[%d, ", node->item.Int);
        else if (i == len - 1)
            printf("%d]", node->item.Int);
        else
            printf("%d, ", node->item.Int);

        node = node->next;
    }

    printf("\n");
}

void array_destroy(OArray array) {
    Node node = array->start, temp;

    //Loop every node of the array and free one at a time starting from the first one
    for (int i = 0; i < array_len(array); i++) {
        // Get the next node
        temp = node->next;
        // Free the previous node
        free(node);
        // Set the next node (used in the next iteration)
        node = temp;
    }
}

int array_is_empty(OArray array) {
    // Check whether the array is empty or not
    return array->len == 0 && array->start == nil && array->end == nil;
}

int array_len(OArray array) {
    // Returns the length of the array
    return array->len;
}

void array_item_type_decode(Node node, void* item, ArrayType type) {
    switch(type) {
        case ArrayInt:
            node->item.Int = *(int*)item;
        case ArrayFloat:
            node->item.Float = *(float*)item;
        case ArrayDouble:
            node->item.Double = *(double*)item;
        case ArrayChar:
            node->item.Char = *(char*)item;
        break;
    }
}

int array_unshift(OArray array, void* item) {
    // Create the node to be inserted
    Node node = (Node)malloc(sizeof(array_node));
    // If, somehow, memory is not available
    if (node == nil) return FALSE;

    // Set the node item
    array_item_type_decode(node, item, array->type);
    node->index = array_len(array);

    // The new head points to no previous node
    node->previous = nil;

    // if the array is empty
    if (array_is_empty(array)) {
        // The head is the tail
        array->end = node;
        // the head and the tail have no next or previous node
        node->next = nil;
    } else {
        // The new node now points to the old head
        node->next = array->start;
        // The old head has the new head as the previous node
        array->start->previous = node;
    }

    // The new node becomes the head
    array->start = node;
    // The length of the array increases
    array->len++;

    return TRUE;
}

int array_push(OArray array, void* item) {
    // Create the node to be inserted
    Node node = (Node)malloc(sizeof(array_node));
    // If, somehow, memory is not available
    if (node == nil) return FALSE;
    
    // Set the node item
    array_item_type_decode(node, item, array->type);
    
    // Since it is an insertion at the end, the node has to point to nowhere
    node->next = nil;

    if (array_is_empty(array)) {
        // if the array is empty, the head and the tail have to be the same
        array->start = node;
        // There is no previous node to point to
        node->previous = nil;
    } else {
        // the old tail has as next node the new tail 
        array->end->next = node;
        // The new tail has as previous node the old tail
        node->previous = array->end;
    }

    // The new node becomes the tail
    array->end = node;
    // The length of the array increases
    array->len++;

    return TRUE;
}

int array_shift(OArray array) {
    // If the array is empty, there is nothing to remove
    if (array_is_empty(array)) return FALSE;

    // Get the first node
    Node first_node = array->start;

    if (array->start == array->end)
        // if array is of length 1, then the head and the tail must point to nowhere
        array->start = array->end = nil;
    else {
        // the node pointed by the old head becomes the head
        array->start = array->start->next;
        // the first node has no previous node
        array->start->previous =  nil;
    }

    // Free the old head
    free(first_node);
    // The length of the array decreases
    array->len--;

    return TRUE;
}

int array_pop(OArray array) {
    // If the array is empty, there is nothing to remove
    if (array_is_empty(array)) return FALSE;

    // If the array is of size of 1
    if (array->start == array->end) {
        // Free the single node
        free(array->start);
        // Head an tail point to nowhere
        array->start = array->end = nil;
    } else {
        // Get the last node
        Node last_node = array->end;
        // The penultimate node, which is the previous node of the last node, is now the last node 
        array->end = last_node->previous;
        // The new tail has no next node
        array->end->next = nil;
        // Free the old tail
        free(last_node);
    }

    // The length of the array decreases
    array->len--;

    return TRUE;
}

