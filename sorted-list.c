/*
 * sorted-list.c
 */

#include    <string.h>
#include    <stdlib.h>
#include    "sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf) {
    
    SortedListPtr list;
    list = malloc(sizeof(SortedListPtr));
    Node *front = malloc(sizeof(Node));
    front->refCount = 0;
    front->next = NULL;
    front->data = NULL;
    list->cf = cf;
    list->size=0;
    list->front = front;
    return list;
    
}

void SLDestroy(SortedListPtr list) {
    
    
    Node *node = list->front;
    for(node; node!=NULL; node = node->next){
        list->front = list->front->next;
        free(node);
    }

    free(list);
}


int SLInsert(SortedListPtr list, void* newObj) {

    Node *new_node = malloc(sizeof(Node));
    Node *curr = list->front;

    if (list->size == 0) { //first item
        new_node->data = newObj;
        new_node->next = NULL;
        list->front = new_node;
        list->size++;
        return 1;
    }
    
    if (list->cf(newObj, curr->data) > 0) { //item will be new front
        new_node->data = newObj;
        new_node->next = curr;
        curr->refCount++;
        list->front = new_node;
        list->size++;
        return 1;
    }
    
    
    while (curr != NULL) {
        
        
        if (list->cf(newObj, curr->data) > 0) {
            
            new_node->data = curr->data;
            new_node->next = curr->next;
            new_node->refCount = curr->refCount;
            curr->data = newObj;
            curr->next = new_node;
            curr->refCount = 1;
            list->size++;
            return 1;
            
        }
        else if (list->cf(newObj, curr->data) < 0) {
            if (curr->next == NULL) { //last item
                new_node->data = newObj;
                new_node->next = curr->next;
                new_node->refCount = 1;
                curr->next = new_node;
                list->size++;
                return 1;
            }
            curr = curr->next;
        }
        else {
            new_node->data = newObj;
            new_node->next = curr->next;
            new_node->refCount = 1;
            curr->next = new_node;
            list->size++;
            return 1;
            
        }
    }
    
    
    return 0;
}

int SLRemove(SortedListPtr list, void* newObj) {
    
    Node *curr = list->front;
    Node *after = curr->next;
    
    if(list->size==0){
        printf("ERROR: List is empty. \n");
        return 0;
    }
    
    if (list->cf(curr->data, newObj)==0) {
        
        if (list->size == 1) {
            // free(list->front);
            list->front = NULL;
            list->size--;
            return 1;
        } else {
            list->front = curr->next;
            list->size--;
            free(curr);
            list->front->refCount--;
            return 1;
        }
    }
    
    curr = list->front;
    for (curr; curr->next != NULL; curr=curr->next) {
        Node *tmpNext = curr->next;
        if (list->cf(newObj, tmpNext->data) == 0) {
            curr->next = tmpNext->next;
            free(tmpNext);
            list->size--;
            return 1;
        }
    }
    
    printf("ERROR: Object not in list. \n");
    return 0;

}

SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
    SortedListIteratorPtr iter = malloc(sizeof(SortedListIteratorPtr));
    iter->curr = list->front;
    iter->front = list->front;
    iter->curr->refCount++;
    return iter;
}

void SLDestroyIterator(SortedListIteratorPtr iter) {
    free(iter);
}

void *SLNextItem(SortedListIteratorPtr iter) { //what happens if i'm at the end?? loop back to front?

    Node *ret = iter->curr;
    
    iter->curr->refCount--;
    iter->curr = iter->curr->next;
    if (iter->curr == NULL) {
        iter->curr = iter->front;
    }
    iter->curr->refCount++;
    return ret;
}

