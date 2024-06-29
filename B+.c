#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 3  // Maximum number of keys in a node (degree of the B+ tree)
#define MIN (MAX + 1) / 2  // Minimum number of keys in a node

typedef struct sv {
    char hoten[40];
    int tuoi;
    char mssv[20];
} sv;

typedef struct BPlusTreeNode {
    int n; // Number of keys in the node
    sv keys[MAX];
    struct BPlusTreeNode* children[MAX + 1];
    struct BPlusTreeNode* next; // Pointer to the next leaf node
    int isLeaf;
} BPlusTreeNode;

BPlusTreeNode* createNode(int isLeaf) {
    BPlusTreeNode* newNode = (BPlusTreeNode*)malloc(sizeof(BPlusTreeNode));
    newNode->n = 0;
    newNode->isLeaf = isLeaf;
    newNode->next = NULL;
    for (int i = 0; i < MAX + 1; i++) {
        newNode->children[i] = NULL;
    }
    return newNode;
}
void insertNonFull(BPlusTreeNode* node, sv key);
void splitChild(BPlusTreeNode* parent, int i, BPlusTreeNode* child);

void insert(BPlusTreeNode** root, sv key) {
    BPlusTreeNode* r = *root;
    if (r->n == MAX) {
        BPlusTreeNode* s = createNode(0);
        *root = s;
        s->children[0] = r;
        splitChild(s, 0, r);
        insertNonFull(s, key);
    } else {
        insertNonFull(r, key);
    }
}

void insertNonFull(BPlusTreeNode* node, sv key) {
    int i = node->n - 1;
    if (node->isLeaf) {
        while (i >= 0 && strcmp(key.hoten, node->keys[i].hoten) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->n++;
    } else {
        while (i >= 0 && strcmp(key.hoten, node->keys[i].hoten) < 0) {
            i--;
        }
        i++;
        if (node->children[i]->n == MAX) {
            splitChild(node, i, node->children[i]);
            if (strcmp(key.hoten, node->keys[i].hoten) > 0) {
                i++;
            }
        }
        insertNonFull(node->children[i], key);
    }
}

void splitChild(BPlusTreeNode* parent, int i, BPlusTreeNode* child) {
    BPlusTreeNode* newNode = createNode(child->isLeaf);
    newNode->n = MIN - 1;
    for (int j = 0; j < MIN - 1; j++) {
        newNode->keys[j] = child->keys[j + MIN];
    }
    if (!child->isLeaf) {
        for (int j = 0; j < MIN; j++) {
            newNode->children[j] = child->children[j + MIN];
        }
    }
    child->n = MIN - 1;
    for (int j = parent->n; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = newNode;
    for (int j = parent->n - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = child->keys[MIN - 1];
    parent->n++;
}
BPlusTreeNode* search(BPlusTreeNode* root, char* hoten) {
    int i = 0;
    while (i < root->n && strcmp(hoten, root->keys[i].hoten) > 0) {
        i++;
    }
    if (i < root->n && strcmp(hoten, root->keys[i].hoten) == 0) {
        return root;
    }
    if (root->isLeaf) {
        return NULL;
    }
    return search(root->children[i], hoten);
}
void traverse(BPlusTreeNode* root) {
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->isLeaf) {
            traverse(root->children[i]);
        }
        printf("\nHo ten: %s", root->keys[i].hoten);
        printf("\nTuoi: %d", root->keys[i].tuoi);
        printf("\nMSSV: %s", root->keys[i].mssv);
    }
    if (!root->isLeaf) {
        traverse(root->children[i]);
    }
}

void freeTree(BPlusTreeNode* root) {
    if (!root->isLeaf) {
        for (int i = 0; i <= root->n; i++) {
            freeTree(root->children[i]);
        }
    }
    free(root);
}
int main() {
    BPlusTreeNode* root = createNode(1);
    int choice;
    char hoten[40];
    sv new_sv;

    while (1) {
        printf("\nMenu:");
        printf("\n1. Them node");
        printf("\n2. Tim node");
        printf("\n3. Duyet cay");
        printf("\n4. Giai phong bo nho cua cay");
        printf("\n0. Thoat");
        printf("\nChon: ");
        if (scanf("%d", &choice) != 1) {
            printf("Lua chon khong hop le. Vui long chon lai.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }
        getchar(); // Clear newline character left by scanf

        switch (choice) {
            case 1:
                printf("\nNhap ho ten: ");
                fgets(new_sv.hoten, sizeof(new_sv.hoten), stdin);
                new_sv.hoten[strcspn(new_sv.hoten, "\n")] = '\0'; // Remove newline character
                printf("Nhap tuoi: ");
                if (scanf("%d", &new_sv.tuoi) != 1) {
                    printf("Tuoi phai la so nguyen!\n");
                    while (getchar() != '\n'); // Clear invalid input
                    continue;
                }
                getchar(); // Clear newline character left by scanf
                printf("Nhap mssv: ");
                fgets(new_sv.mssv, sizeof(new_sv.mssv), stdin);
                new_sv.mssv[strcspn(new_sv.mssv, "\n")] = '\0'; // Remove newline character
                insert(&root, new_sv);
                break;
            case 2:
                printf("\nNhap ho ten cua node can tim: ");
                fgets(hoten, sizeof(hoten), stdin);
                hoten[strcspn(hoten, "\n")] = '\0'; // Remove newline character
                BPlusTreeNode* searchedNode = search(root, hoten);
                if (searchedNode != NULL) {
                    for (int i = 0; i < searchedNode->n; i++) {
                        if (strcmp(searchedNode->keys[i].hoten, hoten) == 0) {
                            printf("\nNode da tim thay:");
                            printf("\nHo ten: %s", searchedNode->keys[i].hoten);
                            printf("\nTuoi: %d", searchedNode->keys[i].tuoi);
                            printf("\nMSSV: %s", searchedNode->keys[i].mssv);
                        }
                    }
                } else {
                    printf("\nKhong tim thay node co ho ten \"%s\".", hoten);
                }
                break;
            case 3:
                printf("\nDuyet cay:");
                traverse(root);
                break;
            case 4:
                printf("\nGiai phong bo nho cua cay.");
                freeTree(root);
                root = createNode(1);
                break;
            case 0:
                freeTree(root);
                exit(0);
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
                break;
        }
    }

    return 0;
}
