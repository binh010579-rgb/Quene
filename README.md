# Queue Engine – BeautyStore (TV4)

Chương trình C++ giữ **hàng đợi FIFO các mã đơn hàng**, cài bằng danh sách liên kết đơn.
Backend gửi lệnh qua **stdin**, chương trình trả kết quả qua **stdout**. Mỗi lệnh là một dòng, mỗi lần trả lời đúng một dòng.

Queue Engine **không truy cập Database** và không biết thông tin khách hàng. Nó chỉ giữ thứ tự các mã đơn đang chờ xử lý. Database vẫn là nguồn dữ liệu chính.

## Cấu trúc thư mục

```
queue-engine/
├── src/
│   ├── queue_engine.h      # Node, Queue (danh sách liên kết), QueueEngine (xử lý lệnh)
│   └── queue_engine.cpp    # hàm main: đọc lệnh từ stdin, in kết quả ra stdout
├── tests/
│   └── test_queue.cpp      # bộ kiểm thử Queue và giao thức lệnh
└── README.md
```

## Biên dịch và chạy

Chạy các lệnh dưới đây **trong thư mục `queue-engine/`**. Cần trình biên dịch hỗ trợ C++17 (g++).

```
g++ -std=c++17 -Wall -Wextra -o queue_engine src/queue_engine.cpp
```

Chạy tương tác (gõ lệnh từng dòng, kết thúc bằng Ctrl+Z rồi Enter trên Windows, hoặc Ctrl+D trên Linux/macOS):

```
.\queue_engine        # Windows PowerShell
./queue_engine        # Linux / macOS
```

## Chạy bộ kiểm thử

```
g++ -std=c++17 -Wall -Wextra -o test_queue tests/test_queue.cpp
.\test_queue          # Windows PowerShell
./test_queue          # Linux / macOS
```

Dòng cuối phải là `=== TONG KET: 80/80 test dat ===`. Có test lỗi thì chương trình thoát với mã 1.

Để kiểm tra thêm lỗi bộ nhớ (Linux/macOS hoặc MinGW có hỗ trợ), thêm `-fsanitize=address,undefined -g` vào lệnh biên dịch.

## Giao thức Backend – Queue Engine

Mỗi lệnh là một dòng văn bản kết thúc bằng xuống dòng. Engine trả đúng **một dòng** phản hồi cho mỗi lệnh và đẩy ra ngay (flush). Tên lệnh **viết hoa đúng như bảng**; `deq` viết thường là lệnh sai.

| Lệnh gửi | Phản hồi | Ý nghĩa |
|---|---|---|
| `ENQ <id>` | `OK` hoặc `ERR BAD_ID` | Thêm id vào cuối hàng (rear) |
| `DEQ` | `ID <id>` hoặc `EMPTY` | Lấy và xóa id ở đầu hàng (front) |
| `PEEK` | `ID <id>` hoặc `EMPTY` | Xem id ở đầu hàng, không xóa |
| `SIZE` | `SIZE <n>` | Số phần tử hiện có |
| `CLEAR` | `OK` | Xóa sạch hàng |
| Lệnh không hợp lệ | `ERR BAD_COMMAND` | Engine bỏ qua và vẫn chạy tiếp |

### Quy ước

- **Khởi động:** Engine in một dòng `READY` khi sẵn sàng nhận lệnh. Backend nên đợi dòng này rồi mới gửi lệnh.
- **Id hợp lệ:** số nguyên dương từ `1` đến `2147483647`. Số 0, số âm, chữ, số thập phân, số quá lớn đều trả `ERR BAD_ID`. Số có số 0 đứng đầu (`007`) được chấp nhận và lưu là `7`.
- **`ERR BAD_COMMAND`:** lệnh lạ, dòng trống, `ENQ` thiếu hoặc thừa tham số, lệnh không có tham số mà bị gõ thêm (ví dụ `DEQ 1`).
- **Kết thúc:** khi hết dữ liệu stdin (Backend đóng pipe), Engine giải phóng bộ nhớ và thoát với mã 0.
- **Không kiểm tra trùng id:** kiểm tra trùng tốn O(n), nên **Backend chịu trách nhiệm không `ENQ` trùng**. Khi khôi phục hàng, luôn gửi `CLEAR` trước rồi mới nạp lại.
- **Định dạng dòng:** Engine chấp nhận cả cuối dòng kiểu Windows (`\r\n`), khoảng trắng thừa ở đầu và cuối dòng, và dòng cuối không có ký tự xuống dòng.

### Ví dụ một phiên làm việc

```
(Engine)  READY
ENQ 101   ->  OK
ENQ 102   ->  OK
ENQ 103   ->  OK
PEEK      ->  ID 101
DEQ       ->  ID 101
SIZE      ->  SIZE 2
DEQ       ->  ID 102
DEQ       ->  ID 103
DEQ       ->  EMPTY
ENQ abc   ->  ERR BAD_ID
HELLO     ->  ERR BAD_COMMAND
```

Thử nhanh trên Linux/macOS:

```
printf 'ENQ 5\nENQ 7\nDEQ\nDEQ\nDEQ\n' | ./queue_engine
```

Kết quả: `READY`, `OK`, `OK`, `ID 5`, `ID 7`, `EMPTY`.

## Hướng dẫn cho Backend (TV6)

1. **Khởi động Engine một lần** cùng lúc với server và giữ kết nối stdin/stdout suốt thời gian chạy. Không gọi chương trình mới cho mỗi request, vì mỗi tiến trình mới có hàng rỗng nên sẽ mất toàn bộ hàng đợi.
2. **Đợi dòng `READY`** rồi mới gửi lệnh.
3. **Mỗi thời điểm chỉ một lệnh đang chờ phản hồi.** Gửi một lệnh, đọc đúng một dòng phản hồi, rồi mới gửi lệnh tiếp theo (cần hàng đợi lệnh hoặc khóa ở Backend để các request đồng thời không làm lẫn phản hồi).
4. **Kiểm tra mọi phản hồi** theo bảng giao thức; gặp phản hồi lạ hoặc hết thời gian chờ thì coi là Engine có sự cố.
5. **Khôi phục khi Engine dừng:** khởi động lại Engine, gửi `CLEAR`, rồi `ENQ` lần lượt các đơn trạng thái PENDING theo thứ tự `created_at` tăng dần, sau đó `id` tăng dần (do TV3 cung cấp).
6. **Đơn mới:** sau khi lưu đơn PENDING vào Database, gửi `ENQ <id>` ngay. Đơn mới có `created_at` lớn nhất nên thêm vào cuối hàng là đúng thứ tự.
7. **Xử lý đơn tiếp theo:** gửi `DEQ`, rồi cập nhật Database có điều kiện (`status = 'PENDING'`). Nếu không có dòng nào bị thay đổi (đơn đã hủy hoặc đã xử lý) thì bỏ qua và `DEQ` tiếp cho đến khi duyệt được một đơn hoặc gặp `EMPTY`.

## Cấu trúc dữ liệu

- `Node`: chứa `orderId` và con trỏ `next`.
- `Queue`: quản lý `front` (đầu hàng), `rear` (cuối hàng) và `size`. Hàng rỗng khi cả `front` và `rear` đều là `nullptr`.
- `QueueEngine`: nhận một dòng lệnh, gọi `Queue`, trả về một dòng phản hồi.

| Thao tác | Ý nghĩa | Độ phức tạp |
|---|---|---|
| `enqueue` | Thêm Node vào rear | O(1) |
| `dequeue` | Lấy Node ở front | O(1) |
| `peek` | Xem phần tử ở front | O(1) |
| `isEmpty` / `getSize` | Kiểm tra rỗng / lấy số phần tử | O(1) |
| `clear` | Giải phóng toàn bộ Node | O(n) |

Danh sách liên kết quản lý sẵn `front` và `rear` nên thêm cuối và lấy đầu đều O(1). Dùng mảng thì lấy phần tử ở đầu phải dời cả mảng, tốn O(n).
