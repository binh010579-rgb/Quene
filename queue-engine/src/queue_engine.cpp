// ============================================================
// queue_engine.cpp - chương trình Queue Engine (TV4, dự án BeautyStore)
//
// Backend gửi lệnh qua stdin, chương trình trả kết quả ra stdout.
// Mỗi lệnh là một dòng, mỗi lần trả lời đúng một dòng.
// Khi khởi động in "READY". Hết dữ liệu stdin thì thoát với mã 0.
// Danh sách lệnh và mã lỗi xem trong queue_engine.h.
//
// Biên dịch: g++ -std=c++17 -o queue_engine src/queue_engine.cpp
// ============================================================

#include <iostream>
#include "queue_engine.h"
using namespace std;

// main: đọc từng dòng, đưa cho QueueEngine xử lý, rồi in kết quả
int main() {
    QueueEngine engine;
    // Báo cho Backend biết đã sẵn sàng. "endl" vừa xuống dòng vừa đẩy chữ ra ngay.
    cout << "READY" << endl;
    string dong;
    while (getline(cin, dong)) {                  // đọc từng dòng cho đến khi hết dữ liệu
        cout << engine.xuLyLenh(dong) << endl;    // xử lý và in đúng một dòng kết quả
    }
    return 0;
}
