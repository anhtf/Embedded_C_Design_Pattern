# Day 11: Concurrency Pattern Overview
> Last updated: 2026-04-10

## Overview
Khi chuyển từ lập trình Bare-metal (Single-threaded) sang RTOS (Multi-threaded) hoặc Multicore, kiến trúc hệ thống thay đổi hoàn toàn. Nhiều Tasks sẽ tranh giành thời gian thực thi của CPU. Concurrency Pattern tập trung vào việc nhận diện, đóng gói và quản lý các Tài nguyên dùng chung (Shared Resources) để ngăn chặn các lỗi tương tranh dữ liệu.

## Core Concepts
- **Multitasking:** Khả năng hệ điều hành chuyển đổi ngữ cảnh (Context Switch) giữa các Tasks với tốc độ cực cao, tạo cảm giác chúng chạy song song.
- **Shared Resource:** Bất kỳ vùng nhớ, ngoại vi, hoặc cấu trúc dữ liệu nào được truy xuất bởi lớn hơn hoặc bằng 2 Tasks, hoặc 1 Task và 1 Interrupt.
- **Critical Section (Vùng găng):** Đoạn mã thao tác trực tiếp lên Shared Resource.
- **Race Condition:** Sự cố xảy ra khi thứ tự thực thi của các Tasks tại vùng găng không được kiểm soát, dẫn đến kết quả dữ liệu bị sai lệch (Read-Modify-Write overlap).

## Implementation Standards
1. **Encapsulation of Shared State:** Tuyệt đối không phơi bày tài nguyên dùng chung dưới dạng biến toàn cục public. Phải đóng gói nó vào struct và chỉ cho phép thao tác thông qua API.
2. **Explicit Critical Sections:** Viết code một cách minh bạch (chia nhỏ các thao tác gán/cộng) để dễ dàng xác định vị trí cần đặt các cơ chế khóa (Locks) trong tương lai.

## Pitfalls to Avoid
- **The `volatile` Fallacy:** Nhầm lẫn rằng từ khóa `volatile` đảm bảo tính toàn vẹn dữ liệu trong môi trường đa luồng. `volatile` chỉ chống compiler optimization, không cung cấp tính nguyên tử (atomicity).
- **Hidden Dependencies:** Các hàm thư viện chuẩn C (như `strtok`, `rand`, `printf` trong một số compiler) chứa trạng thái tĩnh nội bộ (internal static state) và không an toàn cho luồng (Non-reentrant / Thread-unsafe). Việc gọi chúng từ nhiều Task sẽ gây crash.

## File Structure
- `shared_counter.h`: API để truy cập tài nguyên (Đã được đóng gói).
- `shared_counter.c`: Triển khai logic, phơi bày rõ vấn đề tại các vùng găng.
- `main.c`: Mô phỏng khởi tạo nhiều RTOS Tasks cùng chọc vào API.
