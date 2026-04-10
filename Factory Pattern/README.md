# Day 5: Factory Pattern

## Overview
Factory Pattern được sử dụng để đóng gói và trừu tượng hóa quá trình khởi tạo đối tượng. Trong hệ thống Embedded, việc cấu hình một ngoại vi (I2C, SPI, Sensors) thường bao gồm nhiều thao tác thanh ghi phức tạp. Factory chịu trách nhiệm thực thi các quy trình này dựa trên các tham số đầu vào đơn giản (như enum cấu hình) và trả về một instance đã sẵn sàng hoạt động.

## Core Concepts
- **Creation Abstraction:** Application không cần biết làm thế nào để cấu hình thanh ghi của BME280 hay DHT11, nó chỉ cần yêu cầu Factory tạo ra cảm biến tương ứng.
- **Centralized Initialization:** Mọi logic cấp phát bộ nhớ và thiết lập phần cứng ban đầu được tập trung vào một hàm Factory duy nhất.
- **Rollback Mechanism:** Nếu quá trình khởi tạo phần cứng (hardware probe) thất bại, Factory tự động thu hồi vùng nhớ (slot) đã cấp phát trước khi trả về lỗi cho ứng dụng, ngăn ngừa rò rỉ tài nguyên.

## Implementation Standards
1. **Interface Segregation:** Tách biệt file header của API chức năng (`sensor.h`) và API khởi tạo (`sensor_factory.h`).
2. **Deterministic Allocation:** Vẫn sử dụng Static Memory Pool để đảm bảo an toàn bộ nhớ.
3. **Error Propagation:** Hàm Factory trả về Opaque Pointer. Nếu thất bại (hết RAM tĩnh hoặc phần cứng không phản hồi), hàm phải trả về `NULL`. Application bắt buộc phải check Null.

## Pitfalls to Avoid
- **God Factory:** Tránh việc dồn logic khởi tạo của toàn bộ hệ thống (LED, Motor, Sensor, UART) vào một file Factory duy nhất. Mỗi module logic nên có Factory riêng của nó.
- **Switch-case Hell:** Khi số lượng biến thể (models) quá lớn, khối `switch-case` trong Factory sẽ vi phạm nguyên tắc Open-Closed. Giải pháp lúc này là chuyển sang sử dụng Registry/VTable (sẽ áp dụng ở pattern đa hình - Polymorphism).

## File Structure
- `sensor.h`: Giao diện API (Opaque Pointer, Read, Destroy).
- `sensor_factory.h`: Giao diện Factory (Enums, Create method).
- `sensor_core.c`: Triển khai Memory Pool tĩnh, logic cấu hình phần cứng chuyên biệt và Factory logic.
- `main.c`: Tầng ứng dụng gọi Factory.