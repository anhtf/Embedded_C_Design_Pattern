# Day 1: Introduction to Embedded C Design Patterns

## Overview
Ngôn ngữ C thuần túy mang tính hướng thủ tục, dễ dẫn đến mã nguồn khó bảo trì (spaghetti code), kết dính chặt chẽ (tight coupling) và lạm dụng biến toàn cục (global state manipulation) trong các dự án Embedded quy mô lớn. 
Khóa học/Repository này nhằm mục đích triển khai các mẫu thiết kế (Design Patterns) vào C, áp dụng các khái niệm OOP (Encapsulation, Polymorphism, Inheritance) để xây dựng phần mềm nhúng an toàn, có khả năng mở rộng và dễ dàng Unit Test.

## Key Principles
1. **Encapsulation:** Gom nhóm dữ liệu và hành vi liên quan vào cấu trúc `struct`.
2. **Information Hiding:** Ẩn các chi tiết triển khai bên trong file `.c`, chỉ cung cấp API thông qua file `.h` (Opaque pointer).
3. **Hardware Abstraction:** Tách biệt Application Logic khỏi Hardware Dependencies (Decoupling) bằng cách sử dụng Interfaces (Function Pointers).
4. **Static Allocation:** Hạn chế tối đa sử dụng Heap (`malloc`/`free`) để tuân thủ tiêu chuẩn an toàn trong hệ thống nhúng (MISRA-C).

## Coding Standards
Repository này tuân thủ nghiêm ngặt **Linux Kernel Coding Style**:
- Naming convention: `snake_case`.
- Struct declaration: Sử dụng `struct original_name`, tuyệt đối không dùng `typedef` cho struct.
- Brace placement: Chuẩn K&R (Dấu ngoặc nhọn mở ở cùng dòng với control statements, nhưng ở dòng mới đối với function definitions).
- Documentation: Self-documenting code. Hạn chế tối đa inline comments.

## Pitfalls to Avoid
- **Over-engineering:** Tránh áp dụng pattern phức tạp cho các tác vụ trivial, gây lãng phí tài nguyên MCU.
- **Indirection Overhead:** Lưu ý chi phí chu kỳ CPU khi sử dụng quá nhiều VTable/Function pointer trong các ngắt (ISR) hoặc các hàm yêu cầu tính thời gian thực khắt khe.