#include <iostream>
using namespace std;

template<typename keytype, typename valuetype>
class Heap {
    private:
        bool resize(const int newCapacity){
            keytype *tempkeyArray;
            valuetype *tempvalArray;
            tempkeyArray = new keytype[newCapacity];
            tempvalArray = new valuetype[newCapacity];
            for (int i = 0; i < size; i++){
                tempkeyArray[i] = keyArray[i];
                tempvalArray[i] = valArray[i];
            }

            delete[] keyArray;
            delete[] valArray;
            capacity = newCapacity;
            keyArray = tempkeyArray;
            valArray = tempvalArray;

            return true;
        }
    public:
        int capacity = 0;
        int size = 0;
        keytype *keyArray;
        valuetype *valArray;

        Heap(){
            size = 0;
            capacity = 1;
            keyArray = new keytype[1];
            valArray = new valuetype[1];
        }

        Heap(const Heap<keytype, valuetype>& copy) : capacity{copy.capacity}, size{copy.capacity} {
            keyArray = new keytype[capacity];
            valArray = new valuetype[capacity];
            for (int i = 0; i < size; i++){
                keyArray = copy.keyArray[i];
                valArray = copy.keyArray[i];                
            }
        }

        Heap &operator=(const Heap<keytype, valuetype> &rhs){
            if (this != rhs){
                delete[] keyArray;
                delete[] valArray;
                
                capacity = rhs.capacity;
                size = rhs.size;
                keytype keyA = new keytype[capacity];
                valuetype valA = new valuetype[capacity];
                for (int i = 0; i < size; i++){
                    keyA = rhs.keyArray[i];
                    valA = rhs.keyArray[i];                
                }
            }
            return *this;
        }

        ~Heap(){
            delete[] keyArray;
            delete[] valArray;
        }

        void swapKey(keytype *x, keytype *y){
            keytype temp = *x;
            *x = *y;
            *y = temp;
        }
        void swapVal(valuetype *x, valuetype *y){
            valuetype temp = *x;
            *x = *y;
            *y = temp;
        }

        void heapify(keytype keyArray[], valuetype valArray[], int s, int i){
            //parent = (i-1)/2
            int smallest = i;
            int leftChild = 2*i + 1;
            int rightChild = 2*i + 2;

            if (leftChild < size && keyArray[leftChild] < keyArray[smallest]){
                smallest = leftChild;
            }
            if (rightChild < size && keyArray[rightChild] < keyArray[smallest]){
                smallest = rightChild;
            }
            if (smallest != i){
                swapKey(&keyArray[i], &keyArray[smallest]);
                swapVal(&valArray[i], &valArray[smallest]);

                //recurse on sub-tree
                heapify(keyArray, valArray, s, smallest);
            }
        }

        Heap(keytype k[], valuetype v[], int s){
            capacity = 2*s;
            keyArray = new keytype[2*s];
            valArray = new valuetype[2*s];
            for (int i = 0; i < s; i++){
                size++;
                keyArray[i] = k[i];
                valArray[i] = v[i];
            }
            int startIndex = (s/2) - 1;

            for (int i = startIndex; i >= 0; i--){
                heapify(keyArray, valArray, s, i);
            }
        }

        void printKey(){
            for (int i = 0; i < size; i++){
                cout << keyArray[i] << " ";
            }
            cout << endl;
        }

        keytype peekKey(){
            return keyArray[0];
        }

        valuetype peekValue(){
            return valArray[0];
        }

        void insert(keytype k, valuetype v){
            Heap();
            size++;
            if (size >= capacity) resize(2*capacity);
            keyArray[size-1] = k;
            valArray[size-1] = v;
            
            int startIndex = (size/2) - 1;

            for (int i = startIndex; i >= 0; i--){
                heapify(keyArray, valArray, size, i);
            }
        }

        keytype extractMin(){
            keytype temp = keyArray[0];
            swapKey(&keyArray[0], &keyArray[size-1]);
            swapVal(&valArray[0], &valArray[size-1]);
            if (size < (capacity*0.25)) resize(capacity/2);
            size--;
            int startIndex = (size/2)-1;
            for (int i = startIndex; i >= 0; i--){
                heapify(keyArray, valArray, size-1, i);
            }
            return temp;
        }
};