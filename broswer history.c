
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_URL_LENGTH 100



typedef struct HistoryNode {
    char url[MAX_URL_LENGTH];
    struct HistoryNode* prev;
    struct HistoryNode* next;
} HistoryNode;

HistoryNode* head = NULL;
HistoryNode* tail = NULL;
HistoryNode* current = NULL;

void insertPage(const char* url);
void deletePage(const char* url);
void displayHistory();
HistoryNode* linearSearch(const char* url);
void sortHistory();
void navigateForward();
void navigateBackward();
void clearHistory();


HistoryNode* sortedMerge(HistoryNode* a, HistoryNode* b);
void splitList(HistoryNode* source, HistoryNode** frontRef, HistoryNode** backRef);
HistoryNode* mergeSort(HistoryNode* h);



int main() {
    int choice;
    char url[MAX_URL_LENGTH];

    printf("Welcome to the Console Browser History Manager!\n");

    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Visit a new page\n");
        printf("2. Delete a page by URL\n");
        printf("3. Display browsing history\n");
        printf("4. Search for a page (Linear Search)\n");
        printf("5. Sort history by URL (Merge Sort)\n");
        printf("6. Navigate forward\n");
        printf("7. Navigate backward\n");
        printf("8. Clear all history\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        while (getchar() != '\n');

        switch (choice) {
            case 1:
                printf("Enter the URL to visit: ");
                scanf("%s", url);
                insertPage(url);
                printf("Visited page: %s\n", url);
                break;
            case 2:
                printf("Enter the URL to delete: ");
                scanf("%s", url);
                deletePage(url);
                break;
            case 3:
                displayHistory();
                break;
            case 4: {
                printf("Enter the URL to search for: ");
                scanf("%s", url);
                HistoryNode* foundNode = linearSearch(url);
                if (foundNode) {
                    printf("Found '%s' in history.\n", foundNode->url);
                } else {
                    printf("'%s' not found in history.\n", url);
                }
                break;
            }
            case 5:
                sortHistory();
                printf("History has been sorted.\n");
                break;
            case 6:
                navigateForward();
                break;
            case 7:
                navigateBackward();
                break;
            case 8:
                clearHistory();
                printf("All browsing history has been cleared.\n");
                break;
            case 9:
                printf("Exiting Browser History Manager. Goodbye!\n");
                clearHistory();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);

    return 0;
}




void insertPage(const char* url) {
    
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (newNode == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    strcpy(newNode->url, url);
    newNode->next = NULL;

    if (head == NULL) {
        newNode->prev = NULL;
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }

    current = newNode;
}


void deletePage(const char* url) {
    HistoryNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->url, url) == 0) {

            if (temp->prev != NULL) {
                temp->prev->next = temp->next;
            } else {
                head = temp->next;
            }

            if (temp->next != NULL) {
                temp->next->prev = temp->prev;
            } else {
                tail = temp->prev;
            }

            if (temp == current) {
                if (current->prev != NULL) {
                    current = current->prev;
                } else if (current->next != NULL) {
                    current = current->next;
                } else {
                    current = NULL;
                }
            }

            free(temp);
            printf("Successfully deleted '%s' from history.\n", url);
            return;
        }
        temp = temp->next;
    }
    printf("'%s' not found in history.\n", url);
}


void displayHistory() {
    if (head == NULL) {
        printf("History is empty.\n");
        return;
    }

    printf("\n--- Browsing History ---\n");
    HistoryNode* temp = head;
    while (temp != NULL) {
        if (temp == current) {
            printf("-> %s (Current Page)\n", temp->url);
        } else {
            printf("   %s\n", temp->url);
        }
        temp = temp->next;
    }
}


HistoryNode* linearSearch(const char* url) {
    HistoryNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->url, url) == 0) {
            return temp; // Found the URL
        }
        temp = temp->next;
    }
    return NULL; // Not found
}


void sortHistory() {
    if (head == NULL || head->next == NULL) {
        return;
    }

    head = mergeSort(head);

    HistoryNode* temp = head;
    HistoryNode* prevNode = NULL;
    while (temp != NULL) {
        temp->prev = prevNode;
        prevNode = temp;
        temp = temp->next;
    }
    tail = prevNode;
}


HistoryNode* mergeSort(HistoryNode* h) {
    
    if (h == NULL || h->next == NULL) {
        return h;
    }

    HistoryNode* a;
    HistoryNode* b;
    splitList(h, &a, &b);

    a = mergeSort(a);
    b = mergeSort(b);

    return sortedMerge(a, b);
}


HistoryNode* sortedMerge(HistoryNode* a, HistoryNode* b) {

    if (a == NULL) return b;
    if (b == NULL) return a;

    HistoryNode* result = NULL;

    if (strcmp(a->url, b->url) <= 0) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }
    return result;
}


void splitList(HistoryNode* source, HistoryNode** frontRef, HistoryNode** backRef) {
    HistoryNode* fast;
    HistoryNode* slow;
    slow = source;
    fast = source->next;

    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

void navigateForward() {
    if (current == NULL || current->next == NULL) {
        printf("Cannot navigate forward. You are at the end of the history.\n");
    } else {
        current = current->next;
        printf("Moved forward to: %s\n", current->url);
    }
}

void navigateBackward() {
    if (current == NULL || current->prev == NULL) {
        printf("Cannot navigate backward. You are at the beginning of the history.\n");
    } else {
        current = current->prev;
        printf("Moved backward to: %s\n", current->url);
    }
}


void clearHistory() {
    HistoryNode* temp = head;
    HistoryNode* nextNode;
    while (temp != NULL) {
        nextNode = temp->next;
        free(temp);
        temp = nextNode;
    }
    head = NULL;
    tail = NULL;
    current = NULL;
}
