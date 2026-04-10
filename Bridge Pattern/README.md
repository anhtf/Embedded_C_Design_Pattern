# Day 9: Bridge Pattern (Hardware Abstraction)

## Overview
Bridge Pattern là thiết kế cốt lõi để xây dựng các lớp trừu tượng phần cứng (Hardware Abstraction Layers - HAL). Nó giải quyết vấn đề "Cartesian Product" (Sự bùng nổ tổ hợp), nơi bạn có M loại cảm biến và N loại giao thức giao tiếp, bằng cách tách bạch hoàn toàn logic ứng dụng (Abstraction) khỏi chi tiết điều khiển phần cứng (Implementation).

## Core Concepts
- **Abstraction:** Lớp chức năng bậc cao (ví dụ: Driver cảm biến, Module tính toán). Tầng này xử lý logic, tính toán, và không có bất kỳ lệnh gọi thanh ghi phần cứng nào.
- **Implementation (Interface):** Một VTable định nghĩa các chuẩn giao tiếp vật lý cơ bản (ví dụ: `read_register`, `write_register`).
- **The Bridge:** Lớp Abstraction sở hữu một con trỏ trỏ tới giao diện Implementation. Mọi nhu cầu tương tác phần cứng đều được "ủy quyền" (delegate) qua con trỏ này.

## Implementation Standards (Linux Kernel Style)
1. **PIMPL Idiom (Pointer to Implementation):** Khai báo một con trỏ tới một `struct` interface thay vì include trực tiếp thư viện phần cứng.
2. **Strict Decoupling:** File mã nguồn của lớp Abstraction (`sensor.c`) tuyệt đối không được phép chứa bất kỳ file header phần cứng nào (`stm32_hal.h`, `avr_io.h`).
3. **Lifetime Management:** Đảm bảo đối tượng Bus (Implementation) có vòng đời (lifetime) bằng hoặc dài hơn đối tượng Sensor (Abstraction). Khuyến nghị sử dụng cấp phát tĩnh (`static`) ở cấp độ hệ thống.

## Pitfalls to Avoid
- **Dangling Bridge Pointers:** Hủy đối tượng Implementation trong khi Abstraction vẫn đang trỏ tới nó, dẫn đến Hard Fault khi thực thi.
- **Over-Abstraction:** Tạo ra các VTable quá phức tạp cho những thao tác đơn giản (như bật tắt 1 chân LED), gây lãng phí bộ nhớ và CPU cycles.
- **Context Loss:** Đối tượng Bus thường cần một con trỏ `void *private_data` để lưu trữ ngữ cảnh phần cứng cụ thể (ví dụ: cấu hình thanh ghi DMA, port I2C vật lý), không được bỏ qua con trỏ này.

## File Structure
- `bus.h`: Định nghĩa Interface giao tiếp chung.
- `stm32_i2c.h / .c`: Triển khai cụ thể cho vi điều khiển STM32.
- `sensor.h / .c`: Logic tầng cao chứa con trỏ Bridge.
- `main.c`: Lắp ráp (Dependency Injection) Implementation vào Abstraction.