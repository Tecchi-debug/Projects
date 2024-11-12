#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct Node {
    Node * left;
    Node * right;
    char name[26];
    int fine;
};

// Node Functions
Node * createNode (char name[26], int fine);
Node * removeNode (Node * root, Node * node);
Node * parent(Node * root, Node * node);
Node * maxLeft(Node * root);
/*-----------------------------------------------*/
Node * createNode (char name[26], int fine) {
    Node * newNode = malloc (sizeof(Node));
    strcpy(newNode->name,name);
    newNode->fine = fine;
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}
Node* parent(Node *root, Node *node) {

  // Take care of NULL cases.
  if (root == NULL || root == node)
    return NULL;

  // The root is the direct parent of node.
  if (root->left == node || root->right == node)
    return root;

  // Look for node's parent in the left side of the tree.
  if (strcmp(node->name,root->name) < 0)
    return parent(root->left, node);

  // Look for node's parent in the right side of the tree.
  else if (strcmp(node->name,root->name) > 0)
    return parent(root->right, node);

  return NULL; // Catch any other extraneous cases.

}

Node * maxLeft(Node * root) {
    if (root->right == NULL) return root;
    return maxLeft(root->right);
}
int isLeaf(Node *node) {

  return (node->left == NULL && node->right == NULL);
}

// Returns 1 iff node has a left child and no right child.
int hasOnlyLeftChild(Node *node) {
  return (node->left!= NULL && node->right == NULL);
}

// Returns 1 iff node has a right child and no left child.
int hasOnlyRightChild(Node *node) {
  return (node->left== NULL && node->right != NULL);
}

Node *findNode(Node *root, char name[26]) {
    if (root == NULL) return NULL;

    if (strcmp(name, root->name) == 0)
        return root;

    if (strcmp(name, root->name) < 0)
        return findNode(root->left, name);

    return findNode(root->right, name);
}
// Will delete the node storing name in the tree rooted at root. The
// name must be present in the tree in order for this function to work.
// The function returns a pointer to the root of the resulting tree.
Node* delete(Node* root, char name[26]) {

    Node *delnode, *new_del_node, *save_node;
    Node *par;

    delnode = findNode(root, name); // Get a pointer to the node to delete.

    par = parent(root, delnode); // Get the parent of this node.

    // Take care of the case where the node to delete is a leaf node.
    if (isLeaf(delnode)) {// case 1

        // Deleting the only node in the tree.
        if (par == NULL) {
            free(root); // free the memory for the node.
            return NULL;
        }

        // Deletes the node if it's a left child.
        if (strcmp(name,par->name) < 0) {

            free(par->left); // Free the memory for the node.
            par->left = NULL;
        }

        // Deletes the node if it's a right child.
        else {
            free(par->right); // Free the memory for the node.
            par->right = NULL;
        }

        return root; // Return the root of the new tree.
    }

    // Take care of the case where the node to be deleted only has a left
    // child.
    if (hasOnlyLeftChild(delnode)) {

        // Deleting the root node of the tree.
        if (par == NULL) {
            save_node = delnode->left;
            free(delnode); // Free the node to delete.
            return save_node; // Return the new root node of the resulting tree.
        }

        // Deletes the node if it's a left child.
        if (strcmp(name,par->name) < 0) {
            save_node = par->left; // Save the node to delete.
            par->left = par->left->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }

        // Deletes the node if it's a right child.
        else {
            save_node = par->right; // Save the node to delete.
            par->right = par->right->left; // Readjust the parent pointer.
            free(save_node); // Free the memory for the deleted node.
        }

        return root; // Return the root of the tree after the deletion.
    }

    // Takes care of the case where the deleted node only has a right child.
    if (hasOnlyRightChild(delnode)) {

        // Node to delete is the root node.
        if (par == NULL) {
            save_node = delnode->right;
            free(delnode);
            return save_node;
        }

        // Delete's the node if it is a left child.
        if (strcmp(name,par->name) < 0) {
            save_node = par->left;
            par->left = par->left->right;
            free(save_node);
        }

        // Delete's the node if it is a right child.
        else {
            save_node = par->right;
            par->right = par->right->right;
            free(save_node);
        }
        return root;
    }
    //if your code reaches hear it means delnode has two children
    // Find the new physical node to delete.
    new_del_node = maxLeft(delnode->left);
    char tmp_name[26];
    strcpy(tmp_name,new_del_node->name);
    int tmp_fine = new_del_node->fine;

    delete(root, tmp_name);  // Now, delete the proper name.

    // Restore the name to the original node to be deleted.
    strcpy(delnode->name,tmp_name);
    delnode->fine = tmp_fine;

    return root;
}
// Menu Functions
Node * insert (Node * root, char name[26], int fine, int depth);
Node * deduct (Node * root, char name[26], int deduction);
void search(Node * root, char name[26], int depth);
int sum(Node * root);
int numNodes(Node * root);

/*--------------------------------------------------------*/
Node * insert(Node * root, char name[26], int fine, int depth) {
    // If the tree is empty, insert the new node as the root.
    if (root == NULL) {
        Node *newNode = createNode(name, fine);
        printf("%s %d %d\n", newNode->name, newNode->fine, depth);
        return newNode;
    }
    
    // Go left if the new node's name is less than the current node's name.
    else if (strcmp(name, root->name) < 0) {
        // If left child is empty, insert the node here.
        if (root->left == NULL) {
            root->left = insert(root->left, name, fine, depth + 1);
        } else {
            // Otherwise, continue inserting into the left subtree.
            root->left = insert(root->left, name, fine, depth + 1);
        }
    }
    // Go right if the new node's name is greater than the current node's name.
    else if (strcmp(name, root->name) > 0) {
        // If right child is empty, insert the node here.
        if (root->right == NULL) {
            root->right = insert(root->right, name, fine, depth + 1);
        } else {
            // Otherwise, continue inserting into the right subtree.
            root->right = insert(root->right, name, fine, depth + 1);
        }
    }
    // If the name is the same, just update the fine.
    else if (strcmp(name, root->name) == 0) {
        root->fine += fine;
        printf("%s %d %d\n", root->name, root->fine, depth);
    }

    // Return the unchanged root to maintain the tree structure.
    return root;
}
Node *findNodeWdepth(Node *root, char name[26], int *depth) {
    if (root == NULL) return NULL;

    if (strcmp(name, root->name) == 0)
        return root;

    (*depth)++;
    if (strcmp(name, root->name) < 0)
        return findNodeWdepth(root->left, name, depth);

    return findNodeWdepth(root->right, name, depth);
}

int sum (Node * root) {
    if (root == NULL) return 0;
    //printf("%s %d -> ", root->name, root->fine);
    return root->fine + sum(root->left) + sum(root->right);
}

int numNodes (Node * root) {
    if (root == NULL) return 0;
    return 1 + numNodes(root->left) + numNodes(root->right);
}

int height(Node * root) {
    if (root == NULL) return -1;
    
    int left = height(root->left);
    int right = height(root->right);

    if (left > right) return left + 1;
    else return right + 1;
}

int calc_below(Node * root, char name[26]) {
    if (root == NULL) return 0;
    
    //printf("%s %d -> ", root->name, root->fine);
    if (strcmp(root->name, name) <= 0) {
        return root->fine + calc_below(root->left, name)
            + calc_below(root->right, name);
    }
    return calc_below(root->left, name) + calc_below(root->right, name);
}

Node * deduct (Node * root, char name[26], int deduction) {
    
    int depth = 0;
    Node * toDeduct = findNodeWdepth(root, name, &depth);
    if (toDeduct == NULL) {
        printf("%s not found\n", name);
        return root;
    }
    toDeduct->fine -= deduction;
    if (toDeduct->fine > 0) {
        printf("%s %d %d\n", toDeduct->name, toDeduct->fine, depth);
        return root;
    }
    printf("%s removed\n", toDeduct->name);
    delete(root, name);
    return root;
}
void freeTree(Node * root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}

// Main
/*------------------------------------------------*/
int main () {
    
    Node * bst = NULL;
    char name[26]; int fine; 
    int numLines;
    scanf("%d", &numLines);
    for (int i = 0; i < numLines; i++) {

        char choice[26];
        scanf("%s", choice);

        if (strcmp(choice, "add") == 0) {
            scanf("%s %d", name, &fine);
            bst = insert(bst, name, fine, 0);
            
        } else if (strcmp(choice, "deduct") == 0) {
            int deduction;
            scanf("%s %d", name, &deduction);
            bst = deduct(bst, name, deduction);
        } else if (strcmp(choice, "search") == 0) {
            int depth = 0;
            scanf("%s", name);
            Node * tmp = findNodeWdepth(bst, name, &depth);
            if (tmp == NULL)
                printf("%s not found\n", name);
            else
                printf("%s %d %d\n", tmp->name, tmp->fine, depth);
        } else if (strcmp(choice, "average") == 0) {
            double average = ((double)sum(bst)/(double)numNodes(bst));
            printf("%.2f\n", average);
        } else if (strcmp(choice, "height_balance") == 0) {
            int lH = height(bst->left);
            int rH = height(bst->right);
            int balance = lH - rH;
            printf("left height = %d right height = %d ", lH, rH);
            if (balance == 0) printf("balanced\n");
            else printf("not balanced\n");
        } else if (strcmp(choice, "calc_below") == 0) {
            scanf("%s", name);
            int total = calc_below(bst, name);
            printf("%d\n", total);
        }
    }
    freeTree(bst);
    return 0;
}

