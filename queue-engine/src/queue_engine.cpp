// ============================================================
// queue_engine.cpp - Queue Engine cho dự án BeautyStore (TV4)
//
// Hàng đợi (Queue) các mã đơn hàng, tự cài bằng danh sách liên kết đơn.
// Backend gửi lệnh qua stdin, chương trình trả kết quả ra stdout.
// Mỗi lệnh là một dòng, mỗi lần trả lời đúng một dòng.
//
// CÁC LỆNH:
//   ENQ <id>  : thêm đơn vào CUỐI hàng (rear)        -> "OK"
//   DEQ       : lấy đơn ở ĐẦU hàng (front) ra         -> "ID <id>", hoặc "EMPTY"
//   PEEK      : xem đơn ở đầu hàng, không lấy         -> "ID <id>", hoặc "EMPTY"
//   SIZE      : đếm số đơn trong hàng                  -> "SIZE <n>"
//   CLEAR     : xóa sạch hàng                          -> "OK"
//
// LỖI:
//   "ERR BAD_ID"      : id không phải số nguyên dương hợp lệ (1 đến 2147483647)
//   "ERR BAD_COMMAND" : lệnh không tồn tại, hoặc thiếu/thừa tham số
//
// Khi khởi động in "READY". Hết dữ liệu stdin thì thoát với mã 0.
// ============================================================

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// ============================================================
// Node: một phần tử trong hàng đợi
// ============================================================
struct Node {
    int orderId;     // mã đơn hàng
    Node* next;      // trỏ tới node đứng sau nó trong hàng
    Node(int id) {
        orderId = id;
        next = nullptr;
    }
};

// ============================================================
// Queue: hàng đợi FIFO bằng danh sách liên kết
//   front = đầu hàng (người sắp được xử lý)
//   rear  = cuối hàng (người mới đến đứng sau)
//   size  = số phần tử hiện có
// ============================================================
class Queue {
private:
    Node* front;
    Node* rear;
    int size;

public:
    // Hàng mới tạo ra là hàng rỗng
    Queue() {
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

    // Khi Queue bị hủy thì giải phóng hết node để không rò rỉ bộ nhớ
    ~Queue() {
        clear();
    }

    // Không cho sao chép Queue (tránh hai Queue cùng giữ một danh sách rồi delete hai lần)
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    // ---------- isEmpty: hàng có rỗng không ----------  O(1)
    bool isEmpty() {
        return front == nullptr;
    }

    // ---------- getSize: số phần tử ----------  O(1)
    int getSize() {
        return size;
    }

    // ---------- enqueue: thêm node vào CUỐI hàng ----------  O(1)
    void enqueue(Node* node) {
        node->next = nullptr;          // node mới đứng cuối nên chưa có ai phía sau
        if (isEmpty()) {
            front = node;              // hàng rỗng: front và rear cùng trỏ node mới
            rear = node;
        } else {
            rear->next = node;         // nối node mới vào sau rear
            rear = node;               // rear dời sang node mới
        }
        size++;
    }

    // ---------- dequeue: lấy node ở ĐẦU hàng ----------  O(1)
    // Trả true và gán mã đơn vào orderId nếu lấy được; trả false nếu hàng rỗng.
    bool dequeue(int& orderId) {
        if (isEmpty()) {
            return false;
        }
        Node* cu = front;              // nhớ node đầu hàng để lát nữa xóa
        orderId = cu->orderId;         // lấy mã đơn ra
        front = front->next;           // front dời sang node kế tiếp
        if (front == nullptr) {
            rear = nullptr;            // vừa lấy phần tử cuối: rear cũng phải về nullptr
        }
        delete cu;                     // giải phóng node cũ
        size--;
        return true;
    }

    // ---------- peek: xem đầu hàng, không xóa ----------  O(1)
    bool peek(int& orderId) {
        if (isEmpty()) {
            return false;
        }
        orderId = front->orderId;
        return true;
    }

    // ---------- clear: xóa sạch hàng ----------  O(n)
    void clear() {
        while (front != nullptr) {
            Node* tam = front;
            front = front->next;
            delete tam;
        }
        rear = nullptr;
        size = 0;
    }
};

// ============================================================
// QueueEngine: nhận từng dòng lệnh, gọi Queue, trả về một dòng kết quả
// ============================================================
class QueueEngine {
private:
    Queue hangDoi;

    // Id hợp lệ: toàn chữ số, nằm trong khoảng 1 đến 2147483647.
    // Hợp lệ thì gán vào ketQua và trả true.
    bool laIdHopLe(string chuoi, int& ketQua) {
        // Chuỗi rỗng hoặc dài quá 10 ký tự thì chắc chắn sai
        if (chuoi.empty() || chuoi.size() > 10) {
            return false;
        }
        // Duyệt từng ký tự, chỉ chấp nhận chữ số từ '0' đến '9'
        long long giaTri = 0;
        int doDai = chuoi.size();
        for (int i = 0; i < doDai; i++) {
            char kyTu = chuoi[i];
            if (kyTu < '0' || kyTu > '9') {
                return false;                          // gặp chữ cái hoặc dấu '-' là sai
            }
            giaTri = giaTri * 10 + (kyTu - '0');       // ghép từng chữ số thành số
        }
        // Id phải từ 1 trở lên và không vượt quá giới hạn của kiểu int
        if (giaTri < 1 || giaTri > 2147483647) {
            return false;
        }
        ketQua = giaTri;
        return true;
    }

    // ---------- ENQ ----------
    string xuLyENQ(string chuoiId) {
        int id;
        if (laIdHopLe(chuoiId, id) == false) {
            return "ERR BAD_ID";
        }
        Node* node = new Node(id);     // tạo node mới chứa mã đơn
        hangDoi.enqueue(node);         // thêm vào cuối hàng
        return "OK";
    }

    // ---------- DEQ ----------
    string xuLyDEQ() {
        int id;
        if (hangDoi.dequeue(id) == false) {
            return "EMPTY";
        }
        return "ID " + to_string(id);
    }

    // ---------- PEEK ----------
    string xuLyPEEK() {
        int id;
        if (hangDoi.peek(id) == false) {
            return "EMPTY";
        }
        return "ID " + to_string(id);
    }

    // ---------- SIZE ----------
    string xuLySIZE() {
        return "SIZE " + to_string(hangDoi.getSize());
    }

    // ---------- CLEAR ----------
    string xuLyCLEAR() {
        hangDoi.clear();
        return "OK";
    }

public:
    // Hàm duy nhất bên ngoài cần gọi: nhận một dòng lệnh, trả về một dòng kết quả
    string xuLyLenh(string dong) {
        // File lệnh từ Windows có thêm ký tự '\r' cuối dòng, bỏ nó đi
        if (dong.empty() == false && dong.back() == '\r') {
            dong.pop_back();
        }
        // Tách dòng thành các từ, ví dụ "ENQ 5" -> lenh = "ENQ", thamSo = "5"
        // "thua" dùng để phát hiện gõ thừa từ (ví dụ "ENQ 5 6")
        istringstream tachTu(dong);
        string lenh, thamSo, thua;
        tachTu >> lenh >> thamSo >> thua;
        // ENQ phải có đúng 1 tham số
        if (lenh == "ENQ") {
            if (thamSo.empty() || thua.empty() == false) {
                return "ERR BAD_COMMAND";
            }
            return xuLyENQ(thamSo);
        }
        // Các lệnh còn lại không có tham số, nên thamSo phải rỗng
        if (lenh == "DEQ" && thamSo.empty()) {
            return xuLyDEQ();
        }
        if (lenh == "PEEK" && thamSo.empty()) {
            return xuLyPEEK();
        }
        if (lenh == "SIZE" && thamSo.empty()) {
            return xuLySIZE();
        }
        if (lenh == "CLEAR" && thamSo.empty()) {
            return xuLyCLEAR();
        }
        return "ERR BAD_COMMAND";      // không khớp lệnh nào
    }
};

// ============================================================
// main: đọc từng dòng, đưa cho QueueEngine xử lý, rồi in kết quả
// ============================================================
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
