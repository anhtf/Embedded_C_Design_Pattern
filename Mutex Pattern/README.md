# Day 14: Mutex Pattern (Resource Protection)

## Overview
Mutex (Mutual Exclusion) là cơ chế đồng bộ hóa bắt buộc để bảo vệ các Tài nguyên dùng chung (Shared Resources) trong môi trường Đa luồng (RTOS). Khác với Semaphore dùng để báo hiệu, Mutex có "Tính sở hữu" (Ownership) - chỉ Task giữ khóa mới được phép nhả khóa. Tính chất này giúp RTOS triển khai các thuật toán chống lại hiện tượng Đảo ngược quyền ưu tiên (Priority Inversion).

## Core Concepts
- **Critical Section Protection:** Đoạn code thao tác với biến toàn cục, mảng, hoặc ngoại vi (I2C, SPI) phải được bọc trong bộ đôi `lock` và `unlock`.
- **Ownership:** Task A khóa thì Task A phải mở. Task B không thể mở khóa của Task A.
- **Priority Inheritance:** Khi một Task ưu tiên cao bị chặn bởi một Mutex do Task ưu tiên thấp nắm giữ, RTOS sẽ tạm thời nâng quyền ưu tiên của Task thấp lên ngang bằng Task cao để nó có thể hoàn thành vùng găng nhanh nhất, ngăn không cho các Task trung bình cướp CPU.

## Implementation Standards
1. **OS Abstraction Layer (OSAL):** Bọc các hàm `CreateMutex`, `TakeMutex`, `GiveMutex` của RTOS vào các struct và API độc lập để dự án không phụ thuộc phần mềm bên thứ ba.
2. **Deterministic Locking Order:** Trong hệ thống lớn yêu cầu nhiều khóa, các Task phải luôn lấy khóa theo cùng một trật tự cố định (ví dụ: luôn lấy `Lock_A` trước `Lock_B`) để tránh Deadlock vòng tròn.
3. **Structured Cleanup:** Sử dụng mô hình `goto cleanup` để đảm bảo mọi nhánh lỗi (Error paths) đều nhả khóa trước khi `return`.

## Pitfalls to Avoid
- **ISR Blocking:** Tuyệt đối không gọi hàm lấy Mutex bên trong Interrupt Service Routine (ISR). Việc chặn một ISR sẽ làm sập toàn bộ bộ lập lịch của MCU.
- **Self-Deadlock:** Task lấy một Mutex hai lần liên tiếp mà không nhả ở giữa. (Có thể khắc phục bằng cách cấu hình Recursive Mutex trong RTOS, tuy nhiên thiết kế sạch là không nên gọi lặp).
- **Long Lock Hold Times:** Giữ Mutex quá lâu sẽ làm giảm tính đáp ứng thời gian thực (Real-time response) của hệ thống. Vùng găng phải cực kỳ ngắn gọn.

## File Structure
- `os_mutex.h`: API lớp trừu tượng cho Mutex.
- `os_mutex.c`: Vỏ bọc ánh xạ tới các hàm Native RTOS.
- `main.c`: Mô phỏng cạnh tranh tài nguyên I2C giữa hai Task khác mức ưu tiên.