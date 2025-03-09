#include <stdio.h>
#include <stdlib.h>

#define INF 100000000  // Gia tri lon, dung de khoi tao chi phi toi thieu

// Ham thuc hien thuat toan Greedy cho TSP va in ra cac buoc chon dinh va chi phi
void greedyTSP(int start, int n, int **c) {
    // Cap phat mang danh dau da tham
    int *visited = (int *) calloc(n, sizeof(int));
    // Mang luu hanh trinh (co n+1 phan tu, vi phai quay lai dinh xuat phat)
    int *tour = (int *) malloc((n + 1) * sizeof(int));
    int cost = 0;  // Tong chi phi
    int current = start;
    
    visited[current] = 1;
    tour[0] = current;
    int count = 1;
    
    printf("Bat dau tu thanh pho %d\n", current + 1);
    
    // Lap n-1 lan de chon dinh tiep theo chua tham co chi phi nho nhat
    for (int i = 1; i < n; i++) {
        int next = -1;
        int minCost = INF;
        // Duyet qua cac dinh de tim dinh chua tham co chi phi di tu dinh hien tai nho nhat
        for (int j = 0; j < n; j++) {
            if (!visited[j] && c[current][j] < minCost) {
                minCost = c[current][j];
                next = j;
            }
        }
        if (next == -1) break;  // Neu khong tim thay dinh nao (co loi)
        
        visited[next] = 1;
        tour[count++] = next;
        cost += minCost;
        printf("Buoc %d: Tu thanh pho %d -> thanh pho %d voi cost = %d, tong cost = %d\n",
               i, current + 1, next + 1, minCost, cost);
        current = next;
    }
    
    // Quay lai dinh xuat phat
    cost += c[current][start];
    tour[count] = start;
    printf("Buoc cuoi: Tu thanh pho %d -> thanh pho %d (quay ve xuat phat) voi cost = %d, tong cost = %d\n",
           current + 1, start + 1, c[current][start], cost);
    
    // In ra hanh trinh (hien thi theo 1-index)
    printf("\nHanh trinh: ");
    for (int i = 0; i <= n; i++) {
        printf("%d ", tour[i] + 1);
    }
    printf("\nTong chi phi: %d\n", cost);
    
    free(visited);
    free(tour);
}

int main() {
    // Mo file gts1a.txt de doc du lieu
    FILE *fp = fopen("gts1c.txt", "r");
    if (fp == NULL) {
        printf("Khong the mo file .txt\n");
        return 1;
    }
    
    int n, start;
    // Doc dong dau tien: so thanh pho n va dinh xuat phat
    fscanf(fp, "%d %d", &n, &start);
    // Neu file du lieu danh so dinh tu 1, thi chuyen sang 0-index cho xu ly
    start = start - 1;
    
    // Cap phat dong ma tran chi phi
    int **c = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        c[i] = (int *) malloc(n * sizeof(int));
    }
    
    // Doc ma tran chi phi tu file (du lieu luu theo hang, cach nhau boi khoang trang)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(fp, "%d", &c[i][j]) != 1) {
                printf("Loi doc phan tu [%d][%d]\n", i, j);
                return 1;
            }
        }
    }
    fclose(fp);
    
    // Thuc hien thuat toan Greedy TSP va in ket qua
    greedyTSP(start, n, c);
    
    // Giai phong bo nho da cap phat cho ma tran
    for (int i = 0; i < n; i++) {
        free(c[i]);
    }
    free(c);
    
    return 0;
}
