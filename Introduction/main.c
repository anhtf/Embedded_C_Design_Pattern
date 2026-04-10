/*
Ngôn ngữ C mang bản chất là hướng thủ tục (Procedural). Trong giai đoạn đầu phát triển firmware, cách tiếp cận này giúp việc truy xuất thanh ghi (register) rất nhanh và trực quan. Tuy nhiên, khi dự án phình to với hàng tá module ngoại vi, giao thức giao tiếp và hệ điều hành thời gian thực (RTOS), mô hình hướng thủ tục sẽ bộc lộ những điểm yếu chí mạng.

1. Phân tích vấn đề của C truyền thống trong thực tế
Spaghetti Code & Độ kết dính cao (Tight Coupling): Tầng ứng dụng (Application Layer) gọi trực tiếp các hàm của tầng phần cứng (Hardware Layer/HAL). Nếu chip đổi từ STM32 sang NXP, bạn phải đập bỏ và viết lại toàn bộ tầng ứng dụng.

Biến toàn cục (Global Variable Hell): Việc lạm dụng biến toàn cục để lưu trạng thái hệ thống dẫn đến xung đột dữ liệu, đặc biệt khi có ngắt (ISR) xảy ra hoặc khi chạy đa luồng. Việc debug lúc này giống như mò kim đáy bể vì bất kỳ hàm nào cũng có thể thay đổi dữ liệu.

Khó viết Unit Test: Không thể tách rời các module để test độc lập (mocking) vì chúng dính chặt vào nhau.

2. Giải pháp: Design Pattern trong C
Design Pattern không phải là thư viện, nó là kiến trúc. Mục tiêu của khóa học này là vay mượn các đặc tính của Lập trình hướng đối tượng (OOP) — bao gồm Đóng gói (Encapsulation), Đa hình (Polymorphism) và Kế thừa (Inheritance) — và áp dụng chúng vào C.

Ba quy tắc cốt lõi bạn sẽ học trong khóa này:

Data Encapsulation: Dữ liệu và trạng thái phải được gói gọn trong các struct. Không có biến toàn cục trôi nổi.

Information Hiding: Module bên ngoài chỉ được giao tiếp qua các hàm API (Interface), tuyệt đối không được truy cập thẳng vào cấu trúc dữ liệu bên trong của module khác.

Hardware Abstraction: Mọi giao tiếp với phần cứng vật lý phải được trừu tượng hóa qua các con trỏ hàm (Function Pointers / VTable).

3. Các cạm bẫy thực tế cần lưu ý (Pitfalls & Warnings)
Over-engineering (Làm quá vấn đề): Không phải cái gì cũng cần Design Pattern. Viết một đoạn code nháy LED đơn giản trên MCU 8-bit mà dùng tới Virtual API hay Factory Pattern là sự lãng phí tài nguyên RAM/ROM và chu kỳ CPU.

Performance Overhead: Áp dụng OOP vào C thường đòi hỏi sử dụng con trỏ (pointers) và con trỏ hàm. Việc này làm tăng số lượng lệnh assembly khi gọi hàm (dereferencing overhead) và có thể làm vô hiệu hóa khả năng tối ưu hóa (inline function) của trình biên dịch (GCC/Clang). Cần cân nhắc giữa "Kiến trúc sạch" và "Hiệu năng".

Memory Constraints: Một số pattern yêu cầu cấp phát động. Trong embedded (đặc biệt là Automotive/Aerospace), malloc là điều cấm kỵ. Chúng ta sẽ luôn ưu tiên dùng Static Memory Allocation (Cấp phát tĩnh).
*/