#include <iostream>
#include <stdlib.h>
using namespace std;

#define INDEX(j) ((frontIndex + j) % capacity)
 
    template <class T> class CDA {

            bool resize(const int newCapacity){
                    T *temp;
                    temp = new T[newCapacity];
                    for (int i = 0; i < size; i++){
                        temp[i] = array[INDEX(i)]; 
                    }
                    
                    delete[] array;
                    capacity = newCapacity;
                    frontIndex = 0;
                    array = temp;
                    //temp = nullptr;
                return true;
            }
    
        public:
            T *array;
            bool ordered; 
            int capacity;
            int size;
            int frontIndex;

            CDA(){
                capacity = 1;
                size = 0;
                frontIndex = 0;
                array = new T[capacity];
                ordered = false;
            }

            CDA(int s){
                capacity = s;
                size = s;   
                frontIndex = 0;
                array = new T[capacity];
                ordered = false;
            }

            ~CDA(){
                delete[] array;
                //array = nullptr;
            }

            T& operator[](int i){
                if (i >= size) {
                    cout << "Out of bounds index" << endl;
                    return array[INDEX(i)];
                }
                else {
                    int correctIndex = INDEX(i);
                    return array[correctIndex];
                }
            }

            //copy constructor
            CDA(const CDA<T>& copy) : capacity{copy.capacity}, size{copy.size}, frontIndex{0} {
                array = new T[capacity];
                for (int i = 0; i < size; i++){
                    array[i] = copy.array[(copy.frontIndex + i) % copy.capacity];
                }
            }

            //copy assignment
            CDA& operator=(const CDA<T> &rhs){
                if (this != &rhs){
                    delete[] array;

                    size = rhs.size;
                    capacity = rhs.capacity;
                    frontIndex = 0;
                    array = new T[capacity];
                    for (int i = 0; i < size; i++){
                        array[i] = rhs.array[(rhs.frontIndex + i) % rhs.capacity];
                    }
                }
                return (*this);
            }

            void AddEnd(T v){
                if (size >= capacity) resize(capacity * 2);
                array[(frontIndex + size) % capacity] = v;
                size++;
                if (ordered == true){
                    for (int i = 1; i < size; i++){
                        if (array[INDEX(i)] < array[INDEX(i-1)]) {
                            ordered = false;
                            break;
                        }
                        if (ordered == false){
                            break;
                        }
                    }
                }
            }

            void AddFront(T v){
                if (size >= capacity) resize(capacity * 2);
                if (frontIndex == 0) {
                    frontIndex = capacity - 1;
                }
                else {
                    frontIndex--;
                }
                size++;
                array[frontIndex] = v;
                if (ordered == true){
                    for (int i = 1; i < size; i++){
                        if (array[INDEX(i)] < array[INDEX(i-1)]) {
                            ordered = false;
                            break;
                        }
                        if (ordered == false){
                            break;
                        }
                    }
                }
            }

            void DelEnd(){
                if (size == (capacity * 0.25)) resize(capacity/2);
                --size;
            }

            void DelFront(){
                if (size == (capacity * 0.25)) resize(capacity/2);
                frontIndex = (frontIndex+1) % capacity;
                --size;
            }

            bool Ordered(){
                return ordered;
            }

            int Length(){
                return size;
            }

            int Capacity() {
                return capacity;
            }

            void Clear(){
                if (size == 0) return;
                capacity = 1;
                size = 0;
                frontIndex = 0;
            }

            int SetOrdered(){
                ordered = true;
                for (int i = 1; i < size; i++){
                    if (array[INDEX(i)] < array[INDEX(i-1)]) {
                        ordered = false;
                        return -1;
                    }
                }
                return 1;
            }
            
            int part(int left, int right, int pivotIndex){
	            T pivot = array[INDEX(pivotIndex)];
	            swap(pivotIndex, right);
	            int pIndex = left;
	            int i;
	            for (i = left; i < right; i++)
	            {
		            if (array[INDEX(i)] <= pivot)
		            {
			            swap(i, pIndex);
			            pIndex++;
		            }
	            }
	            swap(pIndex, right);
	            return pIndex;
            }
            T quickSelect(int left, int right, int k){
	            if (left == right)
		            return array[INDEX(left)];

	            int pivotIndex = left + rand() % (right - left + 1);

	            pivotIndex = part(left, right, pivotIndex);

	            if (k == pivotIndex) 
		            return array[INDEX(k)];

	            else if (k < pivotIndex) 
		            return quickSelect(left, pivotIndex - 1, k);
	            else
		            return quickSelect(pivotIndex + 1, right, k);
            }
            T Select(int k){
                // USING QUICKSORT THEN CALLING THE SELECT FUNCTION WORKS FASTER THAN USING THE QUICKSELECT ALGORITHM
                if (ordered == true) {
                    return array[INDEX(k-1)];
                }
                else {
                    //T element = quickSelect(0, size-1, k-1);
                    T element = quickSelect(0, size-1, k-1);
                    //QuickSort();      
                    //Select(k);
                    return element;
                }
            }

            void InsertionSort(){
                if (ordered == true) return;
                T key;
                int j;
                for (int i = 1; i < size; i++){
                    key = array[INDEX(i)];
                    j = i - 1;
                    while (j >= 0 && array[INDEX(j)] > key){
                        array[INDEX(j+1)] = array[INDEX(j)];
                        j = j-1;
                    }
                    array[INDEX(j+1)] = key;
                }
                ordered = true;
            }

            void swap(int a, int b){
                T *temp;
                temp = new T[1];
                temp[0] = array[INDEX(a)]; 
                array[INDEX(a)] = array[INDEX(b)]; 
                array[INDEX(b)] = temp[0]; 
            }
            
            int partition(int first, int last){
                int pivot, mid = (first + last) / 2;

                pivot = array[INDEX(first)] > array[INDEX(mid)] ? first : mid;

                if (array[INDEX(pivot)] > array[INDEX(last)]){
                    pivot = last;
                }
                
                swap(pivot, first);
                pivot = first;

                while (first < last){
                    if (array[INDEX(first)] <= array[INDEX(last)]){
                        swap(pivot, first);
                    }
                    first++;
                }
                swap(pivot, last);

                return pivot;
            }

            void qs(int first, int last){
                int Threshold = 1000;
                if (last - first > Threshold) {
                    if (SetOrdered() == 1) return;

                    int pivot = partition(first, last);

                    qs(first, pivot - 1);
                    qs(pivot + 1, last);
                }
                else {
                    return;
                }
            }

            void QuickSort(){
                if (ordered == true) return;
                int lastIndex = INDEX(size)-1;
                qs(frontIndex, lastIndex);
                InsertionSort();
                ordered = true;
            }

            void CountingSort(int m){
                if (ordered == true) return;
                int countArray[m+1]; for (int i = 0; i < m+1; i++) countArray[i] = 0;
                T *tempArray; tempArray = new T[capacity];
                for (int i = 0; i < size; i++){
                    countArray[array[INDEX(i)]]++;
                }
                for (int i = 1; i < m+1; i++){
                    countArray[i] += countArray[i-1];
                }
                // for (int i = 0; i < size; i++){
                //     tempArray[countArray[array[INDEX(i)]]-1] = array[INDEX(i)];
                //     --countArray[array[INDEX(i)]];
                // }
                for (int i = size-1; i >= 0; i--){
                    tempArray[countArray[array[INDEX(i)]]-1] = array[INDEX(i)];
                    --countArray[array[INDEX(i)]];
                }
                for (int i = 0; i < size; i++){
                    array[INDEX(i)] = tempArray[i];
                }
                ordered = true;
            }

            int binarySearch(T e, int l, int r){
                int middle = ((l+r)%capacity)/2;
                while (l <= r){
                    if (array[INDEX(middle)] < e){
                        l = middle + 1;
                    }
                    else if (array[INDEX(middle)] == e){
                        return middle;
                    }
                    else {
                        r = middle - 1;
                    }
                    middle = ((l + r)%capacity)/2;
                }
                return -1;
            }

            int Search(T e){
                if (ordered == true){
                    int itemIndex = binarySearch(e, 0, size-1);
                    if (itemIndex == -1){
                        return -1;
                    }
                    else {
                        return itemIndex;
                    }
                }
                else {
                    for (int i = 0; i < size; i++){
                        if (array[i] == e){
                            return i;
                        }
                    }
                    return -1;
                }
            }
    };