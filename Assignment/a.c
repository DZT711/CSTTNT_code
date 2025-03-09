#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *f = fopen("assignmentd.txt", "r");
    if (f == NULL)
    {
        printf("Không thể mở file \n");
        return 1;
    }

    int n;
    fscanf(f, "%d", &n);

    // Cấp phát bộ nhớ cho ma trận hiệu quả
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            fscanf(f, "%d", &matrix[i][j]);
        }
    }
    fclose(f);

    // Mảng để đánh dấu công việc đã được giao hay chưa (0: chưa, 1: đã giao)
    int *jobAssigned = (int *)calloc(n, sizeof(int));
    // Mảng lưu kết quả phân công: assignment[i] chứa chỉ số công việc được giao cho thợ i
    int *assignment = (int *)malloc(n * sizeof(int));
    int total = 0; // Tổng hiệu quả

    // Với mỗi thợ, chọn công việc có hiệu quả cao nhất trong số các công việc chưa được giao
    for (int i = 0; i < n; i++)
    {
        int bestJob = -1;
        int bestVal = -1; // Giả sử hiệu quả không âm, nếu có giá trị âm cần điều chỉnh lại
        for (int j = 0; j < n; j++)
        {
            if (!jobAssigned[j] && matrix[i][j] > bestVal)
            {
                bestVal = matrix[i][j];
                bestJob = j;
            }
        }
        if (bestJob != -1)
        {
            assignment[i] = bestJob;
            jobAssigned[bestJob] = 1;
            total += bestVal;
        }
        else
        {
            assignment[i] = -1; // Trường hợp không có công việc khả thi
        }
    }

    // Xuất kết quả phân công
    printf("Tho\tCong Viec\n");
    for (int i = 0; i < n; i++)
    {
        // Cộng 1 để chuyển từ chỉ số 0-index sang 1-index theo yêu cầu
        printf("%d\t%d\n", i + 1, assignment[i] + 1);
    }
    printf("Tong chi phi phan cong = %d\n", total);

    // Giải phóng bộ nhớ
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    free(jobAssigned);
    free(assignment);

    return 0;
}
