# Day 7: Inheritance Pattern

## Overview
Inheritance Pattern trong C cho phép tái sử dụng cấu trúc dữ liệu và logic bằng cách nhúng một "Base struct" vào vị trí đầu tiên của một "Derived struct". Kỹ thuật này dựa trên tiêu chuẩn C về việc không có padding trước thành viên đầu tiên, cho phép thực hiện ép kiểu an toàn (Upcasting) giữa đối tượng con và đối tượng cha.

## Core Concepts
- **Composition for Inheritance:** Nhúng instance của struct cha vào làm thành viên đầu tiên của struct con.
- **Upcasting:** Chuyển đổi con trỏ từ `Derived*` sang `Base*` để sử dụng các hàm dùng chung của hệ thống.
- **Memory Alignment:** Địa chỉ của struct con trùng với địa chỉ của struct cha, giúp tối ưu hóa việc quản lý bộ nhớ và truy xuất dữ liệu.

## Implementation Standards
1. **First Member Rule:** Struct cha BẮT BUỘC phải là thành viên đầu tiên của struct con.
2. **Explicit Constructor Chaining:** Hàm khởi tạo của đối tượng con phải chủ động gọi hàm khởi tạo của đối tượng cha để đảm bảo tính toàn vẹn của dữ liệu.
3. **Type Identification:** Khi sử dụng trong hệ thống lớn, nên có một trường `enum type_id` trong Base struct để hỗ trợ kiểm tra kiểu dữ liệu trước khi thực hiện Downcasting.

## Pitfalls to Avoid
- **Unchecked Downcasting:** Ép kiểu từ cha về con mà không kiểm tra loại đối tượng thực tế sẽ dẫn đến memory corruption và crash hệ thống.
- **Padding Issues:** Cần lưu ý về data alignment nếu struct cha có kích thước không chia hết cho 4 hoặc 8 (tùy kiến trúc MCU). Tuy nhiên, thành viên đầu tiên luôn an toàn.
- **Deep Inheritance:** Tránh việc kế thừa quá nhiều tầng (ví dụ A -> B -> C -> D) vì sẽ làm code trở nên cực kỳ khó debug và quản lý.

## File Structure
- `sensor.h / .c`: Base class định nghĩa các thuộc tính và hành vi chung cho mọi loại cảm biến.
- `temperature_sensor.h / .c`: Derived class kế thừa từ sensor và bổ sung các đặc tính chuyên biệt cho nhiệt độ.
- `main.c`: Minh họa việc sử dụng Upcasting để gọi hàm cha từ đối tượng con.