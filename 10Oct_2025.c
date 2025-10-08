#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 3   // Maximum keys per node (order = MAX+1 children)

// ---------------- Node Structure ----------------
typedef struct BPlusNode {
    int keys[MAX];
    struct BPlusNode* children[MAX + 1];
    struct BPlusNode* next;  // Used for leaf node linked list
    bool isLeaf;
    int keyCount;
} BPlusNode;

BPlusNode* root = NULL;

// ---------------- Utility Functions ----------------
BPlusNode* createNode(bool isLeaf) {
    BPlusNode* node = (BPlusNode*)malloc(sizeof(BPlusNode));
    node->isLeaf = isLeaf;
    node->keyCount = 0;
    node->next = NULL;
    for (int i = 0; i <= MAX; i++) node->children[i] = NULL;
    return node;
}

// Search a key
bool search(int key, BPlusNode* cursor) {
    if (!cursor) return false;

    int i = 0;
    while (i < cursor->keyCount && key > cursor->keys[i]) i++;

    if (cursor->isLeaf) {
        if (i < cursor->keyCount && cursor->keys[i] == key) return true;
        return false;
    }
    return search(key, cursor->children[i]);
}

// ---------------- Split Functions ----------------
void splitChild(BPlusNode* parent, int index) {
    BPlusNode* child = parent->children[index];
    int mid = child->keyCount / 2;

    BPlusNode* newNode = createNode(child->isLeaf);
    newNode->keyCount = child->keyCount - mid - 1;

    for (int i = 0; i < newNode->keyCount; i++)
        newNode->keys[i] = child->keys[mid + 1 + i];

    if (!child->isLeaf) {
        for (int i = 0; i <= newNode->keyCount; i++)
            newNode->children[i] = child->children[mid + 1 + i];
    }

    if (child->isLeaf) {
        newNode->keyCount = child->keyCount - mid;
        for (int i = 0; i < newNode->keyCount; i++)
            newNode->keys[i] = child->keys[mid + i];
        newNode->next = child->next;
        child->next = newNode;
        child->keyCount = mid;
    } else {
        child->keyCount = mid;
    }

    for (int i = parent->keyCount; i > index; i--)
        parent->children[i + 1] = parent->children[i];

    parent->children[index + 1] = newNode;

    for (int i = parent->keyCount; i > index; i--)
        parent->keys[i] = parent->keys[i - 1];

    parent->keys[index] = child->keys[mid];
    parent->keyCount++;
}

// ---------------- Insert Functions ----------------
void insertNonFull(BPlusNode* node, int key) {
    int i = node->keyCount - 1;

    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->keyCount++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        if (node->children[i]->keyCount == MAX) {
            splitChild(node, i);
            if (key > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], key);
    }
}

void insert(int key) {
    if (!root) {
        root = createNode(true);
        root->keys[0] = key;
        root->keyCount = 1;
        return;
    }

    if (root->keyCount == MAX) {
        BPlusNode* newRoot = createNode(false);
        newRoot->children[0] = root;
        splitChild(newRoot, 0);
        int i = (key > newRoot->keys[0]) ? 1 : 0;
        insertNonFull(newRoot->children[i], key);
        root = newRoot;
    } else {
        insertNonFull(root, key);
    }
}

// ---------------- Display Function ----------------
void display(BPlusNode* cursor) {
    if (!cursor) return;

    for (int i = 0; i < cursor->keyCount; i++)
        printf("%d ", cursor->keys[i]);
    printf("| ");

    if (!cursor->isLeaf) {
        for (int i = 0; i <= cursor->keyCount; i++)
            display(cursor->children[i]);
    }
}

// ---------------- Main Driver ----------------
int main() {
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++)
        insert(keys[i]);

    printf("B+ Tree Index:\n");
    display(root);
    printf("\n");

    int searchKey = 12;
    if (search(searchKey, root))
        printf("Key %d found in B+ Tree.\n", searchKey);
    else
        printf("Key %d not found in B+ Tree.\n", searchKey);

    return 0;
}