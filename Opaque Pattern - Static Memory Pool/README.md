# Day 3: Opaque Pattern & Static Memory Pool

## Overview
Opaque Pattern nâng cấp tính đóng gói (Encapsulation) lên mức độ Binary/Header. Nó ngăn chặn triệt để việc tầng Application truy cập trái phép vào dữ liệu bên trong của một Object bằng cách sử dụng Forward Declaration (Con trỏ mờ).
Kết hợp với Static Memory Pool (Bể nhớ tĩnh), pattern này cung cấp một cơ chế khởi tạo đối tượng động an toàn, loại bỏ hoàn toàn sự phụ thuộc vào Heap (`malloc`/`free`), đáp ứng các tiêu chuẩn khắt khe trong hệ thống nhúng (MISRA-C).

## Core Concepts
- **Forward Declaration:** Khai báo sự tồn tại của `struct` ở file `.h` nhưng giấu định nghĩa chi tiết vào file `.c`.
- **Incomplete Type:** Trình biên dịch sẽ báo lỗi nếu Application cố gắng truy cập trực tiếp vào properties của struct hoặc cố gắng cấp phát tĩnh (khai báo biến) thay vì dùng con trỏ.
- **Static Memory Pool:** Một mảng tĩnh được khai báo `static` ẩn bên trong file `.c`. Các hàm API sẽ quản lý việc cấp phát và thu hồi các block trong mảng này.

## Defensive Programming Standards
1. **Deterministic Memory:** Mọi việc cấp phát RAM đều được định hình ngay tại thời điểm Compile-time. Không có nguy cơ Memory Leak hay Fragmentation (phân mảnh).
2. **Boundary Checks:** Hàm `sensor_create` tự động trả về `NULL` nếu số lượng đối tượng vượt quá giới hạn cấu hình phần cứng `MAX_SENSORS`. Public APIs luôn phải check Null.

## Implementation Notes
- Cẩn trọng với **Dangling Pointers**: Con trỏ ở tầng ứng dụng không tự động chuyển thành `NULL` sau khi gọi `destroy`. Người sử dụng API cần chủ động gán `NULL` sau khi thu hồi tài nguyên.
- **Concurrency Warning:** Implementation hiện tại không thread-safe. Nếu sử dụng trong RTOS với đa luồng (Multi-threading), các tác vụ `create` và `destroy` yêu cầu cơ chế bảo vệ vùng găng (Critical Section) như Mutex/Spinlock.

## File Structure
- `sensor.h`: Forward declaration và public API (Interface).
- `sensor.c`: Struct definition, Pool management, Hardware logic.
- `main.c`: Application logic (chỉ làm việc với Opaque Pointers).