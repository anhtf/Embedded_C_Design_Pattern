# Day 6: Callback Pattern
> Last updated: 2026-04-10
![alt text](<Screenshot From 2026-04-10 15-15-26.png>)
## Overview
Callback Pattern là trái tim của kiến trúc hướng sự kiện (Event-Driven Architecture) trong hệ thống nhúng. Nó đảo ngược sự phụ thuộc (Dependency Inversion), cho phép các tầng cấp thấp (như Hardware Drivers hoặc ISRs) thông báo ngược lên tầng Ứng dụng khi có sự kiện xảy ra mà không cần phải `include` trực tiếp code của ứng dụng.

## Core Concepts
- **Function Pointers:** Thay vì gọi hàm trực tiếp, Driver nhận và lưu trữ địa chỉ của một hàm thông qua con trỏ.
- **Hollywood Principle:** "Don't call us, we'll call you." Tầng ứng dụng đăng ký (register) bản thân nó với Driver, và Driver sẽ kích hoạt (invoke) khi cần.
- **The Context Pointer (`void *`):** Một callback thiết kế tốt luôn truyền kèm một con trỏ `void *context`. Con trỏ này chứa dữ liệu/trạng thái của Application (Object Instance), giúp hàm callback biết được đối tượng nào cần cập nhật trạng thái, loại bỏ hoàn toàn việc sử dụng biến toàn cục.

## Implementation Standards
1. **Decoupling:** File header của Driver (`button.h`) tuyệt đối không phụ thuộc vào bất kỳ module chức năng nào của hệ thống. 
2. **Defensive Invocation:** Driver phải kiểm tra con trỏ hàm (`if (cb != NULL)`) trước khi gọi (invoke) để tránh Hard Fault/Segmentation Fault.

## Pitfalls to Avoid
- **ISR Context Violation:** Callbacks thường được gọi trực tiếp từ Interrupt Service Routine (ISR). 
Lập trình viên Application phải ý thức được điều này và viết callback cực kỳ ngắn, không blocking, không `printf`/`malloc`.
- **Race Conditions:** Các biến trạng thái được callback thay đổi phải được đánh dấu là `volatile` và bảo vệ bằng cơ chế đồng bộ (Critical Sections/Atomics) khi truy cập từ vòng lặp chính.
- **Dangling Callbacks:** Nếu đối tượng Application bị destroy, nó phải unregister callback (set về `NULL`) ở Driver, nếu không Driver sẽ gọi vào một vùng nhớ đã bị giải phóng.

## File Structure
- `button.h`: Khai báo chữ ký Callback (Function pointer type) và API đăng ký.
- `button.c`: Quản lý con trỏ hàm và thực thi callback trong ISR.
- `main.c`: Tầng ứng dụng triển khai logic thực tế và đăng ký địa chỉ hàm xuống Driver.
