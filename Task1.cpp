#include <iostream>
using namespace std;

class MaxHeap {
    int* arr;
    int size;
    int capacity;

    // Swaps two ints. Time: O(1)
    void swapVals(int& a, int& b) { int t = a; a = b; b = t; }

    // Doubles the array when full. Time: O(n)
    void grow() {
        capacity *= 2;
        int* bigger = new int[capacity];
        for (int i = 0; i < size; i++) bigger[i] = arr[i];
        delete[] arr;
        arr = bigger;
    }

    // Moves arr[i] down within the first n elements until heap property holds. Time: O(log n)
    void siftDown(int i, int n) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, largest = i;
            if (l < n && arr[l] > arr[largest]) largest = l;
            if (r < n && arr[r] > arr[largest]) largest = r;
            if (largest == i) break;
            swapVals(arr[i], arr[largest]);
            i = largest;
        }
    }

public:
    MaxHeap(int cap = 4) : size(0), capacity(cap) { arr = new int[capacity]; }
    ~MaxHeap() { delete[] arr; }

    // Inserts value at the end and sifts it up. Returns number of swaps. Time: O(log n)
    int insert(int value) {
        if (size == capacity) grow();
        int i = size++;
        arr[i] = value;
        int swaps = 0;
        while (i > 0 && arr[i] > arr[(i - 1) / 2]) {
            swapVals(arr[i], arr[(i - 1) / 2]);
            i = (i - 1) / 2;
            swaps++;
        }
        return swaps;
    }

    // Prints the heap array. Time: O(n)
    void print() const {
        for (int i = 0; i < size; i++) cout << arr[i] << (i + 1 < size ? " " : "");
        cout << endl;
    }

    // Sorts the array in ascending order in place (the heap is no longer a heap afterwards). Time: O(n log n)
    void heapSort() {
        for (int end = size - 1; end > 0; end--) {
            swapVals(arr[0], arr[end]);
            siftDown(0, end);
        }
    }
};

/*
Q1. Worst-case swaps for one insertion into a heap of n elements:
    the new value can climb from a leaf to the root, so it is the height of the heap = floor(log2(n+1)) = O(log n).

Q2. Building the heap by n insertions: O(n log n) (each insertion is O(log n)).
    Heap sort: O(n log n) (n removals/sift-downs, each O(log n)); building via insertions adds another O(n log n), total still O(n log n).
*/

int main() {
    int input[] = {25, 40, 15, 60, 35, 10, 70, 50};
    int n = 8;
    MaxHeap h;

    cout << "Insert\tSwaps\tHeap after insertion\n";
    for (int i = 0; i < n; i++) {
        int s = h.insert(input[i]);
        cout << input[i] << "\t" << s << "\t";
        h.print();
    }

    cout << "\nFinal max heap: ";
    h.print();

    h.heapSort();
    cout << "Sorted output: ";
    h.print();
    return 0;
}
