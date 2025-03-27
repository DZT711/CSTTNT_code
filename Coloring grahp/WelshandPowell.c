#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1250
// can cai thiên kha nang xu li them dinh cho thuat toan nay
// Định nghĩa ngưỡng in thông tin debug chi tiết
#define DEBUG_THRESHOLD 30
//dieu chinh viec xuat trinh tu thuc thi cac buoc
typedef unsigned long long ULL;

// Hàm trả về số khối cần dùng cho bitmask (mỗi khối 64 bit)
int getMaskCount(int n) {
    return (n + 63) / 64;
}

// Cấu trúc lưu thông tin của đỉnh (chỉ số đỉnh và bậc)
typedef struct {
    int vertex;
    int degree;
} Vertex;

// Hàm so sánh dùng cho qsort
// Nếu hai đỉnh có cùng bậc thì đỉnh có chỉ số nhỏ hơn sẽ được xếp trước
int compare(const void *a, const void *b) {
    Vertex *v1 = (Vertex *)a;
    Vertex *v2 = (Vertex *)b;
    if (v2->degree == v1->degree) {
        return v1->vertex - v2->vertex; // sắp theo thứ tự tăng dần của chỉ số đỉnh
    }
    return v2->degree - v1->degree; // sắp theo bậc giảm dần
}

int main() {
    int n, i, j;
    FILE *f = fopen("color5.txt", "r");
    if (f == NULL) {
        printf("Không thể mở file \n");
        return 1;
    }
    
    // Đọc số đỉnh của đồ thị
    fscanf(f, "%d", &n);
    if(n > MAX_N) {
        printf("Số đỉnh vượt quá giới hạn (%d > %d)\n", n, MAX_N);
        fclose(f);
        return 1;
    }
    
    int maskCount = getMaskCount(n);
    
    // Cấp phát mảng bitmask cho mỗi đỉnh: neighbors[i] lưu tập các đỉnh kề của đỉnh i
    ULL **neighbors = (ULL **)malloc(n * sizeof(ULL *));
    for (i = 0; i < n; i++) {
        neighbors[i] = (ULL *)calloc(maskCount, sizeof(ULL));
    }
    
    int *degree = (int *)malloc(n * sizeof(int));
    // Đọc ma trận kề và tính bậc
    for (i = 0; i < n; i++) {
        degree[i] = 0;
        for (j = 0; j < n; j++) {
            int val;
            fscanf(f, "%d", &val);
            if (val == 1) {
                degree[i]++;
                int block = j / 64;
                int bit = j % 64;
                neighbors[i][block] |= (1ULL << bit);
            }
        }
    }
    fclose(f);

    // Nếu số đỉnh nhỏ thì in ra ma trận bậc (debug)
    if(n <= DEBUG_THRESHOLD) {
        printf("Bước 1: Tính bậc các đỉnh\n");
        printf("Đỉnh:\t");
        for (i = 0; i < n; i++){
            printf("%d\t", i+1);
        }
        printf("\nBậc:\t");
        for (i = 0; i < n; i++){
            printf("%d\t", degree[i]);
        }
        printf("\n\n");
    }
    
    // Tạo mảng các đỉnh để sắp xếp theo bậc giảm dần
    Vertex *vertices = (Vertex *)malloc(n * sizeof(Vertex));
    for (i = 0; i < n; i++) {
        vertices[i].vertex = i;
        vertices[i].degree = degree[i];
    }
    qsort(vertices, n, sizeof(Vertex), compare);
    
    if(n <= DEBUG_THRESHOLD) {
        printf("Bước 2: Sắp xếp các đỉnh theo bậc giảm dần (với ưu tiên đỉnh số nhỏ hơn khi bậc bằng nhau)\n");
        printf("Đỉnh:\t");
        for (i = 0; i < n; i++){
            printf("%d\t", vertices[i].vertex + 1);
        }
        printf("\nBậc:\t");
        for (i = 0; i < n; i++){
            printf("%d\t", vertices[i].degree);
        }
        printf("\n\n");
    }
    
    // Mảng lưu màu của các đỉnh (0 nghĩa là chưa được tô)
    int *color = (int *)calloc(n, sizeof(int));
    int maxColor = 0;
    
    // Mảng bitmask để lưu tập các đỉnh đã được tô với mỗi màu
    ULL **colorSet = (ULL **)malloc((n + 1) * sizeof(ULL *));
    for (i = 0; i <= n; i++) {
        colorSet[i] = (ULL *)calloc(maskCount, sizeof(ULL));
    }
    
    // Bắt đầu tô màu theo thuật toán Welsh-Powell với tối ưu bitmask:
    for (i = 0; i < n; i++) {
        int u = vertices[i].vertex;
        if (color[u] == 0) {  // Nếu đỉnh u chưa được tô
            maxColor++;       // Tạo một màu mới
            color[u] = maxColor;
            int block = u / 64;
            int bit = u % 64;
            colorSet[maxColor][block] |= (1ULL << bit);
            if(n <= DEBUG_THRESHOLD) {
                printf("Gán màu %d cho đỉnh %d\n", maxColor, u+1);
            }
            
            // Duyệt các đỉnh chưa tô trong danh sách đã sắp xếp
            for (j = i + 1; j < n; j++) {
                int v = vertices[j].vertex;
                if (color[v] == 0) {
                    int conflict = 0;
                    // Kiểm tra xem đỉnh v có kề với bất kỳ đỉnh nào đã được tô màu maxColor không
                    for (int b = 0; b < maskCount; b++) {
                        if (neighbors[v][b] & colorSet[maxColor][b]) {
                            conflict = 1;
                            break;
                        }
                    }
                    if (!conflict) {
                        color[v] = maxColor;
                        int blockv = v / 64;
                        int bitv = v % 64;
                        colorSet[maxColor][blockv] |= (1ULL << bitv);
                        if(n <= DEBUG_THRESHOLD) {
                            printf("Gán màu %d cho đỉnh %d\n", maxColor, v+1);
                        }
                    }
                }
            }
        }
    }
    
    // In kết quả cuối cùng
    printf("Số sắc sử dụng: %d\n", maxColor);
    for (int c = 1; c <= maxColor; c++){
        printf("Màu %d: ", c);
        for (i = 0; i < n; i++) {
            if (color[i] == c)
                printf("%d ", i + 1);
        }
        printf("\n");
    }
    
    // Nếu số đỉnh nhỏ thì in thêm thông tin chi tiết sau khi tô màu
    if(n <= DEBUG_THRESHOLD) {
        printf("\nBảng màu của các đỉnh:\n");
        for(i = 0; i < n; i++){
            printf("Đỉnh %d: Màu %d\n", i+1, color[i]);
        }
    }
    
    // In thông báo hoàn thành
    printf("\nTô màu hoàn tất.\n");
    
    // Giải phóng bộ nhớ
    for (i = 0; i < n; i++) {
        free(neighbors[i]);
    }
    free(neighbors);
    free(degree);
    free(vertices);
    free(color);
    for (i = 0; i <= n; i++) {
        free(colorSet[i]);
    }
    free(colorSet);
    
    return 0;
}
