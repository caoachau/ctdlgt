#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>  // De xac thuc dau vao

// Dinh nghia cau truc cho mot nut trong danh sach lien ket
struct Node {
    int data;
    struct Node* next;
};

typedef struct Node* node;

// Ham tao mot nut moi
node makenode(int x) {
    node t = (node)malloc(sizeof(struct Node));
    t->data = x;
    t->next = NULL;
    return t;
}

// Ham kiem tra xem danh sach co rong khong
bool empty(node a) {
    return a == NULL;
}

// Ham lay kich thuoc cua danh sach
int size(node a) {
    int count = 0;
    while (a != NULL) {
        count++;
        a = a->next;
    }
    return count;
}

// Ham them mot nut vao dau danh sach
void addFirst(node *a, int x) {
    node t = makenode(x);
    t->next = *a;
    *a = t;
}

// Ham them mot nut vao cuoi danh sach
void addLast(node *a, int x) {
    node t = makenode(x);
    if (*a == NULL) {
        *a = t;
    } else {
        node p = *a;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = t;
    }
}

// Ham them mot nut vao vi tri cu the trong danh sach
void addMid(node *a, int x, int p) {
    int n = size(*a);
    if (p <= 0 || p > n + 1) {
        printf("Loi!\n");
        return;
    }
    if (p == 1) {
        addFirst(a, x);
        return;
    }
    if (p == n + 1) {
        addLast(a, x);
        return;
    }
    node t = makenode(x);
    node current = *a;
    for (int i = 1; i < p - 1; ++i) {
        current = current->next;
    }
    t->next = current->next;
    current->next = t;
}

// Ham xoa nut dau tien cua danh sach
void delFirst(node *a) {
    if (*a == NULL) return;
    node temp = *a;
    *a = (*a)->next;
    free(temp);
}

// Ham xoa nut cuoi cung cua danh sach
void delLast(node *a) {
    if (*a == NULL) return;
    if ((*a)->next == NULL) {
        free(*a);
        *a = NULL;
        return;
    }
    node prev = NULL;
    node curr = *a;
    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = NULL;
    free(curr);
}

// Ham xoa mot nut tai vi tri cu the trong danh sach
void delMid(node *a, int p) {
    if (p <= 0 || p > size(*a)) return;
    if (p == 1) {
        delFirst(a);
        return;
    }
    node prev = NULL;
    node curr = *a;
    for (int i = 1; i < p; ++i) {
        prev = curr;
        curr = curr->next;
    }
    prev->next = curr->next;
    free(curr);
}

// Ham in danh sach
void printList(node a) {
    while (a != NULL) {
        printf("%d ", a->data);
        a = a->next;
    }
    printf("\n");
}

// Ham sap xep danh sach
void sortList(node *a) {
    for (node p = *a; p != NULL; p = p->next) {
        for (node q = p->next; q != NULL; q = q->next) {
            if (q->data < p->data) {
                int temp = p->data;
                p->data = q->data;
                q->data = temp;
            }
        }
    }
}

// Ham giai phong bo nho duoc su dung boi danh sach lien ket
void freeList(node *head) {
    node current = *head;
    while (current != NULL) {
        node temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

int main() {
    int x, p;
    node head = NULL;
    while (1) {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
        printf("-----------------------------------------\n");
        printf("0. Thoat\n");
        printf("1. Them vao dau\n");
        printf("2. Them vao cuoi\n");
        printf("3. Them vao giua\n");
        printf("4. Xoa tu dau\n");
        printf("5. Xoa tu cuoi\n");
        printf("6. Xoa tu giua\n");
        printf("7. Sap xep\n");
        printf("8. In ra\n");
        printf("-----------------------------------------\n");
        printf("--Nhan phim de thuc hien thao tac--\n");
        int key;
        if (scanf("%d", &key) != 1) {
            printf("Invalid input!\n");
            while (getchar() != '\n');  // Clear the input buffer
            continue;
        }
        switch (key) {
            case 1:
                printf("Nhap gia tri: ");
                while (scanf("%d", &x) != 1) {
                    printf("Dau vao khong hop le! Nhap mot so nguyen: ");
                    while (getchar() != '\n');  // Clear the input buffer
                }
                addFirst(&head, x);
                break;
            case 2:
                printf("Nhap gia tri: ");
                while (scanf("%d", &x) != 1) {
                    printf("Dau vao khong hop le! Nhap mot so nguyen: ");
                    while (getchar() != '\n');  // Clear the input buffer
                }
                addLast(&head, x);
                break;
            case 3:
                printf("Nhap gia tri: ");
                while (scanf("%d", &x) != 1) {
                    printf("Dau vao khong hop le! Nhap mot so nguyen: ");
                    while (getchar() != '\n');  // Clear the input buffer
                }
                printf("Nhap vi tri: ");
                while (scanf("%d", &p) != 1) {
                    printf("Dau vao khong hop le! Nhap mot so nguyen: ");
                    while (getchar() != '\n');  // Clear the input buffer
                }
                addMid(&head, x, p);
                break;
            case 4:
                delFirst(&head);
                break;
            case 5:
                delLast(&head);
                break;
            case 6: 
                printf("Nhap vi tri: ");
                while (scanf("%d", &p) != 1) {
                    printf("Dau vao khong hop le! Nhap mot so nguyen: ");
                    while (getchar() != '\n');  // Clear the input buffer
                }
                delMid(&head, p);
                break;
            case 7:
                sortList(&head);
                break;
            case 8:
                printList(head);
                break;
            case 0:
                freeList(&head);
                exit(0);
            default:
                printf("Lua chon khong hop le!\n");
                break;
        }
        while (getchar() != '\n');  // Clear the input buffer
        printf("\nNhan Enter de tiep tuc hoac nhan 0 de thoat...");
        int c = getchar();
        if (c == '0') {
            freeList(&head);
            break;
        }
    }
    return 0;
}

