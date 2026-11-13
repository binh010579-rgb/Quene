# Báo cáo Đo hiệu năng (Benchmark) – Cấu trúc dữ liệu Queue (TV1)

## 1. Mục tiêu
Đo lường và so sánh thời gian thực thi thực tế giữa hai cách cài đặt Hàng đợi (Queue) cho hệ thống BeautyStore:
- **Linked List Queue (Danh sách liên kết đơn - TV4):** Quản lý hai con trỏ `front` và `rear`[cite: 18, 20].
- **Array Queue (Mảng tuyến tính):** Thêm ở cuối mảng, lấy ở đầu mảng (index `0`) và dịch chuyển toàn bộ các phần tử phía sau lên trước[cite: 18, 20].

## 2. Môi trường & Kịch bản đo
- **Ngôn ngữ & Trình biên dịch:** C++17 (`g++ -std=c++17 -O2`)[cite: 18]
- **Kịch bản đo:** Thực hiện liên tiếp `n` thao tác `enqueue` (nạp đơn hàng) và `n` thao tác `dequeue` (xử lý hết đơn hàng) tại 3 quy mô dữ liệu: `1.000`, `10.000` và `100.000` đơn hàng[cite: 18, 24].

## 3. Bảng kết quả thực nghiệm

| Số lượng đơn hàng ($n$) | Linked List Queue (ms) | Array Queue (ms) | Mức độ chênh lệch |
| :--- | :---: | :---: | :---: |
| **1.000** | `0.0486 ms`[cite: 24] | `0.0177 ms`[cite: 24] | Array nhanh hơn ~2.7 lần (do kích thước nhỏ, tận dụng CPU Cache) |
| **10.000** | `0.9422 ms`[cite: 24] | `48.9983 ms`[cite: 24] | **Linked List nhanh hơn 52.0 lần**[cite: 24] |
| **100.000** | `7.3379 ms`[cite: 24] | `5582.4145 ms`[cite: 24] | **Linked List nhanh hơn 760.8 lần**[cite: 24] |

## 4. Phân tích độ phức tạp & Nhận xét

1. **Tại mốc dữ liệu nhỏ ($n = 1.000$):**
   - `ArrayQueue` nhỉnh hơn một chút (`0.0177 ms` vs `0.0486 ms`) vì toàn bộ mảng 1.000 số nguyên nằm liền kề nhau trên RAM (tối ưu CPU Cache) và không tốn chi phí cấp phát con trỏ `new`/`delete` từng Node[cite: 18, 24].

2. **Khi dữ liệu tăng lớn ($n = 10.000 \rightarrow 100.000$):**
   - **Với Linked List Queue:** Thao tác `enqueue` tại `rear` và `dequeue` tại `front` đều đạt độ phức tạp **$O(1)$**[cite: 18, 20]. Khi số đơn tăng gấp 10 lần (từ $10.000$ lên $100.000$), thời gian chạy chỉ tăng tuyến tính ~$7.8$ lần (từ `0.9422 ms` lên `7.3379 ms`), tổng độ phức tạp là **$O(n)$**[cite: 18, 24].
   - **Với Array Queue:** Mỗi lần lấy đơn ở đầu (`index 0`), chương trình phải dùng vòng lặp dịch chuyển $n-1$ phần tử còn lại lên trước, tốn **$O(n)$**[cite: 18, 20]. Khi xử lý $n$ đơn hàng liên tiếp, tổng chi phí lên tới **$O(n^2)$** (khi số đơn tăng 10 lần, thời gian tăng vọt gấp **114 lần**, từ `48.9983 ms` lên tới `5582.4145 ms` — hơn 5.5 giây)[cite: 18, 24].

**Kết luận:** Kết quả thực nghiệm chứng minh việc lựa chọn **Danh sách liên kết đơn** với hai con trỏ `front` và `rear` cho `Queue Engine` của hệ thống BeautyStore là hoàn toàn tối ưu, giúp hệ thống xử lý hàng trăm nghìn đơn hàng trong vài mili-giây mà không gặp tình trạng nghẽn cổ chai[cite: 18, 24].
