#include "sensor.h"

/*
1. Phân tích kiến trúc Opaque Pattern
Trong Opaque Pattern, file header (.h) chỉ khai báo cho trình biên dịch biết "có một struct tên là như vậy tồn tại", nhưng không nói rõ bên trong nó có gì.

Khi tầng Application include file .h này, họ chỉ có thể khai báo các con trỏ (pointers) trỏ tới kiểu dữ liệu đó, vì kích thước của con trỏ là cố định (4 bytes trên MCU 32-bit). Bất kỳ nỗ lực nào giải tham chiếu (dereference) như sensor->pin = 1 sẽ bị trình biên dịch báo lỗi dereferencing pointer to incomplete type.

2. Vượt qua rào cản Heap bằng Static Memory Pool
Thông thường, vì Application không biết kích thước của struct, họ không thể khai báo tĩnh kiểu struct sensor s1;. 
Thư viện bắt buộc phải dùng malloc để cấp phát bộ nhớ và trả về con trỏ. 
Trong lập trình nhúng độ tin cậy cao, malloc gây ra phân mảnh bộ nhớ (Memory Fragmentation) và có thể làm crash hệ thống.

Giải pháp là chúng ta sẽ khai báo sẵn một mảng tĩnh (Static array) các struct nằm ẩn bên trong file .c. 
Các hàm create và destroy sẽ thao tác tìm kiếm và đánh dấu vùng nhớ trên mảng này thay vì xin RAM từ hệ điều hành.

4. Phân tích rủi ro và Điểm cần lưu ý (Pitfalls)
Lãng phí RAM (Resource Overhead): Bạn phải định nghĩa MAX_SENSORS đủ lớn cho trường hợp xấu nhất (Worst-case). 
Nếu khai báo MAX_SENSORS = 10 nhưng thực tế chỉ dùng 2, RAM cho 8 block còn lại sẽ bị lãng phí. 
Trong thực tế, các macro này thường được cấu hình qua CMake/Makefile (VD: -DMAX_SENSORS=2) tùy thuộc vào loại Board vật lý để tối ưu RAM.

Race Condition trong RTOS: Mảng sensor_allocated đang chia sẻ trạng thái chung. 
Nếu Task A và Task B cùng gọi sensor_create đồng thời, có nguy cơ cả hai được cấp phát cùng một block nhớ. 
Trong môi trường RTOS, vòng lặp for trong sensor_create và sensor_destroy phải được bọc lại bằng Spinlock hoặc Mutex (Chúng ta sẽ học ở Ngày 12).

Dangling Pointer (Lỗi trỏ lạc): Trong main.c, sau khi gọi sensor_destroy(s1), con trỏ s1 vẫn mang giá trị địa chỉ của block đó. 
Nếu hệ thống tiếp tục cấp phát block này cho s_fail và user vô tình gọi sensor_read(s1), hệ thống không crash (vì vùng nhớ vẫn hợp lệ), nhưng dữ liệu sẽ bị sai (đọc nhầm pin của s_fail). Trách nhiệm của tầng Application là gán s1 = NULL sau khi destroy.
*/
int main(void)
{
	struct sensor *s1;
	struct sensor *s2;
	struct sensor *s_fail;
	int val;

	s1 = sensor_create(5);
	s2 = sensor_create(6);

	s_fail = sensor_create(7);
	s_fail = sensor_create(8);
	s_fail = sensor_create(9);

	if (s1)
		val = sensor_read(s1);

	sensor_destroy(s1);
	sensor_destroy(s2);

	return 0;
}