#include <stdio.h>
#include <stdlib.h>

#define INF 100000000  // Gia tri lon de khoi tao

// Ham thuc hien GTS1: tinh hanh trinh theo thuat toan greedy TSP tu dinh bat dau
// Input: start (0-index), n, ma tran chi phi c (n x n)
// Output: tour (mang co kich thuoc n+1) va tra ve tong chi phi (cost)
int greedyTSP(int start, int n, int **c, int *tour) {
    int *visited = (int *) calloc(n, sizeof(int));
    int cost = 0;
    int current = start;
    visited[current] = 1;
    tour[0] = current;
    int count = 1;
    
    printf("Bat dau tu thanh pho %d\n", current + 1);
    
    for (int i = 1; i < n; i++) {
        int next = -1;
        int minCost = INF;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && c[current][j] < minCost) {
                minCost = c[current][j];
                next = j;
            }
        }
        if (next == -1) break;  // Neu co loi (khong tim thay dinh nao)
        
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
    printf("Buoc cuoi: Tu thanh pho %d -> thanh pho %d voi cost = %d, tong cost = %d\n",
           current + 1, start + 1, c[current][start], cost);
    
    free(visited);
    return cost;
}

int main() {
    // Mo file gts2a.txt de doc du lieu:
    // Dong dau: n p (so thanh pho va so dinh bat dau cho truoc)
    // Dong tiep theo: p so (danh sach cac dinh bat dau, duoc danh so theo 1-index)
    // Sau do: n dong, moi dong co n so la ma tran chi phi (INF duoc bieu dien bang mot so lon)
    FILE *fp = fopen("gts2ex.txt", "r");
    if (fp == NULL) {
        printf("Khong the mo file gts2a.txt\n");
        return 1;
    }
    
    int n, p;
    fscanf(fp, "%d %d", &n, &p);
    
    int *startCities = (int *) malloc(p * sizeof(int));
    for (int i = 0; i < p; i++) {
        fscanf(fp, "%d", &startCities[i]);
        startCities[i]--; // chuyen sang 0-index
    }
    
    // Cap phat ma tran chi phi
    int **c = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        c[i] = (int *) malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(fp, "%d", &c[i][j]);
        }
    }
    fclose(fp);
    
    // Bien de luu hanh trinh tot nhat va chi phi tot nhat
    int bestCost = INF;
    int *bestTour = (int *) malloc((n + 1) * sizeof(int));
    int *currentTour = (int *) malloc((n + 1) * sizeof(int));
    
    // Lap qua cac dinh bat dau da cho (p dinh)
    for (int i = 0; i < p; i++) {
        printf("\n----------------------\n");
        printf("Chay GTS1 voi thanh pho bat dau: %d\n", startCities[i] + 1);
        int cost = greedyTSP(startCities[i], n, c, currentTour);
        
        printf("Ket qua tour: ");
        for (int j = 0; j <= n; j++) {
            printf("%d ", currentTour[j] + 1);
        }
        printf("\nTong cost = %d\n", cost);
        
        if (cost < bestCost) {
            bestCost = cost;
            for (int j = 0; j <= n; j++) {
                bestTour[j] = currentTour[j];
            }
        }
    }
    
    // In ra hanh trinh tot nhat va tong chi phi tot nhat
    printf("\n=========================\n");
    printf("Hanh trinh tot nhat co chi phi = %d\n", bestCost);
    printf("Chi tiet tour: ");
    for (int i = 0; i <= n; i++) {
        printf("%d ", bestTour[i] + 1);
    }
    printf("\n");
    
    // Giai phong bo nho
    free(startCities);
    free(currentTour);
    free(bestTour);
    for (int i = 0; i < n; i++) {
        free(c[i]);
    }
    free(c);
    
    return 0;
}
