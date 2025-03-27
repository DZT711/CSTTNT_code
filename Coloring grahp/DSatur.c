#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_N 1250
#define MAX_COLOR 1250

// DSatur algorithm: doc do thi tu file "colorexample.txt" voi dinh dang:
// dong dau tien: n (so dinh)
// sau do la ma tran ke (n x n)
int main() {
    int n, i, j;
    FILE *f = fopen("color5.txt", "r");
    if (f == NULL) {
        printf("Khong the mo file t\n");
        return 1;
    }
    fscanf(f, "%d", &n);
    if(n > MAX_N) {
        printf("So dinh vuot qua gioi han (%d > %d)\n", n, MAX_N);
        fclose(f);
        return 1;
    }
    int **graph = (int**)malloc(n * sizeof(int*));
    for (i = 0; i < n; i++) {
        graph[i] = (int*)malloc(n * sizeof(int));
        for (j = 0; j < n; j++) {
            fscanf(f, "%d", &graph[i][j]);
        }
    }
    fclose(f);
    
    // Tinh bac ban dau cua cac dinh
    int *degree = (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++){
        degree[i] = 0;
        for(j = 0; j < n; j++){
            degree[i] += graph[i][j];
        }
    }
    
    // Khoi tao cac bien cho DSatur
    int *color = (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        color[i] = -1; // -1 nghia la chua to
    }
    // "sat" luu so mau khac nhau da co o cac dinh ke da to (do bao han)
    int *sat = (int*)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) sat[i] = 0;
    
    // Mang forbidden[v][c]: true neu voi dinh v, mau c bi can (da xuat hien o dinh ke)
    bool **forbidden = (bool**)malloc(n * sizeof(bool*));
    for(i = 0; i < n; i++){
        forbidden[i] = (bool*)malloc(MAX_COLOR * sizeof(bool));
        for(j = 0; j < MAX_COLOR; j++){
            forbidden[i][j] = false;
        }
    }
    
    // "currDegree" la so dinh ke chua to cua moi dinh (de tie-break)
    int *currDegree = (int*)malloc(n * sizeof(int));
    for(i = 0; i < n; i++){
        currDegree[i] = degree[i];
    }
    
    int iteration = 0;
    // Vong lap DSatur: tiep tuc cho den khi tat ca cac dinh duoc to
    while (1) {
        // Kiem tra so dinh chua to
        int uncoloredCount = 0;
        for(i = 0; i < n; i++){
            if(color[i] == -1) uncoloredCount++;
        }
        if(uncoloredCount == 0) break;
        
        // Chon dinh v chua to co do bao han cao nhat.
        // Neu tie, chon dinh co currDegree (so ke chua to) lon nhat, tie tie chon dinh so nho hon.
        int chosen = -1;
        int maxSat = -1;
        int maxDeg = -1;
        for(i = 0; i < n; i++){
            if(color[i] == -1) {
                if(sat[i] > maxSat || 
                   (sat[i] == maxSat && currDegree[i] > maxDeg) || 
                   (sat[i] == maxSat && currDegree[i] == maxDeg && (chosen == -1 || i < chosen))) {
                    chosen = i;
                    maxSat = sat[i];
                    maxDeg = currDegree[i];
                }
            }
        }
        
        // Xac dinh mau nho nhat khong bi can cho dinh duoc chon
        int c;
        for(c = 0; c < MAX_COLOR; c++){
            if(!forbidden[chosen][c]) break;
        }
        color[chosen] = c;
        
        // In ra thong tin buoc to mau
        // (Luu y: in ra mau theo nhan 1,2,3,... thay vi 0,1,2,...)
        printf("To mau lan %d: dinh %d duoc to mau %d\n", iteration+1, chosen+1, c+1);
        
        // Cap nhat cho cac dinh ke (chua to)
        for(i = 0; i < n; i++){
            if(graph[chosen][i] == 1 && color[i] == -1) {
                // Neu mau vua gan chua bi can o dinh i, cap nhat forbidden va do bao han
                if(!forbidden[i][c]) {
                    forbidden[i][c] = true;
                    sat[i]++;
                    printf("   Cap nhat: dinh %d - cam to mau %d\n", i+1, c+1);
                }
                // Giam currDegree cua cac dinh ke vi dinh vua to se khong duoc xem lai
                currDegree[i]--;
            }
        }
        // Sau khi to, gan currDegree cua dinh vua to = 0
        currDegree[chosen] = 0;
        
        // In ra trang thai "Ha bac" (so ket noi chua to) sau lan nay
        printf("Ha bac lan %d: ", iteration+1);
        for(i = 0; i < n; i++){
            printf("%d ", currDegree[i]);
        }
        printf("\n");
        
        iteration++;
    }
    
    // In ket qua cuoi cung
    printf("\nKet luan\n");
    int maxColorUsed = 0;
    for(i = 0; i < n; i++){
        if(color[i] > maxColorUsed) maxColorUsed = color[i];
    }
    printf("Sac so la: %d\n", maxColorUsed+1);
    {
        int c;
        for(c = 0; c <= maxColorUsed; c++){
            printf("Mau %d: ", c+1);
            for(i = 0; i < n; i++){
                if(color[i] == c) {
                    printf("%d ", i+1);
                }
            }
            printf("\n");
        }
    }
    
    printf("To mau cac dinh con lai: ");
    for(i = 0; i < n; i++){
        // In ra mau cua dinh (cong 1 de in theo nhan 1,2,...)
        if(color[i] != -1) {
            printf("%d ", color[i]+1);
        }
    }
    printf("\n");
    
    // Giai phong bo nho
    for(i = 0; i < n; i++){
        free(graph[i]);
        free(forbidden[i]);
    }
    free(graph);
    free(forbidden);
    free(degree);
    free(color);
    free(sat);
    free(currDegree);
    
    return 0;
}
