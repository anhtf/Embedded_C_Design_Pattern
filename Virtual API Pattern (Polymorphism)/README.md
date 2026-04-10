# Day 8: Virtual API Pattern (Polymorphism)
> Last updated: 2026-04-10

## Overview
Virtual API Pattern là cách ngôn ngữ C triển khai tính Đa hình (Polymorphism) của hướng đối tượng. Nó cho phép tầng Ứng dụng giao tiếp với nhiều loại phần cứng/module khác nhau thông qua một giao diện duy nhất (API chung), giúp loại bỏ hoàn toàn các khối `switch-case` rườm rà và giúp hệ thống tuân thủ chặt chẽ nguyên tắc Open-Closed (dễ mở rộng, hạn chế sửa đổi file lõi).

## Core Concepts
- **VTable (Virtual Method Table):** Một `struct` chỉ chứa các con trỏ hàm (function pointers), đóng vai trò như một Interface.
- **Base Object:** Cấu trúc cha chứa một con trỏ trỏ tới VTable.
- **Implementation:** Mỗi lớp con cụ thể (Derived class) sẽ định nghĩa các hàm vật lý của riêng nó và liên kết chúng vào một VTable của riêng lớp đó.
- **API Wrapper:** Các hàm công khai ở tầng giao diện không thao tác trực tiếp, mà sẽ kiểm tra an toàn và ủy quyền (delegate) việc thực thi cho hàm được trỏ bởi VTable.

## Implementation Standards (Linux Kernel Style)
1. **Constant VTable:** Bảng `ops` (VTable) của phần cứng cụ thể phải luôn được khai báo là `static const` để trình biên dịch lưu nó vào vùng nhớ ROM/Flash, tối ưu hóa RAM. Mọi instance của cùng một lớp sẽ chia sẻ chung bảng này.
2. **Defensive Dereferencing:** Trong các API Wrapper, luôn phải kiểm tra chuỗi `if (!dev || !dev->ops || !dev->ops->method)` trước khi thực thi để tránh lỗi Hard Fault do giải tham chiếu con trỏ NULL.
3. **Downcasting:** Bên trong các hàm triển khai cụ thể (như `uart_write_impl`), an toàn sử dụng ép kiểu ngược (Downcasting) từ `struct base*` sang `struct derived*` để truy xuất thuộc tính riêng.

## Pitfalls to Avoid
- **Performance Hit:** Gọi hàm gián tiếp qua con trỏ (indirect call) tốn nhiều chu kỳ CPU hơn và vô hiệu hóa các phép tối ưu hóa inlining. Hạn chế sử dụng Virtual API trong các ngắt (ISR) đòi hỏi tốc độ xử lý tính bằng micro-giây.
- **Complexity:** Việc lạm dụng đa hình cho các module đơn giản sẽ làm mã nguồn phân mảnh, khó debug và theo dõi luồng thực thi (execution flow) khi đọc code tĩnh.

## File Structure
- `comm.h / comm.c`: Định nghĩa VTable Interface, Base struct và các Wrapper APIs.
- `uart_device.h / .c`: Triển khai phần cứng UART cụ thể, kế thừa Base struct và gán VTable.
- `main.c`: Minh họa việc truyền đối tượng UART vào hàm chung nhận đối tượng Base.
