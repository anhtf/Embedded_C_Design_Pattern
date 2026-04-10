# Day 4: Singleton Pattern

## Overview
Singleton Pattern đảm bảo một module hoặc tài nguyên phần cứng chỉ có duy nhất một thực thể (instance) tồn tại trong toàn bộ hệ thống. Trong Embedded, mẫu này cực kỳ hữu ích để quản lý các tài nguyên độc nhất như Watchdog Timer, System Clock, hoặc Power Management.

## Core Concepts
- **Unique Instance:** Sử dụng một biến `static` ẩn bên trong file `.c` để lưu trữ dữ liệu.
- **Global Access Point:** Thay vì dùng hàm `create`, Singleton cung cấp hàm `get_instance()` để trả về địa chỉ của thực thể duy nhất đó.
- **State Protection:** Sử dụng các cờ nội bộ (ví dụ `is_initialized`) để ngăn chặn việc cấu hình lại tài nguyên khi nó đang hoạt động.

## Implementation Standards
1. **Opaque Singleton:** Kết hợp với Opaque pointer để người dùng không thể can thiệp vào struct nội bộ.
2. **Identity Verification:** Trong các hàm API, ngoài việc kiểm tra `NULL`, cần kiểm tra xem con trỏ truyền vào có thực sự là địa chỉ của Singleton instance hay không (`inst == &singleton`).
3. **Static Allocation:** Instance được cấp phát tĩnh trong phân vùng `.data` hoặc `.bss`, không sử dụng Heap.

## Pitfalls to Avoid
- **Overuse:** Đừng biến mọi thứ thành Singleton. Chỉ sử dụng cho các tài nguyên thực sự độc nhất về mặt vật lý.
- **Testing Bottleneck:** Singleton gây khó khăn cho Unit Test do lưu giữ trạng thái toàn cục. Hãy cung cấp hàm reset trạng thái cho mục đích testing nếu cần.
- **Initialization Order:** Đảm bảo Singleton được khởi tạo trong luồng đơn (Single-thread) trước khi RTOS scheduler bắt đầu để tránh Race Condition.

## File Structure
- `watchdog.h`: Interface định nghĩa các thao tác với tài nguyên độc nhất.
- `watchdog.c`: Định nghĩa struct, instance tĩnh và logic bảo vệ trạng thái.
- `main.c`: Minh họa việc truy cập Singleton từ ứng dụng.