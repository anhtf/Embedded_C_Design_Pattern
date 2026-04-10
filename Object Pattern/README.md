# Day 2: Object Pattern (Encapsulation)
> Last updated: 2026-04-10

## Overview
Object Pattern là bước đầu tiên để mang tư duy hướng đối tượng (OOP) vào ngôn ngữ C. Nó giải quyết triệt để vấn đề "Global Variable Hell" (Lạm dụng biến toàn cục) bằng cách gom dữ liệu và trạng thái của một thực thể vật lý/logic vào một khối `struct` duy nhất.

## Core Concepts
- **Class Implementation:** Một cấu trúc `struct` đóng vai trò như một bản thiết kế (Class).
- **Instance Generation:** Các biến thuộc kiểu `struct` đó chính là các thực thể (Objects). Mỗi instance sở hữu một vùng nhớ độc lập chứa trạng thái riêng biệt.
- **Methods (Phương thức):** Được mô phỏng bằng các hàm C thông thường. Tiền tố của hàm mang tên của `struct` để tạo namespace.
- **The "this" Pointer:** Thuộc tính bắt buộc của mọi method là tham số đầu tiên phải là con trỏ trỏ tới instance đang được thao tác (thường đặt tên là `dev`, `inst`, hoặc `me`).

## Defensive Programming Standards
1. **Null Check:** Mọi public API nhận vào con trỏ instance đều phải kiểm tra tính hợp lệ của con trỏ (`if (!dev)`) trước khi giải tham chiếu (dereference) để tránh Hard Fault.
2. **Pointers Only:** Luôn truyền instance vào hàm dưới dạng con trỏ, tuyệt đối không truyền tham trị (pass by value) để tránh lãng phí Stack và đảm bảo trạng thái được cập nhật trực tiếp vào vùng nhớ gốc.

## File Structure
- `led.h`: Định nghĩa cấu trúc dữ liệu public và các public APIs.
- `led.c`: Triển khai logic điều khiển phần cứng.
- `main.c`: Cấp phát tĩnh (Static allocation) các instances và điều phối luồng thực thi.
