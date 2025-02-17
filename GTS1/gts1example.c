#include <stdio.h>
#define N 6
#define INF 99999  // Giá trị INF đại diện cho vô cực (không có đường nối)

void greedyTSP(int start, int n, int c[][N]) {
    int visited[N] = {0};    // Mảng đánh dấu các thành phố đã đi qua
    int tour[N + 1];         // Lưu chu trình (có N+1 phần tử, vì quay lại đỉnh xuất phát)
    int cost = 0;            // Tổng chi phí
    int current = start;     // Khởi tạo đỉnh hiện tại là đỉnh xuất phát
    visited[current] = 1;
    tour[0] = current;       // Thêm đỉnh xuất phát vào chu trình
    int count = 1;
    
    // Lặp qua các thành phố còn lại (n - 1 bước)
    for (int i = 1; i < n; i++) {
        int next = -1;
        int minCost = INF;
        // Tìm thành phố kế tiếp gần nhất chưa được thăm
        for (int j = 0; j < n; j++) {
            if (!visited[j] && c[current][j] < minCost) {
                minCost = c[current][j];
                next = j;
            }
        }
        if (next == -1) break;  // Trường hợp không tìm được thành phố tiếp theo
        visited[next] = 1;
        tour[count++] = next;   // Thêm thành phố được chọn vào chu trình
        cost += minCost;        // Cộng chi phí đi từ current sang next
        current = next;         // Cập nhật current
    }
    
    // Quay lại đỉnh xuất phát
    cost += c[current][start];
    tour[count] = start;
    
    // In kết quả: chu trình và tổng chi phí
    printf("Hanh trinh bat dau tai dinh %d: ", start + 1);
    for (int i = 0; i <= n; i++) {
        printf("%d ", tour[i] + 1);  // Cộng 1 nếu muốn đánh số thành phố từ 1 thay vì 0
    }
    printf("\nTong chi phi: %d\n", cost);
}

int main() {
    // Ma trận chi phí giữa các thành phố; INF biểu thị không có đường (hoặc tự động loại)
    int c[N][N] = {
        {INF, 20, 42, 31, 6, 24},
        {10, INF, 17, 6, 35, 18},
        {25, 5, INF, 27, 14, 9},
        {12, 9, 24, INF, 30, 12},
        {14, 7, 21, 15, INF, 38},
        {40, 15, 16, 5, 20, INF}
    };
    
    // Các đỉnh xuất phát theo ví dụ: V1=1, V2=3, V3=4, V4=5
    // Lưu ý: trong code các đỉnh được đánh số từ 0 nên V1->index 0, V3->index 2, V4->index 3, V5->index 4.
    int startingVertices[] = {0, 2, 3, 4};
    int numberOfStarts = sizeof(startingVertices) / sizeof(startingVertices[0]);
    
    for (int i = 0; i < numberOfStarts; i++) {
        greedyTSP(startingVertices[i], N, c);
        printf("\n");
    }
    
    return 0;
}
