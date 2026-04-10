/*
1. Tại sao cần Singleton trong Embedded?
Nếu bạn sử dụng Object Pattern (Ngày 2) cho Watchdog Timer, một lập trình viên khác có thể vô tình tạo thêm một struct watchdog wdt2 và gọi hàm init lần nữa. Việc này có thể gây ra:

Xung đột cấu hình: Ghi đè lên các thanh ghi điều khiển đang hoạt động.

Lỗi logic: Một Task nghĩ rằng Watchdog đã tắt, trong khi Task khác vừa bật nó lên.

Lãng phí RAM: Cấp phát nhiều vùng nhớ cho cùng một thực thể vật lý.

3. Các lỗi và điểm cần chú ý (Critical Points)
Vi phạm tính Singleton: Một lỗi phổ biến là kiểm tra if (!inst) nhưng lại quên kiểm tra inst != &wdt_singleton. Nếu người dùng tự ý "chế" một con trỏ rác và truyền vào, hệ thống sẽ gặp hành vi không xác định.

Thread Safety (RTOS): Trong môi trường đa luồng, nếu hai Task cùng gọi watchdog_get_instance lần đầu tiên đồng thời, có thể xảy ra Race Condition. 
Tuy nhiên, trong Embedded, Singleton thường được khởi tạo một lần duy nhất tại hàm main() trước khi bộ lập lịch (Scheduler) của RTOS bắt đầu chạy, nên rủi ro này thường được kiểm soát bằng quy trình khởi tạo.

Khó khăn khi Unit Test: Singleton tạo ra một trạng thái toàn cục (Global State). Khi viết test, nếu một test case làm thay đổi trạng thái của Singleton, nó sẽ ảnh hưởng đến các test case sau đó.

Giải pháp: Cần có một hàm "backdoor" là watchdog_reset_internal_state() chỉ dành riêng cho môi trường Test để đưa Singleton về trạng thái chưa khởi tạo.
*/