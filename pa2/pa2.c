#include <stdio.h>
#include <stdlib.h>
#include "hbt.h"

// int *read_nums(char *filename, int *size) {
//     FILE *fp = fopen(filename, "r");
//     if (fp == NULL) {
//         return NULL;
//     }
//     // fseek(fp, 0, SEEK_END);
//     // int sizeFile = ftell(fp);
//     // rewind(fp);
//     int num;
//     char eat;
//     int *arr = malloc(*size * (sizeof(int)));
//     int i = 0;
//     while (fscanf(fp, "%d %c", &num, &eat) == 2) {
//         arr[i] = num;
//         // printf("arr val: %d\n", arr[i]);
//         // printf("char: %c\n", eat);
//         i++;
//     }
//     fclose(fp);
//     return arr;
    
// }

// char *read_commands(char *filename, int *size) {
//     FILE *fp = fopen(filename, "r");
//     if (fp == NULL) {
//         return NULL;
//     }
//     // fseek(fp, 0, SEEK_END);
//     // int sizeFile = ftell(fp);
//     // rewind(fp);
//     int num;
//     char eat;
//     char *arr = malloc(*size * (sizeof(char)));
//     int i = 0;
//     while (fscanf(fp, "%d %c", &num, &eat) == 2) {
//         arr[i] = eat;
//         // printf("arr val: %d\n", arr[i]);
//         // printf("char: %c\n", eat);
//         i++;
//     }
//     fclose(fp);
//     return arr;
    
// }
void delTree(Tnode * root) {
    if(root == NULL) {
        return;
    }
    delTree(root->left);
    delTree(root->right);
    free(root);
}
int  * binaryFileRead(char * input, int * checkIn, int * Arrsize) {
    FILE *ifp;
    int i = 0;
    //int checkInput;
    ifp = fopen(input, "rb");
    if (ifp == NULL) {
        *checkIn = -1;
        return checkIn;
    }
    int num;
    char letter;
    fseek(ifp, 0, SEEK_END);
    int sizeFile = ftell(ifp);

    int size = 2*(sizeFile / (sizeof(int) + sizeof(char)));
    int * arr = malloc(size * sizeof(int));
    // if(((sizeof(int) + sizeof(char)) * size) != 2*sizeFile) {
    //     fclose(ifp);
    //     *checkIn = 0;
    // }
    rewind(ifp);
    while(sizeFile != 0) {
        if(!fread(&num, sizeof(int), 1, ifp)) {
            printf("can't read file\n");
            fclose(ifp);
            *checkIn = 0;
            return arr;
        }
        if(!fread(&letter, sizeof(char), 1, ifp)) {
            *checkIn = 0;
        }
        if (num > HBT_MAX || num < HBT_MIN) {
            fclose(ifp);
            *checkIn = 0;
        }
        
        if(letter != 'i' && letter != 'd') {
            //printf("bad3\n");
            fclose(ifp);
            *checkIn = 0;
        }
        if(*checkIn == 0) {
            return arr;
        }
        arr[i] = num;
        arr[i+1] = (int) letter;
        i+=2;
        *Arrsize += 2;

        //printf("%d %c\n", num, letter);
        //printf("sizefile: %d\n", sizeFile);
        sizeFile = sizeFile - (sizeof(int) + sizeof(char));
    }
    rewind(ifp);
    fclose(ifp);
    *checkIn = 1;
    return arr;
}
int binaryConvert(char *input, char *output) {
    FILE *ifp;
    FILE *ofp;
    //int checkInput;
    ifp = fopen(input, "rb");
    if (ifp == NULL) {
        return -1;
    }
    ofp = fopen(output, "w");
    int num;
    char letter;
    fseek(ifp, 0, SEEK_END);
    int sizeFile = ftell(ifp);

    int size = sizeFile / (sizeof(int) + sizeof(char));
    if((sizeof(int) + sizeof(char)) * (size) != sizeFile) {
        fclose(ifp);
        fclose(ofp);
        //printf("bad\n");
        return 0;
    }
    rewind(ifp);
    while(sizeFile != 0) {
        fread(&num, sizeof(int), 1, ifp);
        fread(&letter, sizeof(char), 1, ifp);
        if (num > HBT_MAX || num < HBT_MIN) {
            fclose(ifp);
            fclose(ofp);
            //printf("bad2\n");
            return 0;
        }
        
        if(letter != 'i' && letter != 'd') {
            //printf("bad3\n");
            fclose(ifp);
            fclose(ofp);
            return 0;
        }
        fprintf(ofp,"%d %c\n", num, letter);
        //printf("%d %c\n", num, letter);
        //printf("sizefile: %d\n", sizeFile);
        sizeFile = sizeFile - (sizeof(int) + sizeof(char));
    }
    rewind(ifp);
    fclose(ifp);
    fclose(ofp);
    return 1;
}


Tnode * createNode(int key, Tnode *leftNode, Tnode *rightNode) {
    Tnode * newNode = malloc(sizeof(Tnode));
    if (newNode != NULL) {
        newNode->key = key;
        newNode->balance = 0;
        newNode->right = rightNode;
        newNode->left = leftNode;
    }
    return newNode;
}
Tnode * clockwise(Tnode * parent) {
    Tnode * newRoot = parent->left;
    parent->left = newRoot->right;
    newRoot->right = parent;
    return newRoot;
}

Tnode * ccw(Tnode * parent) {
    Tnode * newRoot = parent->right;
    parent->right = newRoot->left;
    newRoot->left = parent;
    return newRoot;
}

Tnode * case1(Tnode * parent) { //both child and parent are positive: requires one clockwise rotation
    Tnode *newParent = clockwise(parent);
    //clockwise(parent);

    return newParent;
}

Tnode * case4(Tnode * parent) { //both child and parent are negative: requires one ccw rotation
    return ccw(parent);
}

Tnode * case2(Tnode * parent) { //parent is positive and child is negative   
    Tnode * child = parent->left;
    Tnode * newRoot = ccw(child);
    parent->left = newRoot;
    return clockwise(parent);

}

Tnode * case3(Tnode * parent) { //parent is negative and child is positive
    Tnode * child = parent->right;
    Tnode * newRoot = clockwise(child);
    parent->right = newRoot;
    return ccw(parent);
}
Tnode * insertNode(int keyD, Tnode * headTree, int * checkMalloc) { //keyD is value inside inserted node
    Tnode * currNode = headTree;
    if (currNode == NULL) {
        headTree = createNode(keyD, NULL, NULL);
        //printf("hello\n");
        return headTree;
    }
    Tnode * prev = NULL;
    Tnode * yAncestor = headTree;
    Tnode * pyA = NULL;
    while (currNode !=NULL) {
        prev = currNode;
        if (keyD <= currNode->key) {
            currNode = currNode->left;
        }
        else {
            currNode = currNode->right;
        }
        if (currNode != NULL) {
            if(currNode->balance != 0) {
                yAncestor = currNode;
                pyA = prev;

            }
        }  //finding node closest to insertion point with nonzero balance (can potentially become unbalanced
    }
    Tnode * newNode = createNode(keyD, NULL, NULL);
    if(newNode == NULL) {
        *checkMalloc = 0;
        return headTree;
    }
    if (keyD <= prev->key) {
        prev->left = newNode;
    }
    else {
        prev->right = newNode;
    }
    currNode = yAncestor;
    while (currNode != newNode) { //updating balances between ya and inserted node (don't need to update above since rotations will handle that)
        if(keyD <= currNode->key) {
            currNode->balance +=1;
            currNode = currNode->left;
        }
        else {
            currNode->balance -= 1;
            currNode = currNode -> right;
        }
    }
    //now 4 cases of rotation
    Tnode * child = NULL;
    if ((yAncestor->balance < 2) && (yAncestor->balance > -2)) {
        return headTree;
    }
    if (keyD <= yAncestor->key) {
        child = yAncestor->left;
    }
    else {
        child = yAncestor->right;
    }
    if (child-> balance > 0) {
        if (yAncestor->balance > 0) {
            currNode = case1(yAncestor);
            yAncestor->balance = 0;
            child->balance = 0;
        }
        else {
            currNode = case3(yAncestor);
            if(currNode->balance == 0) {
                yAncestor->balance = 0;
                child->balance = 0;
            }
            else {
                if(currNode->balance == -1) {
                    yAncestor->balance = 1;
                    child->balance = 0;
                }
                else {
                    yAncestor->balance = 0;
                    child->balance = -1;
                }
            }
            currNode->balance = 0;
        }
    }
    else {
        if(yAncestor->balance < 0) {
            currNode = case4(yAncestor);
            yAncestor->balance = 0;
            child->balance = 0;
        }
        else {
            currNode = case2(yAncestor);
            if(currNode->balance == 0) {
                yAncestor->balance = 0;
                child->balance = 0;
            }
            else {
                if(currNode->balance == 1) {
                    yAncestor->balance = -1;
                    child->balance = 0;
                }
                else {
                    yAncestor->balance = 0;
                    child->balance = 1;
                }
            }
            currNode->balance = 0;
        }
    }
    if(pyA == NULL) {
        headTree = currNode;
    }
    else {
        if (keyD <= pyA->key) {
            pyA->left = currNode;
        }
        else {
            pyA->right = currNode;
        }
    }

    return headTree;
}

void printTrees(Tnode * root, FILE * fp) {
    if (root == NULL) {
        return;
    }
    fprintf(fp,"%d", root->key);
    if(root->left != NULL && root->right != NULL){
        fprintf(fp," %d\n", 3);
    }
    else if(root->left != NULL && root->right == NULL) {
        fprintf(fp," %d\n", 2);
    }
    else if(root->left == NULL && root->right != NULL) {
        fprintf(fp," %d\n", 1);
    }
    else {
        fprintf(fp," %d\n", 0);
    }
    printTrees(root->left, fp);
    printTrees(root->right, fp);

}
void fprintTrees(Tnode * root, char * filename) {
    FILE * fp = fopen(filename, "w");
    printTrees(root, fp);
    fclose(fp);
}
void bprintTrees(Tnode * root, FILE * fp) {
    if (root == NULL) {
        return;
    }
    int temp = root->key;
    fwrite(&temp, 1,sizeof(int), fp);
    if(root->left != NULL && root->right != NULL){
        int let = 3;
        fwrite(&let, 1, sizeof(char), fp);
    }
    else if(root->left != NULL && root->right == NULL) {
        int let = 2;
        fwrite(&let, 1, sizeof(char), fp);
        //fprintf(fp," %d\n", 2);
    }
    else if(root->left == NULL && root->right != NULL) {
        int let = 1;
        fwrite(&let, 1, sizeof(char), fp);
        //fprintf(fp," %d\n", 1);
    }
    else {
        int let = 0;
        fwrite(&let, 1, sizeof(char), fp);
        //fprintf(fp," %d\n", 0);
    }
    bprintTrees(root->left, fp);
    bprintTrees(root->right, fp);


}
void fbprintTrees(Tnode * root, char * filename) {
    FILE * fp = fopen(filename, "wb");
    bprintTrees(root, fp);
    fclose(fp);
}
void convertBinary(char * infile, char * outfile) {
    FILE *ifp = fopen(infile, "r");
    FILE *ofp = fopen(outfile, "wb");
    int num;
    int nodeType;
    while (fscanf(ifp, "%d %d", &num, &nodeType) == 2) {
        fwrite(&num, 1, sizeof(int), ofp);
        fwrite(&nodeType, 1, sizeof(char), ofp);
    }
    fclose(ifp);
    fclose(ofp);
    rewind(ofp);
}
void recurDel(Tnode * todelete, Tnode * pred, Tnode * prev) {
    if(pred->left == NULL) {
        todelete->key = pred->key;
        free(pred);//if predecessor is a leaf node
        prev->left = NULL;
        //have to figure out balance stuff here
        return;
    }
    //todelete->key = pred->key;
    int temp = pred->key;
    pred->key = pred->left->key;
    pred->left->key = temp;
    pred->balance = pred->left->balance;
    if (pred->left->right != NULL) {
        pred->right = pred->left->right;
        pred->left->right = NULL;
    }
    recurDel(todelete, pred->left, pred);
}


// void findPred(Tnode * todelete) { //calling recurDel. Guaranteed to have predecessor
//     Tnode * pred = todelete->left;
//     Tnode * prev = todelete;
//     Tnode * yAncestor = NULL;
//     Tnode * pyA = NULL;
//     todelete->balance -= 1; //before i change these balances also line 438, need to make sure height changes, maybe need more recursion
//     if (todelete->balance != 0) {
//         yAncestor = todelete;
//     }
//     while (pred->right != NULL) {
//         prev = pred;
//         pred->balance += 1;
//         pred = pred->right; //found immediate predecessor
//     }
//     if(prev->balance != 0) { //if height didn't change then the balances do not change(must move them back)
//         int prevTemp = prev->balance;
//         pred = todelete->left;
//         prev = todelete;
//         todelete->balance+=1;
//         while(pred->right != NULL) {
//             prev=pred;
//             pred->balance -= 1;
//             pred = pred->right;
//         }
//         prev->balance = prevTemp;
//     }
//     if (pred->left == NULL) { //need this if statement in case predecessor is a leaf node. Then prev right is NULL
//         if(prev != todelete) {
//             todelete->key = pred->key;
//             prev->right = NULL;
//             free(pred); 
//         }
//         else {
//             todelete->key = pred->key;
//             //todelete->balance -= 1;
//             todelete->left = NULL;
//             free(pred);
//         }

        
//     }
//     //return pred;
//     else {
//         recurDel(todelete, pred, prev); //if there are nodes to the left of the predecessor
//     }
    
// }
void predExists(Tnode * todelete, Tnode * pred, Tnode * parent, Tnode ** newRoot, int * heightChange, int * deletion, int * rotation, Tnode * gp) {
    if(pred->right != NULL) { //found predecessor
        predExists(todelete, pred->right, pred, newRoot, heightChange, deletion, rotation, parent);
    }
    if (*heightChange) {
        if(todelete->key < parent->key) {
            parent->balance -= 1;
            if(parent->balance != 0) {
                *heightChange = 0;
            }
        }
        else {
            parent->balance += 1;
            if(parent->balance != 0) {
                *heightChange = 0;
            }
        }
    }
    int temporary = 0;
    if(pred->left == NULL) {
        if(*deletion == 0) {
            if(parent!= todelete) {
                todelete->key = pred->key;
                parent->right = NULL;
                //parent->balance += 1; //because predecessor was on the right
                free(pred);
                *deletion = 1;
                parent->balance+=1;
                if(parent->balance == 0) {
                    *heightChange = 1;
                }
            }
            else {
                todelete->key = pred->key;
                todelete->left = NULL;
                //todelete->balance -= 1;
                free(pred);
                *deletion = 1;
                parent->balance -= 1;
                if(parent->balance == 0) {
                    *heightChange = 1;
                }
            }
        }
        if(parent->balance == 2) {
            *rotation = 1;
            //do rotations
            Tnode * child = parent->left;
            if(child->balance == 1) { //case 1
                *newRoot = case1(parent);
                parent->balance = 0;
                child->balance = 0;
                if((*newRoot)->key < gp->key) {
                    gp->balance-=1;
                }
                else {
                    gp->balance+=1;
                }
                if(gp->balance == 0) {
                    *heightChange = 1;
                }
                //return;
                //height changes by 1 pass information that left subtree shrank by 1
            }
            else if(child->balance == -1) {
                *newRoot = case2(parent); //case 2
                if((*newRoot)->balance == 0) {
                    parent->balance = 0;
                    child->balance = 0;
                }
                else {
                    if((*newRoot)->balance == 1) {
                        parent->balance = -1;
                        child->balance = 0;
                    }
                    else {
                        parent->balance = 0;
                        child->balance = 1;
                    }
                }
                (*newRoot)->balance = 0;  
                if((*newRoot)->key < gp->key) {
                    gp->balance-=1;
                }
                else {
                    gp->balance+=1;
                }
                if(gp->balance == 0) {
                    *heightChange = 1;
                }        
                //height changes by 1 pass information that left subtree shrank by 1
            }
            else if(child->balance == 0) {
                *newRoot = case1(parent);
                parent->balance = 1;
                child->balance = -1;
                //height does not change
            }
        }
        else if(parent->balance == -2) {
            *rotation = 1;
            Tnode * child = parent->right;
            if(child->balance == -1) {
                *newRoot = case4(parent);
                parent->balance = 0;
                child->balance = 0;
                //height changes by 1 pass information that right subtree shrank by 1
                //gp->balance+=1;

                if((*newRoot)->key < gp->key) {
                    gp->balance-=1;
                }
                else {
                    gp->balance+=1;
                }
                if(gp->balance == 0) {
                    *heightChange = 1;
                }
                //*heightChange = 1;
                //return;
            }
            else if(child->balance == 1) {
                Tnode * root = (case3(parent));
                *newRoot = root;
                //*newRoot = case3(parent);
                // if((*newRoot)->balance == 0) {
                //     parent->balance = 0;
                //     child->balance = 0;
                // }
                //need to connect node to rest of tree not sure how
                if((*newRoot)->balance == 0) {
                    parent->balance = 0;
                    child->balance = 0;
                }
                else {
                    // if((*newRoot)->balance == -1) {
                    //     parent->balance = 1;
                    //     child->balance = 0;
                    // }
                    if((*newRoot)->balance == -1) {
                        parent->balance = 1;
                        child->balance = 0;
                    }
                    else {
                        parent->balance = 0;
                        child->balance = -1;
                    }
                }
                //(*newRoot)->balance = 0;
                (*newRoot)->balance = 0;
                if((*newRoot)->key < gp->key) {
                    gp->balance-=1;
                }
                else {
                    gp->balance+=1;
                }
                if(gp->balance == 0) {
                    *heightChange = 1;
                }      
                //*heightChange = 1;
                //return;
                //height changes by 1 pass information that right subtree shrank by 1
            }   
            else if(child->balance == 0) {
                //*newRoot = case4(parent);
                *newRoot = case4(parent);
                parent->balance = 1;
                child->balance = 1;
                //no change in height
            }        
        }
    }
    else {
    //predecessor is not a leaf node 
    // int tempBalance = prev->balance;
    // if()
    // if(prevBalance)
    if(*deletion == 0) {
        //todelete->balance -= 1;
        *heightChange = 1;
        // parent->balance += 1; //might need to create a temporary variable 
        
        // if(parent->balance == 0) {
        //     *heightChange = 1;
        // }
        recurDel(todelete, pred, parent);
        *deletion = 1;

    }
    //return;
    }
    if(*rotation) {
        if(parent != todelete) {
            gp->right = *newRoot;
        }
        else {
            *rotation = -1;
        }
    }
    if(*rotation == -1) {
        return;
    }
    *rotation = 0;
    return;
}

    
    
void delRightSub(Tnode * todelete, Tnode * curr, Tnode * prev) {
    if(curr->right == NULL) {
        todelete->key = curr->key;
        free(curr);
        prev->right = NULL;
        return;
    }
    int temp = curr->key;
    curr->key = curr->right->key;
    curr->right->key = temp;
    curr->balance = curr->right->balance;
    if (curr->right->left != NULL) {
        curr->left = curr->right->left;
        curr->right->left = NULL;
    }
    delRightSub(todelete, curr->right, curr);
}
Tnode * deleteNode(Tnode * headTree, int keyD, int * checkDeletion, Tnode * currNode, int * hTreeChange, Tnode * parent, int * Trotation, int*notHere, Tnode *grand){
    //first pass currNode = headTree and parent = headTree
    if(currNode != NULL && currNode->key != keyD) {
        if(keyD < currNode->key) {
            headTree = deleteNode(headTree, keyD, checkDeletion, currNode->left, hTreeChange, currNode, Trotation, notHere, parent);
        }
        else {
            headTree = deleteNode(headTree, keyD, checkDeletion, currNode->right, hTreeChange, currNode, Trotation, notHere, parent);
        }
    }
    int temp = 0;
    if(*notHere) {
        return headTree;
    }
    if(currNode == NULL) {
        *notHere = 1;
        return headTree;
    }
    //now currNode is equal to node I want to delete
    if (*checkDeletion) {
        if(currNode->left == NULL && currNode->right == NULL) { //if the todelete node is a leaf node
            if(keyD < parent->key) {
                parent->left = NULL;
            //prev->balance -= 1;
                temp = parent->balance;
                parent->balance-=1;
                if(parent->balance ==0) {
                    parent->balance = temp;
                    *hTreeChange = 1;
                }
                //*hTreeChange = -1;
            }
            else {
                parent->right = NULL;
            //prev->balance += 1;
                temp = parent->balance;
                parent->balance+=1;
                if(parent->balance == 0) {
                    parent->balance = temp;
                    *hTreeChange = 1;
                }
                //*hTreeChange = 1;
            }
        

            free(currNode);
        }
        else if(currNode->left != NULL) { //if immediate predecessor exists
            //findPred(currNode);
            //Tnode  * newRoot = NULL;
            //malloc new root
            Tnode ** newRoot = malloc(sizeof(Tnode*));
            int heightChange = 0;
            int deletion = 0;
            int rotation = 0;
            //*newRoot = malloc(sizeof(Tnode));
            predExists(currNode, currNode->left, currNode, newRoot, &heightChange, &deletion, &rotation, grand);
            if((heightChange) != 0) {
                currNode->balance -= 1;
                if(currNode->balance == 0) {
                    *hTreeChange = -1;
                }
            } 
            if(rotation == -1) {
                if((*newRoot)->key < parent->key) {
                    parent->left = *newRoot;
                }
                else{
                    parent->right = *newRoot;
                }
            }
            free(newRoot);
        }
        else if(currNode->right != NULL) {
            if(keyD < parent->key) {
                //prev->balance -=1;
                *hTreeChange = -1;
            }
            else {
                //prev->balance +=1;
                *hTreeChange = 1;
            }
            delRightSub(currNode, currNode, parent);

        }
        *checkDeletion = 0;

    }
    if(*hTreeChange) {
        if(keyD < parent->key) {
            parent->balance-=1;
            if(parent->balance != 0) {
                *hTreeChange = 0;
            }
        }
        else {
            parent->balance+=1;
            if(parent->balance !=0) {
                *hTreeChange = 0;
            }
        }
    }
    
    // if(parent == NULL) {
        
    // }
    Tnode * root = NULL;
    if(parent->balance == 2) {
        //Tnode * root;
        *Trotation = 1;
        Tnode * child = parent->left;
        if(child->balance == 1) {
            root = case1(parent);
            parent->balance = 0;
            child->balance = 0;
            if(root->key < grand->key) {
                temp = grand->balance;
                grand->balance-=1;
            }
            else {
                temp = grand->balance;
                grand->balance+=1;
            }
            if(grand->balance==0) {
                *hTreeChange = 1;
                grand->balance = temp;
            }
            //*hTreeChange = -1;
        }
        else if(child->balance == -1) {
            root = case2(parent); //case 2
            if(root->balance == 0) {
                parent->balance = 0;
                child->balance = 0;
            }
            else {
                if(root->balance == 1) {
                    parent->balance = -1;
                    child->balance = 0;
                }
                else {
                    parent->balance = 0;
                    child->balance = 1;
                }
            }
            root->balance = 0;
            if(root->key < grand->key) {
                temp = grand->balance;
                grand->balance-=1;
            }
            else {
                temp = grand->balance;
                grand->balance+=1;
            }
            if(grand->balance==0) {
                grand->balance = temp;
                *hTreeChange = 1;
            }
        }
        else if(child->balance == 0) {
            root = case1(parent);
            parent->balance = 1;
            child->balance = -1;
        }
    }
    else if(parent->balance == -2) {
        //Tnode * root;
        *Trotation = 1;
        Tnode * child = parent->right;
        if(child->balance == -1) {
            root = case4(parent);
            parent->balance = 0;
            child->balance = 0;
            //height changes by 1 pass information that right subtree shrank by 1
            //*hTreeChange = 1;
            if(root->key < grand->key) {
                temp = grand->balance;
                grand->balance-=1;
            }
            else {
                temp = grand->balance;
                grand->balance+=1;
            }
            if(grand->balance==0) {
                grand->balance = temp;
                *hTreeChange = 1;
            }
            //return;
        }
        else if(child->balance == 1) {
            root = (case3(parent));
            //*newRoot = case3(parent);
            // if((*newRoot)->balance == 0) {
            //     parent->balance = 0;
            //     child->balance = 0;
            // }
            //need to connect node to rest of tree not sure how
            if(root->balance == 0) {
                parent->balance = 0;
                child->balance = 0;
            }
            else {
                // if((*newRoot)->balance == -1) {
                //     parent->balance = 1;
                //     child->balance = 0;
                // }
                if(root->balance == -1) {
                    parent->balance = 1;
                    child->balance = 0;
                }
                else {
                    parent->balance = 0;
                    child->balance = -1;
                }
            }
            //(*newRoot)->balance = 0;
            root->balance = 0;
            //*hTreeChange = 1;
            if(root->key < grand->key) {
                grand->balance-=1;
                temp = grand->balance;
            }
            else {
                grand->balance+=1;
                temp = grand->balance;
            }
            if(grand->balance==0) {
                grand->balance = temp;
                *hTreeChange = 1;
            }
            //return;
            //height changes by 1 pass information that right subtree shrank by 1
        }   
        else if(child->balance == 0) {
            //*newRoot = case4(parent);
            root = case4(parent);
            parent->balance = 1;
            child->balance = 1;
            //no change in height
        }        
    }
    if(*Trotation) {
        if(parent != headTree) {
            if(grand->key < root->key) {
                grand->right = root;
            }
            else {
                grand->left = root;
            }
        }
        else {
            headTree = root;
        }
    }
    *Trotation = 0;
    return headTree;
}

// Tnode * deletionRotation(Tnode * yAncestor, Tnode * prev) {
//     if ((yAncestor->balance < 2) && (yAncestor->balance > -2)) {
//         return headTree;
//     }
//     if (keyD <= yAncestor->key) {
//         child = yAncestor->left;
//     }
//     else {
//         child = yAncestor->right;
//     }
//     if (child-> balance > 0) {
//         if (yAncestor->balance > 0) {
//             currNode = case1(yAncestor);
//             yAncestor->balance = 0;
//             child->balance = 0;
//         }
//         else {
//             currNode = case3(yAncestor);
//             if(currNode->balance == 0) {
//                 yAncestor->balance = 0;
//                 child->balance = 0;
//             }
//             else {
//                 if(currNode->balance == -1) {
//                     yAncestor->balance = 1;
//                     child->balance = 0;
//                 }
//                 else {
//                     yAncestor->balance = 0;
//                     child->balance = -1;
//                 }
//             }
//             currNode->balance = 0;
//         }
//     }
//     else if (child->balance < 0){
//         if(yAncestor->balance < 0) {
//             currNode = case4(yAncestor);
//             yAncestor->balance = 0;
//             child->balance = 0;
//         }
//         else {
//             currNode = case2(yAncestor);
//             if(currNode->balance == 0) {
//                 yAncestor->balance = 0;
//                 child->balance = 0;
//             }
//             else {
//                 if(currNode->balance == 1) {
//                     yAncestor->balance = -1;
//                     child->balance = 0;
//                 }
//                 else {
//                     yAncestor->balance = 0;
//                     child->balance = 1;
//                 }
//             }
//             currNode->balance = 0;
//         }
//     }
//     else {
//         if (yAncestor->balance > 0) {
//             currNode = case1(yAncestor);
//             yAncestor->balance = 0;
//             child->balance = 0;
//         }
//         else {
//             if(yAncestor->balance < 0) {
//                 currNode = case4(yAncestor);
//                 yAncestor->balance = 0;
//                 child->balance = 0;
//             }
//         }
//     }
//     if(pyA == NULL) {
//         headTree = currNode;
//     }
//     else {
//         if (keyD <= pyA->key) {
//             pyA->left = currNode;
//         }
//         else {
//             pyA->right = currNode;
//         }
//     }
// }

int buildTree(char * infile, char * outfile) { //infile and outifle are binary outputs
    int *checkIn = malloc(sizeof(int));
    int *Arrsize = malloc(sizeof(int));
    int *checkMalloc = malloc(sizeof(int));
    *Arrsize = 0;
    *checkMalloc = 1; //good if its one
    *checkIn = 1;
    int * arr = binaryFileRead(infile, checkIn, Arrsize);
    //int checkIn = binaryConvert(infile, "tempin.txt");
    if(*checkIn == -1) {
        printf("%d\n", *checkIn);
        //printf("exitfailure")
        free(checkIn);
        free(Arrsize);
        free(checkMalloc);
        return EXIT_FAILURE;
    }
    Tnode * root = NULL;
    for(int i = 0; i < *Arrsize; i+=2) {
        if(arr[i+1] == 'i'){ //0x69 in ascii
            root = insertNode(arr[i], root, checkMalloc);
            if(*checkMalloc == 0) {
                *checkIn = 0;
                break; //memory allocation failed
            }
        } 
        else if(arr[i+1] == 'd') { //0x64 in ascii
            int checkDeletion = 1;
            int hTreeChange = 0;
            int Trotation = 0;
            int notHere = 0;
            if(root->left == NULL || root->right == NULL) {
                free(root);
            }
            else {
                root = deleteNode(root, arr[i], &checkDeletion, root, &hTreeChange, root, &Trotation, &notHere, root);
            }
        }
    }
    // FILE * fp = fopen("tempin.txt", "r");
    // //FILE * ofp = fopen("out0.txt", "w");
    // if (fp== NULL) {
    //     return EXIT_FAILURE;
    // }
    // int num;
    // char eat;
    // while (fscanf(fp, "%d %c", &num, &eat) == 2) {
    //     if (eat == 'i') {
    //         //printf("inserting\n");
    //         root = insertNode(num, root);
    //         //printf("%d\n", root->key);
    //     }
    //     else if (eat == 'd') {
    //         //printf("deleting\n");
    //         deleteNode(root, num);
    //     }
    // }
    //printf("hello\n");
    fbprintTrees(root, outfile);
    fprintTrees(root, "tempout.txt");
    // convertBinary("tempout.txt", outfile);
    //fclose(fp);
    delTree(root);
    free(Arrsize);
    free(checkMalloc);
    free(arr);
    if(*checkIn == 0) {
        printf("%d\n", *checkIn);
        free(checkIn);
        return EXIT_FAILURE;
    }
    if(*checkIn == 1) {
        printf("%d\n", *checkIn);
    }
    free(checkIn);
    return EXIT_SUCCESS;
}


//evaluate trees code
int checkBSTFunc(Tnode * root) {
    Tnode * curr = root;
    if (curr == NULL) {
        return 1;
    }
    if(curr->left != NULL) {
        if (curr->left->key > curr->key) {
            return 0;
        }
    }
    else if (curr->right != NULL) {
        if (curr->right->key < curr->key) {
            return 0;
        }

    }
    if(checkBSTFunc(curr->left)) {
        return checkBSTFunc(curr->right);
    }
    else {
        return 0;
    }
}
int *checkInput(int * checkVal, char * infile) { //returns array of preorder integer trees. checkVal is value for input file checking validity
    FILE *ifp;
    int i = 0;
    //int checkInput;
    ifp = fopen(infile, "rb");
    if (ifp == NULL) {
        *checkVal= -1;
        return checkVal;
    }
    int num;
    char letter;
    fseek(ifp, 0, SEEK_END);
    int sizeFile = ftell(ifp);
    int size = 2*(sizeFile / (sizeof(int) + sizeof(char)));
    int * arr = malloc(size * sizeof(int));
    if(((sizeof(int) + sizeof(char)) * (size)) != 2*sizeFile) {
        *checkVal = 0;
    }
    rewind(ifp);
    while(sizeFile != 0) {
        fread(&num, sizeof(int), 1, ifp);
        fread(&letter, sizeof(char), 1, ifp);
        if (num > HBT_MAX || num < HBT_MIN) {
            *checkVal = 0;
        }
        //printf("letter: %d\n", letter);
        int iletter;
        iletter = (int) letter;
        if(iletter != 0 && iletter != 1 && iletter != 2 && iletter != 3) {
            *checkVal = 0;
        }
        if(*checkVal == 0) {
            free(arr);
            return checkVal;
        }
        arr[i] = num;
        //printf("arr[%d] = %d", i, num);
        arr[i+1] = letter;
        i+=2;
        
        //printf("%d %c\n", num, letter);
        //printf("sizefile: %d\n", sizeFile);
        sizeFile = sizeFile - (sizeof(int) + sizeof(char));
    }
    rewind(ifp);
    fclose(ifp);
    //fclose(ofp);
    *checkVal = 1;
    return arr;
}
Tnode * buildingPreorder(int * arr, int start, int * end) { //might want to keep track of heights here so i can find the balances
    Tnode * curr = createNode(arr[start], NULL, NULL);
    if(arr[start+1] == 0) {
        *end = start+2;
        curr->left = NULL;
        curr->right = NULL;
        return curr;
    }
    if(arr[start+1] == 1) {
        curr->left = NULL;
        curr->right = buildingPreorder(arr, start+2, end);
        //*end = start+2;
    }
    else if(arr[start+1] == 2) {
        curr->right = NULL;
        curr->left = buildingPreorder(arr, start+2, end);
        //*end = start+2;
    }
    else{
        //*end = something;
        curr->left = buildingPreorder(arr, start+2, end);
        //*end = start+2;
        curr->right = buildingPreorder(arr, *end, end);
        //*end = start+2;
    }
    return curr;

}
// Tnode * buildPreorder(int * arr, int start, int end) {
//     if (start > end) {
//         return NULL;
//     }

//     Tnode * curr = createNode(arr[start], NULL, NULL);
//     int i;
//     for (i = start; i <= end; i++) {
//         if (arr[i] > curr->key){
//             break; //break if the value in the array is bigger(splits left and right subtrees)
//         }
//     }
//     curr->left = buildPreorder(arr, start + 1, i - 1);
 
//     // recursively construct the right subtree
//     curr->right = buildPreorder(arr, i, end);
//     return curr;
// }
int isBalanced(Tnode * root)
{
    if (root == NULL) {
        return 0;
    }
    int lHeight = isBalanced(root->left);
    if (lHeight == -1) {
        return -1;
    }
    int rHeight = isBalanced(root->right);
    if (rHeight == -1) {
        return -1;
    }
 
    if (abs(lHeight - rHeight) > 1) {
        return -1;
    }
    else {
        if (lHeight > rHeight) {
            return lHeight + 1;
        }
        else {
            return rHeight + 1;
        }
    }
        
}
int evaluateTrees(char * infile) {
    int checkBST;
    int checkHeight;
    int q = 0;
    int *end = &q;
    int * checkVal = malloc(sizeof(int));
    *checkVal = 1;
    int * arr = checkInput(checkVal, infile);
    if (*checkVal == -1 || *checkVal == 0) {
        printf("%d, 0, 0\n", *checkVal);
        free(checkVal);
        free(arr);
        return EXIT_FAILURE;
    }
    // for(int i = 0; i < *size; i++) {
    //     printf("%d\n", arr[i]);
    // }
    //printf("hello\n");
    Tnode * root = buildingPreorder(arr, 0, end);
    //printf("hello\n");
    checkBST = checkBSTFunc(root);
    if (isBalanced(root) > 0) {
        //printf("Balanced");
        checkHeight = 1;
    }
        
    else {
        checkHeight = 0;
    }
    printf("%d, %d, %d\n", *checkVal, checkBST, checkHeight);
    free(checkVal);
    free(arr);
    delTree(root);
    return 1;
    //free(end);
    //have to delete preorder tree here
}
int main(int argc, char ** argv){
    if (argv[1][1] == 'e') {
        //printf("in evaluate\n");
        if (argc != 3) {
            return EXIT_FAILURE;
        }
        evaluateTrees(argv[2]); 
    }
    else if(argv[1][1] == 'b') {
        if(argc != 4) {
            return EXIT_FAILURE;
        }
        //if we run into a problem when building the tree must return what we have built so far and then delete it
        buildTree(argv[2], argv[3]);
        //binaryConvert(argv[3], "outTree.txt");
    }
    else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

    
}