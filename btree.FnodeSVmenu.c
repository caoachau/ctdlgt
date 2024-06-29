
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
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
//1.//tao cay
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
//7.8.9//Duyet cay
    void traverseBTree(TREE root) {
        if (root) {
            int i;
            for (i = 0; i < root->n; i++) {
                traverseBTree(root->child[i]);
                printf("\nMSSV: %s || Ho ten: %s Tuoi: %d\n ||", root->students[i].mssv,root->students[i].hoten,root->students[i].tuoi);
            }
            traverseBTree(root->child[i]);
        }
    }
    void traverseBTreeLRN(TREE root) {
        if (root) {
            for (int i = 0; i <= root->n; i++) {
                traverseBTreeLRN(root->child[i]);
            }
            for (int i = 0; i < root->n; i++) {
                printf("\nMSSV: %s || ", root->students[i].mssv);
                printf("Ho ten: %s || ", root->students[i].hoten);
                printf("Tuoi: %d\n", root->students[i].tuoi);
            } 
        }
    }
    void traverseBTreeLNR(TREE root) {
    if (root) {
        int i;
        for (i = 0; i < root->n; i++) {
            traverseBTreeLNR(root->child[i]);
            printf("\nMSSV: %s || ", root->students[i].mssv);
            printf("Ho ten: %s || ", root->students[i].hoten);
            printf("Tuoi: %d\n", root->students[i].tuoi);
        }
        traverseBTreeLNR(root->child[i]);
    }
}

   void insertBTree(Student x);
   void splitChild(TREE node, int i);
//chen pt vao nut cua cay khi nut hien tai chua day
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
//chia nut con 
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
//2//chen sinh vien moi
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
//5.//tim sv
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
//3//xoa sv
    TREE deleteBTreeNode(TREE root, char *mssv) {
        if (root == NULL){
            return root;}
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
//Dua pt vao vi tri trong cay khi chen || xoa 
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
//muon pt cua nut phia truoc de thuc hien thao tac
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
//muon pt cua nut phia sau de thuc hien thao tac
    void borrowFromNext(TREE root, int i) {
    int childIndex = i + 1; // L?u ch? s? c?a nút con c?n m??n
    TREE child = root->child[childIndex];
    // T?m ph?n t? nh? nh?t trong nút con
    TREE minNode = minValueNode(child);
    Student minStudent = minNode->students[0];
    // Gán giá tr? nh? nh?t cho ph?n t? c?n xóa trong root
    root->students[i] = minStudent;
    // Xóa ph?n t? nh? nh?t kh?i nút con
    child = deleteBTreeNode(child, minStudent.mssv);
    root->child[childIndex] = child;
}
//Hop nhat 2 nut con khi 1 trong 2 nut con bi xoa
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
//10//tinh chieu cao
    int calculateHeight(TREE root) {
    if (root == NULL) {
        return 0;
    }
    int height = 0;
    TREE current = root;
    while (!current->leaf) {
        current = current->child[0];
        height++;
    }
    return height + 1;
}
//check in4  
    int isValidNumber(const char *str) {
        int length = strlen(str);
        for (int i = 0; i < length; i++) {
            if (!isdigit((unsigned char)str[i])) {
                return 0; // Không h?p l? n?u có b?t k? k? t? nào không ph?i là s?
            }
        }
        return 1; // H?p l? n?u t?t c? các k? t? ??u là s?
    }
    int isValidName( char *name) {
    int len = strlen(name);
    if (len < 2 || len > 39) return 0;
    for (int i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '-' && name[i] != '\'') return 0;
    }
    return 1;
}
// 11. Giai phong bo nho cua cay 
    void freeBTree(TREE *root) {
    if (*root != NULL) {
        if (!(*root)->leaf) {
            for (int i = 0; i <= (*root)->n; i++) {
                freeBTree(&((*root)->child[i]));
            }
        }
        free(*root);
        *root = NULL;
    }
}


void saveNodeToFile(TREE node, FILE* file) {
    if (node != NULL) {
        for (int i = 0; i < node->n; i++) {
            fprintf(file, "%s,%s,%d\n", node->students[i].mssv, node->students[i].hoten, node->students[i].tuoi);
        }
        if (!node->leaf) {
            for (int i = 0; i <= node->n; i++) {
                saveNodeToFile(node->child[i], file);
            }
        }
    }
}

void saveToFile(TREE root, FILE* file) {
    if (file == NULL) {
        printf("File khong hop le.\n");
        return;
    }
    saveNodeToFile(root, file);
}

void loadFromFile(FILE* file) {
    if (file == NULL) {
        printf("File khong hop le.\n");
        return;
    }

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        Student student;
        if (sscanf(line, "%19[^,],%39[^,],%d", student.mssv, student.hoten, &student.tuoi) == 3) {
            insertBTree(student);
            count++;
        } else {
            printf("Loi khi doc dong: %s", line);
        }
    }

    printf("Da doc va chen %d sinh vien tu file.\n", count);
}
int main() {
    int choice, shouldExit = 0;
    while (!shouldExit) {
    printf("--------------------------Menu-------------------------\n");
printf("|  Lua chon  |              Chuc nang                         |\n");
printf("-------------------------------------------------------------\n");
printf("|     1      |          Luu du lieu vao file( da tao cay)     |\n");
printf("-------------------------------------------------------------\n");
printf("|     2      |         Doc du lieu tu  file                   |\n");
printf("-------------------------------------------------------------\n");
printf("|     3      |               Tao cay                          |\n");
printf("-------------------------------------------------------------\n");
printf("|     4      |              Them node                        |\n");
printf("-------------------------------------------------------------\n");
printf("|     5      |               Xoa node                        |\n");
printf("-------------------------------------------------------------\n");
printf("|     6      |               Sua node                        |\n");
printf("-------------------------------------------------------------\n");
printf("|     7      |               Tim node                        |\n");
printf("-------------------------------------------------------------\n");
printf("|     8      |            Thay the node                      |\n");
printf("-------------------------------------------------------------\n");
printf("|     9      |          Duyet cay (NLR)                      |\n");
printf("-------------------------------------------------------------\n");
printf("|    10      |          Duyet cay (LRN)                      |\n");
printf("-------------------------------------------------------------\n");
printf("|    11      |          Duyet cay (LNR)                      |\n");
printf("-------------------------------------------------------------\n");
printf("|    12      |          Chieu cao cua cay                    |\n");
printf("-------------------------------------------------------------\n");
printf("|    13      |    Giai phong bo nho cua cay                  |\n");
printf("-------------------------------------------------------------\n");
printf("|     0      |               Thoat                           |\n");
printf("-------------------------------------------------------------\n");
printf("Lua chon:");

        if (scanf("%d", &choice) != 1) {
            printf("Lua chon khong hop le. Vui long nhap so.\n");
            while (getchar() != '\n'); // Xóa buffer ??u vào
            continue;
        }
        switch (choice) {
        	
        	case 1: {
				    if (root == NULL || root->n == 0) {
				        printf("Cay B-tree trong, khong co gi de luu.\n");
				    } else {
				        char filepath[256];
				        printf("Nhap duong dan day du de luu file (vi du: D:/[Filepath]: ");
				        scanf(" %255[^\n]", filepath);  // Đ?c c? d?ng, bao g?m kho?ng tr?ng
				        
				        FILE* file = fopen(filepath, "w");
				        if (file == NULL) {
				            printf("Khong the mo file de ghi. Loi: %s\n", strerror(errno));
				        } else {
				            saveToFile(root, file);
				            fclose(file);
				            printf("Da luu du lieu vao file %s\n", filepath);
				        }
				    }
				    break;
				}
		    case 2: {
				    char filepath[256];
				    printf("Nhap duong dan day du de doc file (vi du: D:/[Filepath]: ");
				    scanf(" %255[^\n]", filepath);  // Đ?c c? d?ng, bao g?m kho?ng tr?ng
				    
				    FILE* file = fopen(filepath, "r");
				    if (file == NULL) {
				        printf("Khong the mo file de doc. Loi: %s\n", strerror(errno));
				    } else {
				        loadFromFile(file);
				        fclose(file);
				        printf("Da doc du lieu tu file %s\n", filepath);
				    }
				    break;
				}
            case 3:
                if (root == NULL) {
                    root = createBTreeNode();
                    printf("Cay da duoc tao!\n");
                } else {
                    printf("Cay da ton tai!\n");
                }
                break;
                
            
            case 4: {
                if (root == NULL) {
                    printf("Nh?n 1 đ? t?o B-Tree trư?c khi thêm node!\n");
                    break;
                }
                Student newStudent;
                do {
                    printf("\nNh?p MSSV: ");
                    scanf("%s", newStudent.mssv);
                    if (!isValidNumber(newStudent.mssv)) {
                        printf("Error! Nh?p l?i MSSV.\n");
                    }
                } while (!isValidNumber(newStudent.mssv));

                while (getchar() != '\n'); // Clear input buffer
                do {
                    printf("Nh?p H? và tên: ");
                    getchar(); // Clear input buffer
                    fgets(newStudent.hoten, sizeof(newStudent.hoten), stdin);
                    newStudent.hoten[strcspn(newStudent.hoten, "\n")] = 0;
                    if (!isValidName(newStudent.hoten)) {
                        printf("Error! Nh?p l?i H? và tên.\n");
                    }
                } while (!isValidName(newStudent.hoten));

                do {
                    printf("Nh?p Tu?i: ");
                    char ageStr[10];
                    scanf("%s", ageStr);
                    if (!isValidNumber(ageStr)) {
                        printf("Error! Nh?p l?i Tu?i.\n");
                    } else {
                        newStudent.tuoi = atoi(ageStr);
                        break;
                    }
                } while (1);

                insertBTree(newStudent);
                printf("Sinh viên đ? đư?c thêm vào B-tree.\n");
                break;
            }
             case 5: {
                    printf("Nh?p sinh viên c?n xóa: ");
                    char mssv[20];
             		scanf("%s", mssv);
             		if (!isValidNumber(mssv)) {
             		printf("Error! Nh?p l?i MSSV.\n");
             		break;
             		}
            		 TREE studentNode = searchBTree(root, mssv);
             		if (studentNode == NULL) {
             		printf("Sinh viên có MSSV %s không t?n t?i.\n", mssv);
             		break;
              		}
             		 // Delete the student from the B-tree
             		deleteBTreeNode(root, mssv);
             		printf("Sinh viên có MSSV %s đ? đư?c xóa kh?i B-tree.\n", mssv);
             		break;
             		}
             case 6: {
                printf("Nh?p MSSV c?a sinh viên c?n xóa: ");
                char mssv[20];
                scanf("%s", mssv);
                if (!isValidNumber(mssv)) {
                printf("Error! Nh?p l?i MSSV.\n");
                break;
                }

    			// Search for the student with the given MSSV
    			TREE studentNode = searchBTree(root, mssv);

    			if (studentNode == NULL) {
        		printf("Sinh viên có MSSV %s không t?n t?i.\n", mssv);
        		break;
    			}
   				 // Find the student record to update
   				 Student *studentToUpdate = NULL;
   				 for (int i = 0; i < studentNode->n; i++) {
       			 if (strcmp(studentNode->students[i].mssv, mssv) == 0) {
      		      studentToUpdate = &(studentNode->students[i]);
          		  break;
       				 }
   				 }
   				 if (studentToUpdate == NULL) {
        		printf("Error: Không th? xác đ?nh thông tin sinh viên.\n");
        		break;
    				}
    			// Display current information
   				 printf("Thông tin hi?n t?i:\n");
   				 printf("MSSV: %s ||", studentToUpdate->mssv);
    			printf("Ho ten: %s   ||", studentToUpdate->hoten);
    			printf("Tuoi: %d\n", studentToUpdate->tuoi);
   				 // Update student information
    			printf("Nh?p thông tin m?i( Ho?c nh?t Enter đ? ti?p t?c):\n");
    			printf("H? tên m?i: ");
    			getchar(); // Clear input buffer
    			fgets(studentToUpdate->hoten, sizeof(studentToUpdate->hoten), stdin);
    			studentToUpdate->hoten[strcspn(studentToUpdate->hoten, "\n")] = 0;
    			printf("Nh?p Tu?i m?i( Ho?c 0 đ? ti?p t?c): ");
    			char ageStr[10];
    			scanf("%s", ageStr);
    			if (isValidNumber(ageStr) && atoi(ageStr) > 0) {
       				studentToUpdate->tuoi = atoi(ageStr);
    		} else {
        			printf("Error! Tu?i không h?p l?.\n");
   					 }
    			printf("Đ? c?p nh?t sinh viên.\n");
    			break;
				}
		case 7: {
    			printf("Nh?p MSSV c?n t?n: ");
    			char mssv[20];
    			scanf("%s", mssv);
    			if (!isValidNumber(mssv)) {
       				 printf("Error! Nh?p l?i MSSV.\n");
        			break;
   				 }
    			// Search for the student with the given MSSV
    			TREE studentNode = searchBTree(root, mssv);

   				 if (studentNode == NULL) {
        			printf("MSSV %s không t?n t?i.\n", mssv);
   			 } else {
        			printf("Thông tin:\n");
       				 printf("\n MSSV: %s ||", studentNode->students[0].mssv);
       				 printf("Ho ten: %s    ||", studentNode->students[0].hoten);
       				 printf("Tuoi: %d\n", studentNode->students[0].tuoi);
    				}
    			break;
			}
		case 8: {
			    printf("Nh?p MSSV c?n thay th?: ");
			    char mssv[20];
			    scanf("%s", mssv);
			    if (!isValidNumber(mssv)) {
			        printf("Error! Nh?p l?i MSSV.\n");
			        break;
			    }
			    // Delete the existing node with the given MSSV
			    root = deleteBTreeNode(root, mssv);
			    // Prompt for new student information
			    Student newStudent;
			    printf("Nh?p MSSV m?i: ");
			    scanf("%s", newStudent.mssv);
			    if (!isValidNumber(newStudent.mssv)) {
			        printf("Error! Nh?p l?i MSSV.\n");
			        break;
			    }
			    printf("Nh?p Ho ten m?i: ");
			    getchar(); // Clear input buffer
			    fgets(newStudent.hoten, sizeof(newStudent.hoten), stdin);
			    newStudent.hoten[strcspn(newStudent.hoten, "\n")] = 0;
			    printf("Nh?p Tuoi m?i : ");
			    char ageStr[10];
			    scanf("%s", ageStr);
			    if (isValidNumber(ageStr)) {
			        newStudent.tuoi = atoi(ageStr);
			    } else {
			        printf("Error! Tu?i sai.\n");
			        newStudent.tuoi = 0;
			    }
			    // Insert the new student information into the B-tree
			    insertBTree(newStudent);
			    printf(" Sinh viên có MSSV %s đ? đư?c thay th?.\n", mssv);
			    break;
			}
        case 9: {
                if (root == NULL) {
                    printf("Cay chua duoc tao!\n");
                } else {
                    printf("Cay da duoc tao:\n");
                    traverseBTree(root);
                }
                break;
            }
            case 10: {
			    if (root == NULL) {
			        printf("Cay chua duoc tao!\n");
			    } else {
			        printf("Duyet cay theo thu tu LRN:\n");
			        traverseBTreeLRN(root);
			    }
			    break;
			}

			case 11: {
			    if (root == NULL) {
			        printf("Cay chua duoc tao!\n");
			    } else {
			        printf("Duyet cay theo thu tu LNR:\n");
			        traverseBTreeLNR(root);
			    }
			    break;
			}

			case 12: {
			    if (root == NULL) {
			        printf("Cay chua duoc tao!\n");
			    } else {
			        int height = calculateHeight(root);
			        printf("Chieu cao cua cay: %d\n", height);
			    }
			    break;
			}

			case 13: {
			    if (root == NULL) {
			        printf("Cay chua duoc tao!\n");
			    } else {
			        freeBTree(&root);
			        root = NULL;
			        printf("Da giai phong bo nho cua cay.\n");
			    }
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
			