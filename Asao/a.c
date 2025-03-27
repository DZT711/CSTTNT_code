#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 1000

// Cấu trúc cho 1 cạnh
typedef struct
{
    int v; // đỉnh kề
    int w; // trọng số cạnh
} Edge;

// Danh sách kề cho đồ thị: sử dụng mảng 1-based
Edge *adj[MAX + 1];
int adjSize[MAX + 1];

// Mảng lưu heuristic h(u) (1-based)
double h[MAX + 1];

// Mảng lưu giá trị g(u) = chi phí từ s đến u (1-based)
double gScore[MAX + 1];

// Mảng lưu cha (để truy vết đường đi)
int parent[MAX + 1];

// Mảng đánh dấu đỉnh đã duyệt xong
int closedSet[MAX + 1]; // 0: chưa duyệt, 1: đã duyệt

// Cấu trúc cho hàng đợi ưu tiên (Open)
typedef struct
{
    int vertex;    // đỉnh
    double fScore; // f(u) = g(u) + h(u)
} NodePQ;

NodePQ openSet[MAX + 1];
int openSize = 0;

// Hàm thêm 1 Node vào Open
void pushOpen(int v, double fVal)
{
    openSet[openSize].vertex = v;
    openSet[openSize].fScore = fVal;
    openSize++;
}

// Lấy và xoá đỉnh có fScore nhỏ nhất từ Open
int popOpen()
{
    if (openSize == 0)
    {
        return -1; // báo lỗi nếu không còn phần tử
    }
    int minIndex = 0;
    double minVal = openSet[0].fScore;
    for (int i = 1; i < openSize; i++)
    {
        if (openSet[i].fScore < minVal)
        {
            minVal = openSet[i].fScore;
            minIndex = i;
        }
    }
    int v = openSet[minIndex].vertex;
    openSize--;
    openSet[minIndex] = openSet[openSize];
    return v;
}

// Kiểm tra xem đỉnh v có nằm trong Open hay không
int inOpen(int v)
{
    for (int i = 0; i < openSize; i++)
    {
        if (openSet[i].vertex == v)
        {
            return 1;
        }
    }
    return 0;
}

// Hàm truy vết đường đi từ t về s
void reconstructPath(int s, int t)
{
    if (parent[t] == -1 && s != t)
    {
        printf("Khong tim thay duong di.\n");
        return;
    }
    int path[MAX + 1];
    int idx = 0;
    int current = t;
    while (current != -1)
    {
        path[idx++] = current;
        if (current == s)
            break;
        current = parent[current];
    }
    // In đường đi theo thứ tự từ s -> t
    for (int i = idx - 1; i >= 0; i--)
    {
        printf("%d", path[i]);
        if (i > 0)
            printf(" -> ");
    }
    printf("\n");
}

// Triển khai thuật toán A*
void AStar(int s, int t, int n)
{
    // Khởi tạo
    for (int i = 1; i <= n; i++)
    {
        gScore[i] = INT_MAX;
        parent[i] = -1;
        closedSet[i] = 0;
    }
    openSize = 0;

    // Khởi tạo đỉnh s: g(s)=0, f(s)=h(s)
    gScore[s] = 0.0;
    pushOpen(s, gScore[s] + h[s]);

    // Vòng lặp chính
    while (openSize > 0)
    {
        int u = popOpen();
        if (u == -1)
            break;

        if (u == t)
        {
            // Tìm được đỉnh t, truy vết và in kết quả
            printf("Duong di tim duoc (theo dinh):\n");
            reconstructPath(s, t);
            printf("Tong chi phi duong di: %.2f\n", gScore[t]);
            return;
        }

        closedSet[u] = 1; // đánh dấu u đã duyệt

        // Duyệt các đỉnh kề của u
        for (int i = 0; i < adjSize[u]; i++)
        {
            int v = adj[u][i].v;
            int w = adj[u][i].w;
            if (closedSet[v])
                continue;
            double tentative_gScore = gScore[u] + w;
            if (tentative_gScore < gScore[v])
            {
                gScore[v] = tentative_gScore;
                parent[v] = u;
                double fVal = gScore[v] + h[v];
                if (!inOpen(v))
                {
                    pushOpen(v, fVal);
                }
            }
        }
    }
    printf("Khong tim thay duong di tu %d den %d.\n", s, t);
}

int main()
{
    // Mở file path2.txt để đọc dữ liệu
    FILE *fp = fopen("path4.txt", "r");
    if (fp == NULL)
    {
        printf("Khong the mo file   \n");
        return 1;
    }

    int n, m, s, t;

    // Đọc dòng đầu: n, m, s, t
    fscanf(fp, "%d %d %d %d", &n, &m, &s, &t);

    // Vì dữ liệu file dùng đỉnh 1-based, ta sẽ sử dụng các mảng từ 1 đến n.
    // Khởi tạo danh sách kề
    for (int i = 1; i <= n; i++)
    {
        adjSize[i] = 0;
        // Cấp phát tối đa n phần tử cho mỗi đỉnh
        adj[i] = (Edge *)malloc(sizeof(Edge) * (n + 1));
    }

    // Đọc m dòng chứa thông tin cạnh: u, v, w
    for (int i = 1; i <= m; i++)
    {
        int u, v, w;
        fscanf(fp, "%d %d %d", &u, &v, &w);
        // Lưu vào danh sách kề: giả sử đồ thị có hướng
        adj[u][adjSize[u]].v = v;
        adj[u][adjSize[u]].w = w;
        adjSize[u]++;
        // Nếu đồ thị vô hướng, bạn có thể thêm dòng sau:
        // adj[v][adjSize[v]].v = u;
        // adj[v][adjSize[v]].w = w;
        // adjSize[v]++;
    }

    // Đọc dòng cuối: heuristic cho từng đỉnh (1-based)
    for (int i = 1; i <= n; i++)
    {
        fscanf(fp, "%lf", &h[i]);
    }

    fclose(fp);

    // Gọi thuật toán A*
    AStar(s, t, n);

    // Giải phóng bộ nhớ đã cấp phát
    for (int i = 1; i <= n; i++)
    {
        free(adj[i]);
    }

    return 0;
}
