#include <iostream>
using namespace std;

class Heap {
    int* arr;
    int size;
    int capacity;
    bool isMax;   // true = max heap, false = min heap

    // Returns true if a should sit above b. Time: O(1)
    bool higher(int a, int b) const { return isMax ? a > b : a < b; }

    // Swaps two ints. Time: O(1)
    void swapVals(int& a, int& b) { int t = a; a = b; b = t; }

    // Doubles the array when full. Time: O(n), amortized O(1) per push
    void grow() {
        capacity *= 2;
        int* bigger = new int[capacity];
        for (int i = 0; i < size; i++) bigger[i] = arr[i];
        delete[] arr;
        arr = bigger;
    }

public:
    Heap(bool maxHeap, int cap = 4) : size(0), capacity(cap), isMax(maxHeap) { arr = new int[capacity]; }
    ~Heap() { delete[] arr; }

    int getSize() const { return size; }
    bool empty() const { return size == 0; }

    // Returns the root value (caller must check not empty). Time: O(1)
    int top() const { return arr[0]; }

    // Adds a value and sifts it up. Time: O(log n)
    void push(int value) {
        if (size == capacity) grow();
        int i = size++;
        arr[i] = value;
        while (i > 0 && higher(arr[i], arr[(i - 1) / 2])) {
            swapVals(arr[i], arr[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    // Removes and returns the root, then sifts the moved last element down. Time: O(log n)
    int pop() {
        int root = arr[0];
        arr[0] = arr[--size];
        int i = 0;
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, best = i;
            if (l < size && higher(arr[l], arr[best])) best = l;
            if (r < size && higher(arr[r], arr[best])) best = r;
            if (best == i) break;
            swapVals(arr[i], arr[best]);
            i = best;
        }
        return root;
    }
};

class WaitMonitor {
    Heap low;    // max heap: smaller half (holds the extra element)
    Heap high;   // min heap: larger half

public:
    WaitMonitor() : low(true), high(false) {}

    // Inserts a wait into the correct heap, then rebalances so sizes differ by at most one. Time: O(log n)
    void addWait(int x) {
        if (low.empty() || x <= low.top()) low.push(x);
        else high.push(x);

        if (low.getSize() > high.getSize() + 1) high.push(low.pop());
        else if (high.getSize() > low.getSize()) low.push(high.pop());
    }

    // Returns the median of all waits so far. Time: O(1)
    double getMedian() const {
        if (low.getSize() > high.getSize()) return low.top();
        return (low.top() + high.top()) / 2.0;
    }

    int maxRoot() const { return low.top(); }
    bool hasMinRoot() const { return !high.empty(); }
    int minRoot() const { return high.top(); }
};

/*
Q7. The average is dragged by extreme values, the median is not. At arrival 6 (45,20,60,15,30,90) the average is
    260/6 = 43.3 (pulled up by the 90), while the median is 37.5. The median reflects what a typical patient waits.

Q8. If the sizes differ by 2 or more, the roots are no longer the middle of the data, so the median formula
    (one root, or the average of two roots) gives a wrong answer.

Q9. Repeated sorting: sort after every arrival, about k log k for k = 1..n, total roughly O(n^2 log n).
    Heap method: O(log n) per arrival and O(1) per median, total O(n log n).
*/

int main() {
    int waits[] = {45, 20, 60, 15, 30, 90, 10, 55};
    int n = 8;
    const double THRESHOLD = 35.0;
    WaitMonitor m;

    cout << "Arrival\tWait\tMaxRoot\tMinRoot\tMedian\tStatus\n";
    for (int i = 0; i < n; i++) {
        m.addWait(waits[i]);
        double med = m.getMedian();
        cout << (i + 1) << "\t" << waits[i] << "\t" << m.maxRoot() << "\t";
        if (m.hasMinRoot()) cout << m.minRoot(); else cout << "none";
        cout << "\t" << med << "\t" << (med > THRESHOLD ? "ALERT: call extra staff" : "Normal") << endl;
    }
    return 0;
}
