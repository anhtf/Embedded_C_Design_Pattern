# Day 10: Return Value Pattern
> Last updated: 2026-04-10

## Overview
Do ngôn ngữ C không hỗ trợ cơ chế ngoại lệ (Exceptions), việc xử lý lỗi thường rơi vào các thiết kế tồi như trộn lẫn mã lỗi vào dữ liệu trả về (In-band error) hoặc dùng biến trạng thái toàn cục. Return Value Pattern chuẩn hóa luồng thực thi: Giá trị trả về của hàm BẮT BUỘC dùng để báo cáo mã trạng thái (Error Codes), trong khi dữ liệu đầu ra được truyền qua các con trỏ đối số (Out-parameters).

## Core Concepts
- **Status Code:** Hàm luôn trả về `int` (hoặc `enum`). Quy ước chuẩn của Linux Kernel: `0` là thành công (OK), các số âm (ví dụ `-1`, `-2`) là các mã lỗi cụ thể.
- **Out-Parameters:** Dữ liệu thực tế được ghi vào địa chỉ bộ nhớ do tầng ứng dụng cung cấp thông qua con trỏ.
- **Cleanup Pattern:** Sử dụng `goto` một cách có cấu trúc để dọn dẹp tài nguyên (Free memory, Disable hardware) khi xảy ra lỗi chuỗi.

## Implementation Standards
1. **Defensive Validation:** Mọi out-parameter phải được kiểm tra `NULL` trước khi ghi dữ liệu để tránh Segmentation Fault / Hard Fault.
2. **Standardized Error Definitions:** Các mã lỗi (`SYS_ERR_INVAL`, `SYS_ERR_TIMEOUT`) phải được gom vào một file header chung của hệ thống để thống nhất toàn bộ các modules.
3. **LIFO Cleanup:** Trong mẫu `goto cleanup`, thứ tự giải phóng tài nguyên tại các nhãn (labels) phải ngược hoàn toàn với thứ tự cấp phát (Last In, First Out).

## Pitfalls to Avoid
- **Uninitialized Out-Variables:** Tầng ứng dụng không khởi tạo giá trị ban đầu cho biến out-parameter. Nếu hàm trả về lỗi và không ghi đè dữ liệu, ứng dụng vẫn sử dụng biến đó thì sẽ đọc phải giá trị rác (Garbage data) trên Stack.
- **Ignoring Return Values:** Lập trình viên gọi hàm nhưng bỏ qua (không check) trạng thái trả về. Khuyến nghị sử dụng compiler flag `-Wunused-result` hoặc `__attribute__((warn_unused_result))` trong GCC để chặn đứng lỗi này từ lúc compile.

## File Structure
- `system_errors.h`: Định nghĩa bảng mã lỗi chuẩn của toàn hệ thống.
- `sensor.h / .c`: API triển khai Return Value Pattern (trả về status, dữ liệu qua con trỏ).
- `main.c`: Minh họa việc bắt lỗi và áp dụng kiến trúc Goto Cleanup.
