#include <iostream>
#include "../src/queue_engine.h"
using namespace std;

int main() {
    Queue q;
    int idDau = 0;

    // 1. Test Enqueue vao hang rong
    q.enqueue(new Node(101));
    cout << "Test 1 - Hang rong them 101 -> Size: " << q.getSize() << endl;

    // 2. Test Enqueue nhieu don A(201) -> B(202) -> C(203)
    Queue q2;
    q2.enqueue(new Node(201));
    q2.enqueue(new Node(202));
    q2.enqueue(new Node(203));

    q2.peek(idDau);
    cout << "Test 2 - Front sau khi them A, B, C la: " << idDau << " (Mong doi: 201)" << endl;

    int layRa;
    q2.dequeue(layRa); cout << "Lay lan 1: " << layRa << endl;
    q2.dequeue(layRa); cout << "Lay lan 2: " << layRa << endl;
    q2.dequeue(layRa); cout << "Lay lan 3: " << layRa << endl;

    return 0;
}
// cách chạy pass 2 câu này g++ -std=c++17 queue-engine/tests/test_queue.cpp -o test_queue.exe
// .\test_queue.exe
