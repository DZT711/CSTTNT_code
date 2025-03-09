#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *f = fopen("scheduled.txt", "r");
    if (f == NULL)
    {
        printf("Khong the mo file \n");
        return 1;
    }

    int n;
    fscanf(f, "%d", &n);

    // Mang thoi gian thuc hien cua cac cong viec J1...Jn
    int *T = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        fscanf(f, "%d", &T[i]);
    }
    fclose(f);

    int m = 3; // so may (P1, P2, P3)

    // Mang luu tru thoi gian hien tai cua moi may (tong thoi gian cong viec da duoc gan)
    int machineTime[3] = {0, 0, 0};

    // Cho moi may, luu tru danh sach cong viec duoc gan (luu index cua cong viec)
    // Ta dung mang dong voi kich thuoc toi da la n cong viec
    int **assignments = (int **)malloc(m * sizeof(int *));
    int *count = (int *)malloc(m * sizeof(int)); // dem so cong viec cua moi may
    for (int i = 0; i < m; i++)
    {
        assignments[i] = (int *)malloc(n * sizeof(int));
        count[i] = 0;
    }

    // Thuat toan: duyet danh sach cong viec theo thu tu L (tu J1 -> Jn)
    // voi moi cong viec, gan cho may co thoi gian hien tai nho nhat
    // Neu co nhieu may cung nho, may voi chi so thap duoc uu tien
    for (int i = 0; i < n; i++)
    {
        int bestMachine = 0;
        for (int j = 1; j < m; j++)
        {
            if (machineTime[j] < machineTime[bestMachine])
            {
                bestMachine = j;
            }
        }
        assignments[bestMachine][count[bestMachine]++] = i; // gan cong viec i vao may bestMachine
        machineTime[bestMachine] += T[i];                   // cap nhat lai thoi gian cua may
    }

    // Tinh thoi gian hoan thanh (la thoi gian lon nhat cua cac may)
    int maxTime = machineTime[0];
    for (int i = 1; i < m; i++)
    {
        if (machineTime[i] > maxTime)
            maxTime = machineTime[i];
    }

    // In ket qua
    // for (int i = 0; i < m; i++)
    // {
    //     printf("P%d: ", i + 1);
    //     for (int j = 0; j < count[i]; j++)
    //     {
    //         int job = assignments[i][j];
    //         printf("J%d(%d)", job + 1, T[job]);
    //         if (j < count[i] - 1)
    //             printf(", ");
    //     }
    //     printf("\n");
    // }
    printf("Thoi gian hoan thanh tat ca cac cong viec la: %d\n", maxTime);

    // Giai phong bo nho
    for (int i = 0; i < m; i++)
    {
        free(assignments[i]);
    }
    free(assignments);
    free(count);
    free(T);

    return 0;
}
