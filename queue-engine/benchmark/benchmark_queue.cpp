#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "../src/queue_engine.h" // Gọi trực tiếp Queue danh sách liên kết của TV4

using namespace std;
using namespace std::chrono;

// Hàng đợi cài đặt bằng Mảng để so sánh đối chứng
class ArrayQueue {
private:
    int* arr;
    int capacity;
    int count;

public:
    ArrayQueue(int maxCap) {
        capacity = maxCap;
        arr = new int[capacity];
        count = 0;
    }

    ~ArrayQueue() {
        delete[] arr;
    }

    bool isEmpty() {
        return count == 0;
    }

    // Thêm vào cuối mảng: O(1)
    void enqueue(int orderId) {
        if (count < capacity) {
            arr[count] = orderId;
            count++;
        }
    }

    // Lấy ở đầu mảng và dời toàn bộ phần tử sau lên: O(n)
    bool dequeue(int& orderId) {
        if (isEmpty()) {
            return false;
        }
        orderId = arr[0];
        for (int i = 0; i < count - 1; i++) {
            arr[i] = arr[i + 1];
        }
        count--;
        return true;
    }
};

// Hàm đo thời gian cho Queue Danh sách liên kết (TV4)
double doThoiGianLinkedList(int n) {
    Queue llQueue;
    int idLayRa = 0;

    auto batDau = high_resolution_clock::now();
    for (int i = 1; i <= n; i++) {
        llQueue.enqueue(new Node(i));
    }
    while (!llQueue.isEmpty()) {
        llQueue.dequeue(idLayRa);
    }
    auto ketThuc = high_resolution_clock::now();

    duration<double, milli> thoiGian = ketThuc - batDau;
    return thoiGian.count();
}

// Hàm đo thời gian cho Queue Mảng
double doThoiGianArray(int n) {
    ArrayQueue arrQueue(n);
    int idLayRa = 0;

    auto batDau = high_resolution_clock::now();
    for (int i = 1; i <= n; i++) {
        arrQueue.enqueue(i);
    }
    while (!arrQueue.isEmpty()) {
        arrQueue.dequeue(idLayRa);
    }
    auto ketThuc = high_resolution_clock::now();

    duration<double, milli> thoiGian = ketThuc - batDau;
    return thoiGian.count();
}

int main() {
    vector<int> cacMoc = {1000, 10000, 100000};

    cout << "   BENCHMARK HIEU NANG QUEUE: LINKED LIST (TV4) VS ARRAY (MANG)       " << endl;
    cout << "   Thao tac do: Enqueue n don hang + Dequeue toan bo n don hang       " << endl;
    cout << left << setw(15) << "So don (n)"
         << setw(22) << "Linked List (ms)"
         << setw(22) << "Array Queue (ms)"
         << setw(15) << "Chenh lech" << endl;
    cout << "----------------------------------------------------------------------" << endl;

    for (int n : cacMoc) {
        double tgLinkedList = doThoiGianLinkedList(n);
        double tgArray = doThoiGianArray(n);
        double tyLe = (tgLinkedList > 0) ? (tgArray / tgLinkedList) : 0;

        cout << left << setw(15) << n
             << setw(22) << fixed << setprecision(4) << tgLinkedList
             << setw(22) << fixed << setprecision(4) << tgArray
             << "Nhanh hon " << fixed << setprecision(1) << tyLe << "x" << endl;
    }

    return 0;
}
