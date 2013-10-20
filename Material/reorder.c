/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Inserts integers provided as command-line arguments into       *
 *              a linked list and reorders them, first 0, then positive        *
 *              numbers from largest to smallest, and then negative numbers    *
 *              from smallest to largest, without duplicates.                  *
 *                                                                             *
 * Written by *** for COMP9021                                                 *
 *                                                                             *
 * Other source files, if any, one per line, starting on the next line:        *
 *       tailored_list.c                                                       *
 *       quiz9.c                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* THIS IS THE ONLY FILE YOU HAVE TO SUBMIT.
 * IT WILL BE COMPILED WITH THE OTHER FILES YOU HAVE BEEN PROVIDED WITH. */

#include "reorder.h"
#include <stdio.h>
#include <stdlib.h>

void special_zero(Node **const pt_to_pt_to_node); 
void positive_list(Node **const pt_to_pt_to_node);
void negative_list(Node **const pt_to_pt_to_node); 
void filtering(Node **const pt_to_pt_to_node); 
Node* remove_node(Node **const pt_to_pt_to_node, Node *address); 
void moving_front(Node **const pt_to_pt_to_node, Node* target); 

void reorder(Node **const pt_to_pt_to_node) {
    filtering(pt_to_pt_to_node);
    special_zero(pt_to_pt_to_node);
    positive_list(pt_to_pt_to_node);
    negative_list(pt_to_pt_to_node);
}

void filtering(Node **const pt_to_pt_to_node) {
    int len = list_length(*pt_to_pt_to_node);
    Node *pt_org = *pt_to_pt_to_node;
    for (int i = 1; i <= len; i++) {
        Node *pt_local_org = *pt_to_pt_to_node;
        for (int j = 1; j <= len; j++) {
            if (pt_local_org != pt_org) {
                if (pt_local_org->value == pt_org->value) {
                    Node* duplicate = pt_local_org;
                    pt_local_org = remove_node(pt_to_pt_to_node, duplicate);  
                    len = list_length(*pt_to_pt_to_node);
                    j--;
                }
            }
            pt_local_org = pt_local_org->pt_to_next_node;
        }
        pt_org = pt_org->pt_to_next_node;
    }
}

Node* remove_node(Node **const pt_to_pt_to_node, Node *address) {
    Node *pt_org = *pt_to_pt_to_node;   
    while (pt_org->pt_to_next_node != address)
        pt_org = pt_org->pt_to_next_node;
    (*pt_org).pt_to_next_node = (*address).pt_to_next_node;
    free(address);
    return pt_org;
}

void special_zero(Node **const pt_to_pt_to_node) {
    Node* pt_org = *pt_to_pt_to_node;
    int len = list_length(*pt_to_pt_to_node);
    if ((!(pt_org->pt_to_next_node)) || pt_org->value == 0)
        return;
    for (int i = 1; i <= len; i++) {
        if (pt_org->value == 0) {
            while (*pt_to_pt_to_node != pt_org) 
                moving_front(pt_to_pt_to_node, pt_org);
            break;
        }
        if (pt_org->pt_to_next_node)
            pt_org = pt_org->pt_to_next_node;
    }
}  

void moving_front(Node **const pt_to_pt_to_node, Node* target) {
    Node *pt_previous = *pt_to_pt_to_node, *pt_before_prev;
    bool flag = true;
    while (pt_previous->pt_to_next_node != target)
        pt_previous = pt_previous->pt_to_next_node;
    if (pt_previous != *pt_to_pt_to_node) {
        flag = false;
        pt_before_prev = *pt_to_pt_to_node;
        while (pt_before_prev->pt_to_next_node != pt_previous) 
            pt_before_prev = pt_before_prev->pt_to_next_node; 
    }
    if (flag) {
        Node* store = NULL;
        *pt_to_pt_to_node = target;
        store = target->pt_to_next_node;
        target->pt_to_next_node = pt_previous;
        pt_previous->pt_to_next_node = store;
    }
    else {
        Node* store = NULL;
        store = target->pt_to_next_node; 
        pt_before_prev->pt_to_next_node = target;
        target->pt_to_next_node = pt_previous;
        pt_previous->pt_to_next_node = store;
    }
}
void positive_list(Node **const pt_to_pt_to_node) {
    Node* pt_org = *pt_to_pt_to_node;
    int len = list_length(*pt_to_pt_to_node);
    bool change = true;
    if (len < 2)
        return;
    while (change) {
        change = false;
        for (int i = 1; i <= len; i++) {    
            if ((pt_org->value > 0) && (pt_org != *pt_to_pt_to_node)) {
                Node *prev = *pt_to_pt_to_node;
                while (prev->pt_to_next_node != pt_org)
                    prev = prev->pt_to_next_node;
                if (prev->value == 0) {
                    pt_org = pt_org->pt_to_next_node; 
                    continue;
                }
                if (prev->value < pt_org->value) { 
                    change = true;
                    moving_front(pt_to_pt_to_node, pt_org);
                    break;
                }
            }
            pt_org = pt_org->pt_to_next_node; 
        }
        pt_org = *pt_to_pt_to_node;
    }
}
void negative_list(Node **const pt_to_pt_to_node) {
    Node* pt_org = *pt_to_pt_to_node;
    int len = list_length(*pt_to_pt_to_node);
    bool change = true;
    if (len < 2)
        return;
    while (change) {
        change = false;
        for (int i = 1; i <= len; i++) {    
            if ((pt_org->value < 0) && (pt_org != *pt_to_pt_to_node)) {
                Node *prev = *pt_to_pt_to_node;
                while (prev->pt_to_next_node != pt_org)
                    prev = prev->pt_to_next_node;
                if (prev->value == 0) {
                    pt_org = pt_org->pt_to_next_node; 
                    continue;
                }
                if ((prev->value > pt_org->value) && (prev->value < 0)){ 
                    change = true;
                    moving_front(pt_to_pt_to_node, pt_org);
                    break;
                }
            }
            pt_org = pt_org->pt_to_next_node; 
        }
        pt_org = *pt_to_pt_to_node;
    }
}

