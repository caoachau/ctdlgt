
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 3  // Degree of the B-tree (minimum degree)

typedef struct {
    char mssv[20];
    char hoten[40];
    int tuoi;
} Student;

typedef struct bTreeNode {
    int n; // Number of keys in the node
    Student students[2 * T - 1]; // Array of students
    struct bTreeNode *child[2 * T]; // Array of child pointers
    int leaf; // Is true if node is a leaf, false otherwise
} BTreeNode, *TREE; // Typedef for BTreeNode and TREE

TREE root = NULL; // Root of the B-tree

TREE createBTreeNode() {
    TREE newNode = (TREE)malloc(sizeof(BTreeNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    newNode->n = 0;
    newNode->leaf = 1; // Initially, node is a leaf
    for (int i = 0; i < 2 * T; i++) {
        newNode->child[i] = NULL;
    }
    return newNode;
}

void traverseBTree(TREE root) {
    if (root) {
        int i;
        for (i = 0; i < root->n; i++) {
            traverseBTree(root->child[i]);
            printf("\nMSSV: %s", root->students[i].mssv);
            printf("\nHo ten: %s", root->students[i].hoten);
            printf("\nTuoi: %d", root->students[i].tuoi);
        }
        traverseBTree(root->child[i]);
    }
}

void insertBTree(Student x);
void splitChild(TREE node, int i);
void insertNonFull(TREE node, Student x) {
    int i = node->n - 1;
    if (node->leaf) {
        while (i >= 0 && strcmp(node->students[i].mssv, x.mssv) > 0) {
            node->students[i + 1] = node->students[i];
            i--;
        }
        node->students[i + 1] = x;
        node->n++;
    } else {
        while (i >= 0 && strcmp(node->students[i].mssv, x.mssv) > 0)
            i--;
        i++;
        if (node->child[i]->n == 2 * T - 1) {
            splitChild(node, i);
            if (strcmp(node->students[i].mssv, x.mssv) < 0)
                i++;
        }
        insertNonFull(node->child[i], x);
    }
}

void splitChild(TREE node, int i) {
    TREE y = node->child[i];
    TREE z = createBTreeNode();
    z->leaf = y->leaf;
    z->n = T - 1;
    for (int j = 0; j < T - 1; j++)
        z->students[j] = y->students[j + T];
    if (!y->leaf) {
        for (int j = 0; j < T; j++)
            z->child[j] = y->child[j + T];
    }
    y->n = T - 1;
    for (int j = node->n; j >= i + 1; j--)
        node->child[j + 1] = node->child[j];
    node->child[i + 1] = z;
    for (int j = node->n - 1; j >= i; j--)
        node->students[j + 1] = node->students[j];
    node->students[i] = y->students[T - 1];
    node->n++;
}

void insertBTree(Student x) {
    if (root == NULL) {
        root = createBTreeNode();
        root->students[0] = x;
        root->n = 1;
    } else {
        if (root->n == 2 * T - 1) { // Root is full, need to split
            TREE s = createBTreeNode();
            s->leaf = 0;
            s->child[0] = root;
            splitChild(s, 0);
            int i = 0;
            if (strcmp(s->students[0].mssv, x.mssv) < 0)
                i++;
            insertNonFull(s->child[i], x);
            root = s;
        } else {
            insertNonFull(root, x);
        }
    }
}

TREE searchBTree(TREE root, char *mssv) {
    if (root == NULL)
        return NULL;

    int i = 0;
    while (i < root->n && strcmp(mssv, root->students[i].mssv) > 0)
        i++;

    if (i < root->n && strcmp(mssv, root->students[i].mssv) == 0)
        return root;

    if (root->leaf)
        return NULL;

    return searchBTree(root->child[i], mssv);
}

TREE minValueNode(TREE node) {
    TREE current = node;
    while (current && current->child[0] != NULL)
        current = current->child[0];
    return current;
}

void merge(TREE root, int i);
void fill(TREE root, int i);
TREE deleteBTreeNode(TREE root, char *mssv) {
    if (root == NULL)
        return root;

    int i = 0; // Initialize i here
    while (i < root->n && strcmp(mssv, root->students[i].mssv) > 0)
        i++;

    if (i < root->n && strcmp(mssv, root->students[i].mssv) == 0) {
        if (root->leaf) {
            for (int j = i; j < root->n - 1; j++)
                root->students[j] = root->students[j + 1];
            root->n--;
            return root;
        }

        if (root->child[i]->n >= T) {
            TREE pred = root->child[i];
            TREE temp = minValueNode(pred);
            root->students[i] = temp->students[0];
            root = deleteBTreeNode(pred, temp->students[0].mssv);
        } else if (root->child[i + 1]->n >= T) {
            TREE succ = root->child[i + 1];
            TREE temp = minValueNode(succ);
            root->students[i] = temp->students[0];
            root = deleteBTreeNode(succ, temp->students[0].mssv);
        } else {
            merge(root, i);
            root = deleteBTreeNode(root->child[i], mssv);
        }
    } else {
        if (root->child[i]->n < T)
            fill(root, i);

        if (root->child[i] == NULL)
            root = deleteBTreeNode(root->child[i], mssv);
    }
    return root;
}


void borrowFromPrev(TREE root, int i);
void borrowFromNext(TREE root, int i);
void fill(TREE root, int i) {
    if (i != 0 && root->child[i - 1]->n >= T)
        borrowFromPrev(root, i);
    else if (i != root->n && root->child[i + 1]->n >= T)
        borrowFromNext(root, i);
    else {
        if (i != root->n)
            merge(root, i);
        else
            merge(root, i - 1);
    }
}

void borrowFromPrev(TREE root, int i) {
    TREE child = root->child[i];
    TREE sibling = root->child[i - 1];

    for (int j = child->n - 1; j >= 0; j--)
        child->students[j + 1] = child->students[j];

    if (!child->leaf) {
        for (int j = child->n + 1; j >= 0; j--)
            child->child[j + 1] = child->child[j];
        child->child[0] = sibling->child[sibling->n];
    }

    child->students[0] = sibling->students[sibling->n - 1];
    child->n++;
    sibling->n--;
}

void borrowFromNext(TREE root, int i) {
    int childIndex = i + 1; // Lưu chỉ số của nút con cần mượn
    TREE child = root->child[childIndex];

    // Tìm phần tử nhỏ nhất trong nút con
    TREE minNode = minValueNode(child);
    Student minStudent = minNode->students[0];

    // Gán giá trị nhỏ nhất cho phần tử cần xóa trong root
    root->students[i] = minStudent;

    // Xóa phần tử nhỏ nhất khỏi nút con
    child = deleteBTreeNode(child, minStudent.mssv);
    root->child[childIndex] = child;
}

void merge(TREE root, int i) {
    TREE child = root->child[i];
    TREE sibling = root->child[i + 1];

    // Copy the middle key from the current root to the end of the left child
    child->students[T - 1] = root->students[i];

    // Copy all keys from the sibling to the right of the middle key in child
    for (int j = 0; j < sibling->n; j++)
        child->students[j + T] = sibling->students[j];

    // If not a leaf, move all children pointers from sibling to child
    if (!child->leaf) {
        for (int j = 0; j <= sibling->n; j++)
            child->child[j + T] = sibling->child[j];
    }

    // Move all keys and child pointers from root one position left to fill the gap
    for (int j = i + 1; j < root->n; j++)
        root->students[j - 1] = root->students[j];
    for (int j = i + 2; j <= root->n; j++)
        root->child[j - 1] = root->child[j];

    // Update the counts of keys in child and root
    child->n += sibling->n + 1;
    root->n--;

    // Free the memory allocated to sibling
    free(sibling);
}
int calculateHeight(TREE root) {
    int height = 0, maxHeight = 0;
    TREE temp = root;
    while (temp) {
        height++;
        temp = deleteBTreeNode(temp, temp->students[0].mssv);

        maxHeight = (maxHeight > height) ? maxHeight : height;
    }
    return maxHeight;
}

int main() {
    int choice, shouldExit = 0;

    while (!shouldExit) {
        printf("\nMenu:");
        printf("\n1. Tao cay");
        printf("\n2. Them node");
        printf("\n3. Xoa node");
        printf("\n4. Sua node");
        printf("\n5. Tim node");
        printf("\n6. Thay the node");
        printf("\n7. Duyet cay (NLR)");
        printf("\n8. Duyet cay (LRN)");
        printf("\n9. Duyet cay (LNR)");
        printf("\n10. Chieu cao cua cay");
        printf("\n11. Giai phong bo nho cua cay");
        printf("\n0. Thoat");
        printf("\nChon: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Code to create the B-tree
                break;
            case 2: {
                Student newStudent;
                printf("\nNhap MSSV: ");
                scanf("%s", newStudent.mssv);
                printf("Nhap ho ten: ");
                fgets(newStudent.hoten, sizeof(newStudent.hoten), stdin);
                printf("Nhap tuoi: ");
                scanf("%d", &newStudent.tuoi);
                insertBTree(newStudent);
                break;
            }
            case 3: {
                printf("Nhap MSSV can xoa: ");
                char mssv[20];
                scanf("%s", mssv);
                root = deleteBTreeNode(root, mssv);
                break;
            }
            case 4:
                // Code to edit a node
                break;
            case 5: {
                printf("Nhap MSSV can tim: ");
                char mssv[20];
                scanf("%s", mssv);
                TREE searchResult = searchBTree(root, mssv);
                if (searchResult != NULL) {
                    printf("Thong tin sinh vien:\n");
                    printf("MSSV: %s\n", searchResult->students[0].mssv);
                    printf("Ho ten: %s\n", searchResult->students[0].hoten);
                    printf("Tuoi: %d\n", searchResult->students[0].tuoi);
                } else {
                    printf("Khong tim thay sinh vien!\n");
                }
                break;
            }
            case 6: {
                TREE temp = root;
                printf("Nhap MSSV can thay the: ");
                char mssv[20];
                scanf("%s", mssv);
                root = deleteBTreeNode(root, mssv);
                Student newStudent;
                printf("Nhap MSSV moi: ");
                scanf("%s", newStudent.mssv);
                insertBTree(newStudent);
                break;
            }
            case 7: {
                TREE temp = root;
                printf("Cay da duoc tao\n");
                traverseBTree(temp);
                break;
            }
            case 0:
                shouldExit = 1;
                break;
            default:
                printf("\nLua chon khong hop le!");
                break;
        }
    }

    return 0;
}