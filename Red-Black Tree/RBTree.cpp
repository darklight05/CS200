//CHECK WITH DIXON TO SEE IF STRUCTURE NEEDS TO BE MAINTAINED AFTER COPY IN COPY OPERATORS
    //FIX RANK AFTER REMOVING SOMETHING

#include <stdlib.h>
#include <iostream>
using namespace std; 

enum Color {red, black};

template<typename keytype, typename valuetype>
class RBTree{
    private:
        struct Node{
            keytype key;
            valuetype value;
            bool color;
            int leftSize;
            int rightSize;
            Node *left, *right, *parent;
            Node (){
                this->right = NULL;
                this->left = NULL;
                this->parent = NULL;
                this->color = black;
                // this->leftSize = 0;
                // this->rightSize = 0; 
            }
        }; 
        Node *root;
        Node *Tnil;
        int TreeSize;

    public:
        //int TreeSize;
        RBTree(){
            Tnil = new Node;
            root = Tnil;
            TreeSize = 0;
        }

        void fuckingDelete(Node *&temp, Node*&tempTnil){
            if (temp != tempTnil){
                fuckingDelete(temp->left, tempTnil);
                fuckingDelete(temp->right, tempTnil);
                delete temp;
            }
            temp = NULL;
        }
        ~RBTree(){
            fuckingDelete(root, Tnil);
        }

        void copyLeft(Node *newObj, Node *old_Left, Node *old_Tnil){
            if (old_Left == old_Tnil){
                newObj->left = Tnil;
                return;
            }
            Node *newLeft = new Node;
            newLeft->key = old_Left->key;
            newLeft->value = old_Left->value;
            newLeft->leftSize = old_Left->leftSize;
            newLeft->color = old_Left->color;
            newObj->left = newLeft;
            newLeft->parent = newObj;
            copyLeft(newLeft, old_Left->left, old_Tnil);
            copyRight(newLeft, old_Left->right, old_Tnil);
        }
        void copyRight(Node *newObj, Node* old_Right, Node *old_Tnil){
            if (old_Right == old_Tnil){
                newObj->right = Tnil;
                return;
            }
            Node *newRight = new Node;
            newRight->key = old_Right->key;
            newRight->value = old_Right->value;
            newRight->leftSize = old_Right->leftSize;
            newRight->color = old_Right->color;
            newObj->right = newRight;
            newRight->parent = newObj;
            copyLeft(newRight, old_Right->left, old_Tnil);
            copyRight(newRight, old_Right->right, old_Tnil);
        }
        RBTree (const RBTree &old_obj){
            Tnil = new Node;
            if (old_obj.root == old_obj.Tnil){
                root = Tnil;
                return;
            }

            Node *newOne = new Node;
            newOne->key = old_obj.root->key;
            newOne->value = old_obj.root->value;
            newOne->leftSize = old_obj.root->leftSize;
            newOne->color = old_obj.root->color;

            root = newOne;
            root->parent = Tnil;
            copyLeft(root, old_obj.root->left, old_obj.Tnil);
            copyRight(root, old_obj.root->right, old_obj.Tnil);
            TreeSize = old_obj.TreeSize;
        }
        RBTree & operator=(const RBTree &old_obj){
            fuckingDelete(root, Tnil);
            Tnil = new Node;
            if (old_obj.root == old_obj.Tnil){
                root = Tnil;
                return *this;
            }

            Node *newOne = new Node;
            newOne->key = old_obj.root->key;
            newOne->value = old_obj.root->value;
            newOne->leftSize = old_obj.root->leftSize;
            newOne->color = old_obj.root->color;

            root = newOne;
            root->parent = Tnil;
            copyLeft(root, old_obj.root->left, old_obj.Tnil);
            copyRight(root, old_obj.root->right, old_obj.Tnil);
            TreeSize = old_obj.TreeSize;

            return *this;
        }

        Node *bstInsert(Node *root, Node *pt){
            if (root == Tnil){ 
                return pt;
            }

            if (pt->key < root->key){
                root->leftSize++;
                root->left = bstInsert(root->left, pt);
                root->left->parent = root;
            }
            else if (pt->key > root->key){
                root->rightSize++;
                root->right = bstInsert(root->right, pt);
                root->right->parent = root;
            }

            return root;
        }
        void rotateLeft(Node *&pt){
            Node *tempRight = pt->right;
            tempRight->leftSize = pt->leftSize + tempRight->leftSize + 1;
            pt->rightSize = pt->rightSize - pt->rightSize + tempRight->leftSize;
            pt->right = tempRight->left;
            if (pt->right != Tnil){
                pt->right->parent = pt;
            }
            tempRight->parent = pt->parent;
            if (pt->parent == Tnil){
                root = tempRight;
            }
            else if (pt == pt->parent->left){
                pt->parent->left = tempRight;
            }
            else {
                pt->parent->right = tempRight;
            }
            tempRight->left = pt;
            pt->parent = tempRight;
        }
        void rotateRight(Node *&pt){
            Node *tempLeft = pt->left;
            pt->leftSize = pt->leftSize - tempLeft->leftSize - 1;
            tempLeft->rightSize = pt->rightSize - tempLeft->rightSize + 1 + pt->leftSize;
            pt->left = tempLeft->right;
            if (pt->left != Tnil){
                pt->left->parent = pt;
            }
            tempLeft->parent = pt->parent;
            if (pt->parent == Tnil){
                root = tempLeft;
            }
            else if (pt == pt->parent->left){
                pt->parent->left = tempLeft;
            }
            else {
                pt->parent->right = tempLeft;
            }
            tempLeft->right = pt;
            pt->parent = tempLeft;
        }
        void insertFixUp(Node*&pt){
            Node *parentPt = Tnil;
            Node *grandParentPt = Tnil;

            while ((pt != root) && (pt->color != black) && (pt->parent->color == red)){
                parentPt = pt->parent;
                grandParentPt = pt->parent->parent;

                //Parent of pt is left child of grandParent 
                if (parentPt == grandParentPt->left){
                    Node *unclePt = grandParentPt->right;
                    //uncle is red
                    if ((unclePt != Tnil) && (unclePt->color == red)){
                        grandParentPt->color = red;
                        parentPt->color = black;
                        unclePt->color = black;
                        pt = grandParentPt;
                    }
                    else {
                    //pt is right child so leftRotation
                        if (pt == parentPt->right){
                            rotateLeft(parentPt);
                            pt = parentPt;
                            parentPt = pt->parent;
                        }
                    //pt is left child so rightRotation
                        rotateRight(grandParentPt);
                        swap(parentPt->color, grandParentPt->color);
                        pt = parentPt;
                    }
                }

                else {
                    Node *unclePt = grandParentPt->left;

                    if ((unclePt != Tnil) && (unclePt->color == red)){
                        grandParentPt->color = red;
                        parentPt->color = black;
                        unclePt->color = black;
                        pt = grandParentPt;
                    }
                    else {
                    //pt is left child so rightRotation
                        if (pt == parentPt->left){
                            rotateRight(parentPt);
                            pt = parentPt;
                            parentPt = pt->parent;
                        }
                    //pt is right child so leftRotation
                        rotateLeft(grandParentPt);
                        swap(parentPt->color, grandParentPt->color);
                        pt = parentPt;
                    }
                }
            }
            root->color = black;
        }
        void insert(keytype key, valuetype value){ 
            Node *pt = new Node;
            pt->key = key;
            pt->value = value;
            pt->color = red;
            pt->leftSize = 0;
            pt->rightSize = 0;
            pt->left = Tnil;
            pt->right = Tnil;
            pt->parent = Tnil;
            root = bstInsert(root, pt);
            insertFixUp(pt);
            TreeSize++;
        }

        RBTree(keytype K[], valuetype V[], int s){
            
            Tnil =  new Node;
            root = Tnil;
            TreeSize = 0;
            for (int i = 0; i < s; i++){
                insert(K[i], V[i]);
            }
        }

        void inorderHelper(Node *n){
            Node *temp = n;
            if (temp == Tnil){
                return;
            }
            inorderHelper(temp->left);
            cout << temp->key << " ";
            inorderHelper(temp->right);
        }
        void inorder(){
            inorderHelper(root);
            cout << endl;
        }

        void preorderHelper(Node* n){
            Node *temp = n;
            if (temp == Tnil){
                return;
            }
            cout << temp->key << " ";
            preorderHelper(temp->left);
            preorderHelper(temp->right);
        }
        void preorder(){
            preorderHelper(root);
            cout << endl;
        }

        void postorderHelper(Node *n){
            Node *temp = n;
            if (temp == Tnil){
                return;
            }
            postorderHelper(temp->left);
            postorderHelper(temp->right);
            cout << temp->key << " ";
        }
        void postorder(){
            postorderHelper(root);
            cout << endl;
        }

        int size(){
            return TreeSize;
        }
        
        int rank(keytype K){
            Node *temp = root;
            int rankNumber = 1;
            while (temp != Tnil){
                if (K == temp->key){
                    rankNumber = rankNumber + temp->leftSize;
                    return rankNumber;
                }
                else if (K > temp->key){
                    rankNumber = rankNumber + temp->leftSize + 1;
                    temp = temp->right;
                }
                else if (K < temp->key){
                    temp = temp->left;
                }
            }
            return 0;
        }

        valuetype * search(keytype K){
            Node *s = root;
            while (s != Tnil){
                if (K < s->key){
                    s = s->left;
                }
                else if (K > s->key){
                    s = s->right;
                }
                else if (K == s->key){
                    return &s->value;
                }
                else {
                    break;
                }
            }
            return NULL;
        }

        Node *selectHelper(int pos){
                Node *temp = root;
                int rankNumber = 1;
                while (temp != Tnil){
                    if (temp->leftSize + rankNumber == pos){
                        return temp;
                    }
                    else if (pos > temp->leftSize + rankNumber){
                        rankNumber = rankNumber + temp->leftSize + 1;
                        temp = temp->right;
                    }
                    else if (pos < temp->leftSize + rankNumber){
                        temp = temp->left;
                    }
                    else {
                        break;
                    }
                }
            return NULL;
        }
        keytype select(int pos){
            Node *selected = selectHelper(pos);
            return selected->key;
        }

        keytype *successor(keytype K){
            Node *successorNode;
            int Srank = rank(K);
            if (Srank == 0 || Srank == TreeSize) return NULL;
            else {
                successorNode = selectHelper(Srank+1);
            }
            return &successorNode->key;
        }
        //maxValue helps predecessor() find the biggest node in the left subtree of node sent to it
        keytype *predecessor(keytype K){
            Node *temp;
            int Krank = rank(K);
            if (Krank == 0) return NULL;
            else{
                temp = selectHelper(Krank-1);
                return &temp->key;
            }
        }        

        void RBTransplant(Node* u, Node* v){
            if (u->parent == Tnil){
                root = v;
            }
            else if(u == u->parent->left){
                u->parent->left = v;
            }
            else {
                u->parent->right = v;
            }
            if (v != Tnil){
                v->leftSize = u->leftSize;
            }
            v->parent = u->parent;
        }
        void RBDeleteFixUp(Node *x){
            Node *w;
            while ((x != root) && (x->color == black)){
                if (x == x->parent->left){
                    w = x->parent->right;
                    if (w->color == red){
                        w->color = black;
                        x->parent->color = red;
                        rotateLeft(x->parent);
                        w = x->parent->right;
                    }
                    if ((w->left->color == black) && (w->right->color == black)){
                        w->color = red;
                        x = x->parent;
                    }
                    else {
                        if (w->right->color == black){
                            w->left->color = black;
                            w->color = red;
                            rotateRight(w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = black;
                        w->right->color = black;
                        rotateLeft(x->parent);
                        x = root;
                    }
                }
                else {
                    w = x->parent->left;
                    if (w->color == red){
                        w->color = black;
                        x->parent->color = red;
                        rotateRight(x->parent);
                        w = x->parent->left;
                        cout << w->key << endl;
                    }
                    if ((w->right->color == black) && (w->left->color == black)){
                        w->color = red;
                        x = x->parent;
                    }
                    else {
                        if (w->left->color == black){
                            w->right->color = black;
                            w->color = red;
                            rotateLeft(w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = black;
                        w->left->color = black;
                        rotateRight(x->parent);
                        x = root;
                    }
                }
            }
            x->color = black;
        }
        Node * rminValue(Node *n){
            Node *min = n;
            while (min->left != Tnil){
                min->leftSize--;
                min = min->left;
            }
            return min;
        }
        int remove(keytype K){
            Node *temp = root;
            Node *y;
            Node *x;
            while (temp != Tnil){
                if (K > temp->key){
                    temp = temp->right;
                }
                else if (K < temp->key){
                    temp = temp->left;
                }
                else if (K == temp->key){
                    Node *decreementer = root;
                    while (decreementer != Tnil){
                        if (K > decreementer->key){
                            decreementer = decreementer->right;
                        }
                        else if (K < decreementer->key){
                            decreementer->leftSize--;
                            decreementer = decreementer->left;
                        }
                        else {
                            break;
                        }
                    }
                    Node *z = temp;
                    y = z;
                    bool yOriginalColor = y->color;
                    if (z->left == Tnil){
                        x = z->right;
                        RBTransplant(z, z->right);
                    }
                    else if(z->right == Tnil){
                        x = z->left;
                        RBTransplant(z, z->left);
                    }
                    else {
                        y = rminValue(z->right);
                        yOriginalColor = y->color;
                        x = y->right;
                        if (y->parent == z){
                            x->parent = y;
                        }
                        else {
                            RBTransplant(y, y->right);
                            y->right = z->right;
                            y->right->parent = y;
                        }
                        RBTransplant(z, y);
                        y->left = z->left;
                        y->left->parent = y;
                        y->color = z->color;
                    }
                    if (yOriginalColor == black){
                        RBDeleteFixUp(x);
                    }
                    TreeSize = TreeSize - 1;
                    return 1;
                }
                else {
                    //the key is not in there
                    break;
                }
            }
            //key not in tree
            return 0;
        }
};