
// ============================================================
// test_queue.cpp - Bộ kiểm thử cho Queue và QueueEngine (TV4, BeautyStore)
//
// Không cần thư viện ngoài. Mỗi test in PASS hoặc FAIL, cuối cùng in tổng kết.
// Nếu có test lỗi thì chương trình thoát với mã 1.
//
// Biên dịch và chạy (từ thư mục gốc của repo):
//   g++ -std=c++17 -Wall -Wextra -fsanitize=address,undefined -o test_queue tests/test_queue.cpp
//   ./test_queue
// (-fsanitize giúp bắt lỗi bộ nhớ; có thể bỏ nếu máy không hỗ trợ)
// ============================================================

#include <iostream>
#include "../src/queue_engine.h"
using namespace std;

int soTest = 0;
int soLoi = 0;

// Ghi nhận kết quả một test
void kiemTra(bool dieuKien, string tenTest) {
    soTest++;
    if (dieuKien) {
        cout << "PASS  " << tenTest << endl;
    } else {
        soLoi++;
        cout << "FAIL  " << tenTest << endl;
    }
}

// Thêm một mã đơn vào Queue (tạo Node rồi enqueue)
void them(Queue& q, int id) {
    q.enqueue(new Node(id));
}

// Lấy một phần tử ra, trả -1 nếu hàng rỗng
int lay(Queue& q) {
    int id;
    if (q.dequeue(id) == false) {
        return -1;
    }
    return id;
}

// Gửi một lệnh cho Engine và so với kết quả mong đợi
void kiemTraLenh(QueueEngine& engine, string lenh, string mongDoi) {
    string ketQua = engine.xuLyLenh(lenh);
    kiemTra(ketQua == mongDoi, "Lenh \"" + lenh + "\" -> " + mongDoi);
}

// ============================================================
// PHẦN 1: Kiểm thử cấu trúc Queue
// ============================================================
void testQueue() {
    cout << "--- Queue: hang rong ---" << endl;
    {
        Queue q;
        int id = 0;
        kiemTra(q.isEmpty(), "Hang moi tao la hang rong");
        kiemTra(q.getSize() == 0, "Hang moi tao co size = 0");
        kiemTra(q.dequeue(id) == false, "dequeue khi rong tra false");
        kiemTra(q.peek(id) == false, "peek khi rong tra false");
    }

    cout << "--- Queue: mot phan tu ---" << endl;
    {
        Queue q;
        int id = 0;
        them(q, 42);
        kiemTra(q.isEmpty() == false, "Them 1 don thi khong con rong");
        kiemTra(q.getSize() == 1, "Them 1 don thi size = 1");
        kiemTra(q.peek(id) && id == 42, "peek thay don 42");
        kiemTra(lay(q) == 42, "dequeue lay duoc don 42");
        kiemTra(q.isEmpty(), "Lay xong thi hang rong lai");
    }

    cout << "--- Queue: thu tu FIFO ---" << endl;
    {
        Queue q;
        them(q, 5);
        them(q, 7);
        them(q, 9);
        kiemTra(lay(q) == 5, "FIFO: lay lan 1 ra 5");
        kiemTra(lay(q) == 7, "FIFO: lay lan 2 ra 7");
        kiemTra(lay(q) == 9, "FIFO: lay lan 3 ra 9");
        kiemTra(lay(q) == -1, "FIFO: lay lan 4 la hang rong");
    }

    cout << "--- Queue: peek khong xoa ---" << endl;
    {
        Queue q;
        int a = 0, b = 0;
        them(q, 3);
        them(q, 4);
        q.peek(a);
        q.peek(b);
        kiemTra(a == 3 && b == 3, "peek nhieu lan van thay don dau (3)");
        kiemTra(q.getSize() == 2, "peek khong lam giam size");
    }

    cout << "--- Queue: lay het roi them lai ---" << endl;
    {
        Queue q;
        them(q, 4);
        lay(q);
        them(q, 8);          // hang vua rong, them lai phai dung binh thuong
        them(q, 9);
        kiemTra(q.getSize() == 2, "Them lai sau khi lay het: size = 2");
        kiemTra(lay(q) == 8, "Them lai sau khi lay het: ra 8");
        kiemTra(lay(q) == 9, "Them lai sau khi lay het: ra 9");
        kiemTra(q.isEmpty(), "Lay het lan 2 thi hang rong");
    }

    cout << "--- Queue: them va lay xen ke ---" << endl;
    {
        Queue q;
        them(q, 1);
        them(q, 2);
        kiemTra(lay(q) == 1, "Xen ke: ra 1");
        them(q, 3);
        kiemTra(lay(q) == 2, "Xen ke: ra 2");
        kiemTra(lay(q) == 3, "Xen ke: ra 3");
        kiemTra(lay(q) == -1, "Xen ke: het thi rong");
    }

    cout << "--- Queue: size tang giam ---" << endl;
    {
        Queue q;
        them(q, 1);
        them(q, 2);
        them(q, 3);
        kiemTra(q.getSize() == 3, "Them 3 don thi size = 3");
        lay(q);
        kiemTra(q.getSize() == 2, "Lay 1 don thi size = 2");
    }

    cout << "--- Queue: clear ---" << endl;
    {
        Queue q;
        them(q, 1);
        them(q, 2);
        q.clear();
        kiemTra(q.isEmpty() && q.getSize() == 0, "clear xoa sach hang");
        them(q, 6);
        kiemTra(lay(q) == 6, "Dung lai duoc sau khi clear");
        q.clear();
        q.clear();
        kiemTra(q.isEmpty(), "clear khi rong khong loi");
    }

    cout << "--- Queue: nhieu phan tu ---" << endl;
    {
        Queue q;
        int n = 100000;
        for (int i = 1; i <= n; i++) {
            them(q, i);
        }
        kiemTra(q.getSize() == n, "Them 100000 don thi size = 100000");
        bool dungThuTu = true;
        for (int i = 1; i <= n; i++) {
            if (lay(q) != i) {
                dungThuTu = false;
            }
        }
        kiemTra(dungThuTu, "100000 don ra dung thu tu FIFO");
        kiemTra(q.isEmpty(), "Lay het 100000 don thi hang rong");
    }

    cout << "--- Queue: huy khi con phan tu (destructor) ---" << endl;
    {
        Queue q;
        them(q, 1);
        them(q, 2);
        them(q, 3);
    }   // q bi huy o day; chay voi -fsanitize se bao neu ro ri bo nho
    kiemTra(true, "Huy Queue con 3 phan tu (xem ket qua sanitizer)");
}

// ============================================================
// PHẦN 2: Kiểm thử giao thức lệnh của QueueEngine
// ============================================================
void testGiaoThuc() {
    cout << "--- Engine: luong co ban ---" << endl;
    {
        QueueEngine e;
        kiemTraLenh(e, "ENQ 5", "OK");
        kiemTraLenh(e, "ENQ 7", "OK");
        kiemTraLenh(e, "PEEK", "ID 5");
        kiemTraLenh(e, "SIZE", "SIZE 2");
        kiemTraLenh(e, "DEQ", "ID 5");
        kiemTraLenh(e, "DEQ", "ID 7");
        kiemTraLenh(e, "DEQ", "EMPTY");
        kiemTraLenh(e, "PEEK", "EMPTY");
        kiemTraLenh(e, "SIZE", "SIZE 0");
    }

    cout << "--- Engine: CLEAR ---" << endl;
    {
        QueueEngine e;
        kiemTraLenh(e, "ENQ 1", "OK");
        kiemTraLenh(e, "ENQ 2", "OK");
        kiemTraLenh(e, "CLEAR", "OK");
        kiemTraLenh(e, "SIZE", "SIZE 0");
        kiemTraLenh(e, "ENQ 9", "OK");
        kiemTraLenh(e, "DEQ", "ID 9");
        kiemTraLenh(e, "CLEAR", "OK");
    }

    cout << "--- Engine: id hop le va khong hop le ---" << endl;
    {
        QueueEngine e;
        kiemTraLenh(e, "ENQ 1", "OK");
        kiemTraLenh(e, "ENQ 2147483647", "OK");
        kiemTraLenh(e, "ENQ 2147483648", "ERR BAD_ID");
        kiemTraLenh(e, "ENQ 99999999999", "ERR BAD_ID");
        kiemTraLenh(e, "ENQ 0", "ERR BAD_ID");
        kiemTraLenh(e, "ENQ -5", "ERR BAD_ID");
        kiemTraLenh(e, "ENQ abc", "ERR BAD_ID");
        kiemTraLenh(e, "ENQ 1.5", "ERR BAD_ID");
        kiemTraLenh(e, "ENQ 12a", "ERR BAD_ID");
        kiemTraLenh(e, "SIZE", "SIZE 2");
    }

    cout << "--- Engine: lenh sai ---" << endl;
    {
        QueueEngine e;
        kiemTraLenh(e, "ENQ", "ERR BAD_COMMAND");
        kiemTraLenh(e, "ENQ 1 2", "ERR BAD_COMMAND");
        kiemTraLenh(e, "HELLO", "ERR BAD_COMMAND");
        kiemTraLenh(e, "deq", "ERR BAD_COMMAND");
        kiemTraLenh(e, "", "ERR BAD_COMMAND");
        kiemTraLenh(e, "   ", "ERR BAD_COMMAND");
        kiemTraLenh(e, "DEQ 1", "ERR BAD_COMMAND");
        kiemTraLenh(e, "PEEK x", "ERR BAD_COMMAND");
        kiemTraLenh(e, "SIZE 2", "ERR BAD_COMMAND");
        kiemTraLenh(e, "CLEAR 3", "ERR BAD_COMMAND");
    }

    cout << "--- Engine: loi khong lam hong hang ---" << endl;
    {
        QueueEngine e;
        kiemTraLenh(e, "ENQ 1", "OK");
        kiemTraLenh(e, "ENQ x", "ERR BAD_ID");
        kiemTraLenh(e, "HELLO", "ERR BAD_COMMAND");
        kiemTraLenh(e, "ENQ 2", "OK");
        kiemTraLenh(e, "DEQ", "ID 1");
        kiemTraLenh(e, "DEQ", "ID 2");
    }

    cout << "--- Engine: dinh dang dau vao ---" << endl;
    {
        QueueEngine e;
        kiemTraLenh(e, "ENQ 1\r", "OK");        // cuoi dong kieu Windows
        kiemTraLenh(e, "DEQ\r", "ID 1");
        kiemTraLenh(e, "  ENQ    12  ", "OK");  // khoang trang thua
        kiemTraLenh(e, "ENQ 007", "OK");        // so 0 dau: luu thanh 7
        kiemTraLenh(e, "DEQ", "ID 12");
        kiemTraLenh(e, "DEQ", "ID 7");
    }
}

int main() {
    testQueue();
    testGiaoThuc();

    cout << endl << "=== TONG KET: " << (soTest - soLoi) << "/" << soTest << " test dat ===" << endl;
    if (soLoi > 0) {
        cout << "Co " << soLoi << " test LOI" << endl;
        return 1;
    }
    return 0;
}
