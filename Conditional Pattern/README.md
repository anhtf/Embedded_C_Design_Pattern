# Day 15: Conditional Pattern (Condition Variables)
> Last updated: 2026-04-10

## Overview
Conditional Pattern (hay Condition Variables - CondVars) là công cụ đồng bộ hóa giải quyết triệt để tình trạng Busy-Waiting/Polling khi một luồng (Task) cần chờ một biểu thức logic trở thành sự thật (ví dụ: `buffer_count > 0` hoặc `state == READY`). Nó cho phép Task giải phóng khóa (Mutex) và đi vào giấc ngủ một cách nguyên tử (atomically), sau đó được đánh thức chính xác khi trạng thái hệ thống thay đổi.

## Core Concepts
- **Atomic Wait:** Hàm `wait` thực hiện 2 việc cùng lúc: đưa Task vào danh sách chờ và mở khóa Mutex để các Task khác có thể vào vùng găng thay đổi trạng thái. Khi được đánh thức, nó tự động khóa Mutex lại trước khi trả về.
- **Signal vs Broadcast:** `signal` chỉ đánh thức 1 Task đang ngủ chờ điều kiện đó. `broadcast` đánh thức TẤT CẢ các Task đang chờ.
- **The State Predicate:** CondVar không bao giờ đứng một mình. Nó luôn phải đi kèm với một biến trạng thái thực tế (như biến `count`) và một Mutex để bảo vệ biến đó.

## Implementation Standards (Linux Kernel Style)
1. **The `while` Loop Rule:** Việc kiểm tra điều kiện trước khi gọi `wait` BẮT BUỘC phải nằm trong một vòng lặp `while`, không bao giờ dùng `if`. Điều này bảo vệ hệ thống khỏi hiện tượng Spurious Wakeup (Thức giấc giả).
2. **Lock Before Wait/Signal:** Cả Task kiểm tra điều kiện và Task thay đổi điều kiện đều phải khóa Mutex trước khi thao tác với biến trạng thái và gọi hàm `wait`/`signal`.

## Pitfalls to Avoid
- **Lost Wakeups:** Gọi `signal` khi không có Task nào đang gọi `wait` sẽ làm tín hiệu biến mất. Hệ thống phải được thiết kế dựa trên Biến Trạng Thái, CondVar chỉ đóng vai trò đánh thức.
- **Deadlock by Forgetting Unlock:** Nếu một thao tác báo lỗi hoặc thoát sớm giữa vùng găng mà quên nhả khóa, Task đang ngủ ở `cond_wait` sẽ vĩnh viễn không bao giờ lấy lại được khóa khi thức dậy.

## File Structure
- `os_cond.h`: Interface định nghĩa OS Abstraction Layer cho Condition Variable.
- `os_cond.c`: Wrapper API cho hệ điều hành thực tế (FreeRTOS, Zephyr, pthreads).
- `main.c`: Triển khai mô hình Bounded-Buffer Producer/Consumer.
