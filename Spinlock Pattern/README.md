# Day 12: Spinlock Pattern

## Overview
Spinlock là một cơ chế đồng bộ hóa (Synchronization) cấp thấp nhất, được thiết kế để bảo vệ các vùng găng (Critical Sections) có thời gian thực thi cực ngắn. Khác với Mutex (đưa Task vào trạng thái ngủ khi bị chặn), Spinlock sử dụng kỹ thuật "Chờ bận" (Busy-waiting). Nó giữ CPU trong một vòng lặp liên tục để thăm dò trạng thái của khóa cho đến khi khóa được mở.

## Core Concepts
- **Atomic Operations:** Việc khóa và mở khóa phải được thực hiện bằng các tập lệnh phần cứng không thể bị chia cắt (nguyên tử) như `LDREX/STREX` hoặc `Compare-And-Swap`.
- **Busy-Waiting:** Ưu điểm là loại bỏ được độ trễ chuyển ngữ cảnh (Context switch overhead) của RTOS. Nhược điểm là đốt cháy chu kỳ CPU vô ích và tốn năng lượng.
- **Interrupt Context:** Spinlock là loại khóa duy nhất an toàn để sử dụng bên trong các trình phục vụ ngắt (ISR), nơi các cơ chế blocking/sleeping bị nghiêm cấm.

## Implementation Standards
1. **Volatile Flag:** Biến cờ khóa (lock flag) bắt buộc phải có từ khóa `volatile` để ngăn trình biên dịch tối ưu hóa vòng lặp thăm dò thành một vòng lặp vô tận chết (Dead code elimination).
2. **Compiler Intrinsics:** Không được tự viết các phép gán C thông thường để làm khóa. Phải sử dụng các built-in primitives của Compiler (như họ `__sync_` hoặc `__atomic_` trong GCC).

## Pitfalls to Avoid
- **Long Critical Sections:** TUYỆT ĐỐI KHÔNG chứa các thao tác tốn thời gian (delay, tính toán nặng, copy mảng lớn) bên trong vùng bảo vệ của Spinlock. Điều này làm sập hiệu năng của toàn bộ hệ thống đa lõi.
- **Deadlock with Interrupts (Single-core):** Nếu một Task lấy Spinlock rồi bị ngắt bởi một ISR, và ISR đó cũng cố lấy chính Spinlock đó, hệ thống sẽ Deadlock ngay lập tức. Trên MCU đơn lõi, việc lấy Spinlock phải luôn đi kèm với thao tác Disable Interrupts (tương đương `spin_lock_irqsave` trong Linux Kernel).

## File Structure
- `spinlock.h`: Định nghĩa `struct spinlock` và APIs.
- `spinlock.c`: Triển khai các thao tác khóa nguyên tử dựa trên GCC built-ins.
- `main.c`: Minh họa việc áp dụng Spinlock để bảo vệ dữ liệu dùng chung giữa Task và ISR.