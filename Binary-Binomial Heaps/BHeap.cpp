#include <iostream>
#include <queue>
#include <stack>

using namespace std;

template <typename keytype, typename valuetype>
class BHeap {
    private:
        struct Node{
            keytype key;
            valuetype value;
            int degree;
            Node *parent; 
            Node *child;
            Node *sibling;
            Node() {
                this->parent = nullptr;
                this->sibling = nullptr;
                this->child = nullptr;
            }
        };
        typedef Node *nodePtr;
    
        nodePtr head;   //points to the root of the leftmost tree
        nodePtr firstNode = new Node;

        void initializeNode(nodePtr node, keytype key, valuetype value, int degree){
            node->key = key;
            node->value = value;
            node->degree = degree;
            node->parent = nullptr;
            node->child = nullptr;
            node->sibling = nullptr;
        }

        //combine trees of same degree
        static void linkTrees(nodePtr x, nodePtr y){
            y->parent = x;
            y->sibling = x->child;
            x->child = y;
            x->degree++;
        }
    
    public:
        BHeap(){
            firstNode->degree = -1;
            setHead(firstNode);
            //head = nullptr;
        }

        nodePtr getHead(){
            return head;
        }
        
        void setHead(nodePtr head){
            this->head = head;
        }

        keytype peekKey(){
            keytype min = head->key;
            nodePtr current = head;
            nodePtr minPtr = head;

            while (current != nullptr){
                if (current->key < min){
                    min = current->key;
                    minPtr = current;
                }
                current = current->sibling;
            }
            return min;
        }

        valuetype peekValue(){
            keytype min = head->key;
            nodePtr current = head;
            nodePtr minPtr = head;

            while (current != nullptr){
                if (current->key < min){
                    min = current->key;
                    minPtr = current;
                }
                current = current->sibling;
            }
            return minPtr->value;
        }

        void merge(BHeap<keytype, valuetype> &H2){
            nodePtr curr1 = getHead();
            nodePtr curr2 = H2.getHead();
            nodePtr curr3 = nullptr;
            nodePtr temp = nullptr;

            if (curr1->degree <= curr2->degree){
                curr3 = curr1;
                curr1 = curr1->sibling;
            }
            else {
                curr3 = curr2;
                curr2 = curr2->sibling;
            }
            //smallest key will be make the head of the merged heap
            temp = curr3;

            //just put the heaps together, doesnt take care of same degree trees yet 
            //ascending order
            while(curr1 != nullptr && curr2 != nullptr){
                if (curr1->degree <= curr2->degree) {
				    curr3->sibling = curr1;
				    curr1 = curr1->sibling;
			    }
                else {
				    curr3->sibling = curr2;
				    curr2 = curr2->sibling;
			    }
			    curr3 = curr3->sibling;
            }

            if (curr1 != nullptr){
                while (curr1 != nullptr){
                    curr3->sibling = curr1;
                    curr1 = curr1->sibling;
                    curr3 = curr3->sibling;
                }
            }

            if (curr2 != nullptr){
                while(curr2 != nullptr){
                    curr3->sibling = curr2;
                    curr2 = curr2->sibling;
                    curr3 = curr3->sibling;
                }
            }

            //link trees with same degree
            curr3 = temp;
            nodePtr prev = nullptr;
            nodePtr next = curr3->sibling;

            while (next != nullptr) {
                // if two adjacent tree roots have different degree or 3 consecutive roots have same degree
                // move to the next tree
                if ((curr3->degree != next->degree )|| (next->sibling != nullptr && curr3->degree == next->sibling->degree)) {
                    prev = curr3;
                    curr3 = next;
                } 
                else {
                    if (curr3->key <= next->key) {
					    curr3->sibling = next->sibling;
					    linkTrees(curr3, next);
				    } 
                    else {
					    if (prev == nullptr) {
						    temp = next;
					    } 
                        else {
						    prev->sibling = next;
					    }

					    linkTrees(next, curr3);
					    curr3 = next;
				    }
			    }   
			    next = curr3->sibling;
		    }
		    setHead(temp);
        }

        //FIX ME OR BHEAP()
        void insert(keytype k, valuetype v){
            if (firstNode->degree != -1){ 
                BHeap h1;
                nodePtr node = new Node;
                initializeNode(node, k, v, 0);
                h1.setHead(node);
                merge(h1);
            }

            if (firstNode->degree == -1){
                firstNode->key = k; firstNode->value = v; firstNode->degree = 0;
                firstNode->child = nullptr; firstNode->sibling = nullptr; 
                firstNode->parent = nullptr;
            }
        }

        BHeap(keytype k[], valuetype v[], int s){
            nodePtr firstNode = new Node;
            initializeNode(firstNode, k[0], v[0], 0);
            setHead(firstNode);

            for (int i = 1; i < s; i++){
                insert(k[i], v[i]);
            }
        }

        keytype extractMin(){
            nodePtr current = head;
            nodePtr prevMin = nullptr;
            nodePtr minPtr = head;
            nodePtr prevPtr = nullptr;
            keytype min = head->key;

            while (current != nullptr){
                if (current->key <= min){
                    min = current->key;
                    prevMin = prevPtr;
                    minPtr = current;
                }
                prevPtr = current;
                current = current->sibling;
            }

            // delete the node pointed by minPtr
            if (prevMin != nullptr && minPtr->sibling != nullptr) {
                prevMin->sibling = minPtr->sibling;
            } 
            else if (prevMin != nullptr && minPtr->sibling == nullptr) {
                prevMin->sibling = nullptr;
            }

            //remove parent refrence from all minPtr children
            nodePtr childPtr = minPtr->child;
            while (childPtr != nullptr) {
                childPtr->parent = nullptr;
                childPtr = childPtr->sibling;
            }

            // reverse the order 
            stack<nodePtr> s;
            childPtr = minPtr->child;
            while (childPtr != nullptr) {
                s.push(childPtr);
                childPtr = childPtr->sibling;
            }

            current = s.top();
            nodePtr temp = current;
            s.pop();

            while (!s.empty()) {
                current->sibling = s.top();
                s.pop();
                current = current->sibling;
            }
            current->sibling = nullptr;

		    BHeap h;
		    h.setHead(temp);

		    // merge 
		    merge(h);
            return minPtr->key;
        }

        void printHelp(nodePtr current){
            nodePtr tree = current;
            cout << tree->key << " ";
            nodePtr child = tree->child;
            if (child != nullptr){
                while (child != tree->sibling){
                    cout << child->key << " ";
                    if (child->child == nullptr){
                        child = child->parent->sibling;
                    }
                    else {
                        child = child->child;
                    }
                }
            }
            cout << endl << endl;
        }

        void printKey(){
            nodePtr current = getHead();
            while (current != nullptr){
                cout << "B" << current->degree << endl;
                printHelp(current);
                current = current->sibling;
            }
        }

        ~BHeap(){
            setHead(nullptr);
        }

        void copyHelp(nodePtr old, nodePtr p){
            if (old->child == nullptr){
                p->child = nullptr;
                return;
            }
            nodePtr temp = new Node;
            nodePtr old_trkr = old->child;
            temp->key = old_trkr->key;
            temp->value = old_trkr->value;
            temp->degree = old_trkr->degree;
            p->child = temp;
            temp->parent = p;
            old_trkr = old_trkr->sibling;
            while (old_trkr != nullptr){
                nodePtr temp2 = new Node;
                temp2->parent = p;
                temp2->key = old_trkr->key;
                temp2->value = old_trkr->value;
                temp2->degree = old_trkr->degree;
                temp->sibling = temp2;
                temp2->child = copyHelp(old_trkr, temp2);
                old_trkr = old_trkr->sibling;
                temp = temp2;
            }
            return;
        }

        BHeap(const BHeap<keytype, valuetype> &copy){
            nodePtr old, temp;
            old = copy.getHead();
            while (copy != nullptr){
                temp = new Node;
                temp->key = copy->key;
                temp->value = copy->value;
                temp->degree = copy->degree;
                temp->child = copyHelp(copy, temp);
                copy = old->sibling;
            }
        }

        BHeap &operator=(const BHeap<keytype, valuetype> &copy){
            if (this != copy){
                ~BHeap();

                nodePtr old, temp;
                old = copy.getHead();
                while (copy != nullptr){
                    temp = new Node;
                    temp->key = copy->key;
                    temp->value = copy->value;
                    temp->degree = copy->degree;
                    temp->child = copyHelp(copy, temp);
                    copy = old->sibling;
                }
            }
            return *this;
        }
};