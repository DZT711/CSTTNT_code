#include <stdio.h>
#include <stdlib.h>

#define INF 100000000  // Gia tri lon de khoi tao (0 tren duong cheo cua file duoc dinh nghia la vo cuc)


// Ham GTS1: tinh tour theo thuat toan greedy cho TSP tu dinh bat dau
// Input: start (0-index), n, ma tran chi phi c (n x n)
// Output: mang tour (co n+1 phan tu, dinh cuoi la dinh xuat phat) va tra ve tong cost
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
        // Tim thanh pho chua di voi chi phi nho nhat
        for (int j = 0; j < n; j++) {
            if (!visited[j] && c[current][j] < minCost) {
                minCost = c[current][j];
                next = j;
            }
        }
        if (next == -1) break;  // Neu khong tim thay (truong hop bat thuong)
        
        visited[next] = 1;
        tour[count++] = next;
        cost += minCost;
        printf("Buoc %d: Tu thanh pho %d -> thanh pho %d voi cost = %d, tong cost = %d\n",
               i, current + 1, next + 1, minCost, cost);
        current = next;
    }
    
    // Quay lai thanh pho xuat phat
    cost += c[current][start];
    tour[count] = start;
    printf("Buoc cuoi: Tu thanh pho %d -> thanh pho %d voi cost = %d, tong cost = %d\n",
           current + 1, start + 1, c[current][start], cost);
    
    free(visited);
    return cost;
}

int main() {
    FILE *fp = fopen("gts2c.txt", "r");
    if (fp == NULL) {
        printf("Khong the mo file gts2a.txt\n");
        return 1;
    }
    
    int n, p;
    // Dong dau: n (so thanh pho) va p (so thanh pho bat dau co san)
    fscanf(fp, "%d %d", &n, &p);
    
    // Doc mang cac thanh pho bat dau (1-index trong file -> chuyen sang 0-index)
    int *startCities = (int *) malloc(p * sizeof(int));
    for (int i = 0; i < p; i++) {
        fscanf(fp, "%d", &startCities[i]);
        startCities[i]--; // chuyen sang 0-index
    }
    
    // Cap phat ma tran chi phi (n x n)
    int **c = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        c[i] = (int *) malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(fp, "%d", &c[i][j]);
            // Neu gia tri la 0 va i == j, thi duoc coi la INF (vo cuc)
            if(i == j && c[i][j] == 0)
                c[i][j] = INF;
        }
    }
    fclose(fp);
    
    // Bien luu best tour va best cost (duoc khoi tao bestCost la INF)
    int bestCost = INF;
    int *bestTour = (int *) malloc((n + 1) * sizeof(int));
    int *currentTour = (int *) malloc((n + 1) * sizeof(int));
    
    printf("\n====== GTS2: Duyet cac thanh pho bat dau ======\n");
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
    
    // In ra ket qua cua GTS2: tour tot nhat va chi phi tot nhat
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
