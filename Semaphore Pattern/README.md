# Day 13: Semaphore Pattern (Signaling & Deferred Processing)
> Last updated: 2026-04-10

## Overview
Semaphore (Đèn tín hiệu) là công cụ đồng bộ hóa nền tảng của RTOS. Khác với Spinlock bắt CPU phải chờ bận, Semaphore cho phép Task đi vào trạng thái Block/Sleep, nhường 100% CPU cho các Task khác. Semaphore là kiến trúc cốt lõi để triển khai mô hình "Xử lý trì hoãn" (Deferred Processing / Top-half & Bottom-half), giúp giảm thiểu thời gian chiếm dụng CPU của các trình phục vụ ngắt (ISR).

## Core Concepts
- **Top-half (ISR):** Chỉ đọc phần cứng và thực hiện `give` tín hiệu Semaphore một cách nhanh nhất có thể.
- **Bottom-half (Task):** Luôn chặn (block) tại hàm `take` Semaphore. Khi được ISR đánh thức, nó thực hiện các công việc tính toán nặng nhọc.
- **Binary vs Counting Semaphore:** Binary giới hạn đếm tối đa là 1, dùng để báo hiệu trạng thái (ví dụ: có người vừa bấm nút). Counting cho phép đếm lên N, dùng để quản lý một nhóm tài nguyên (ví dụ: đệm dữ liệu có 5 slot, số lần ngắt UART đến liên tiếp).

## Implementation Standards
1. **OS Abstraction Layer (OSAL):** Không nên gọi trực tiếp API của RTOS (như `xSemaphoreGive`) vào sâu trong logic ứng dụng. Hãy bọc chúng qua các struct và API giao diện để dự án không bị khóa chặt (vendor lock-in) vào một loại RTOS cụ thể.
2. **ISR Specific APIs:** Bắt buộc phân tách hàm `give` từ Task và hàm `give` từ ISR. API từ ISR có cơ chế quản lý thanh ghi trạng thái ngắt và tránh việc gọi trực tiếp vào bộ lập lịch.

## Pitfalls to Avoid
- **Blocking inside ISR:** Tuyệt đối không bao giờ gọi hàm `take` (vốn có khả năng đưa tiến trình vào trạng thái Sleep) bên trong ngắt. Gây ra Hard Fault/Kernel Panic ngay lập tức.
- **Priority Inversion (Cảnh báo):** Không nên sử dụng Semaphore để bảo vệ Vùng găng chứa Dữ liệu dùng chung (như biến đếm, mảng). Semaphore không có cơ chế kế thừa ưu tiên (Priority Inheritance), có thể dẫn đến hiện tượng đảo ngược thứ tự ưu tiên làm treo hệ thống. Để bảo vệ dữ liệu, dùng Mutex.
- **Lost Wakeups:** Nếu sử dụng Binary Semaphore cho các sự kiện xảy ra nhanh hơn tốc độ Task có thể xử lý, các sự kiện dồn dập sẽ bị gộp làm một, gây mất dữ liệu.

## File Structure
- `os_semaphore.h`: API định nghĩa lớp trừu tượng cho Semaphore.
- `os_semaphore.c`: Vỏ bọc ánh xạ xuống các API của RTOS thực tế.
- `main.c`: Mô hình Producer-Consumer giữa ngắt phần cứng (ISR) và Task xử lý dữ liệu.
