#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id; // so thu tu chi tiet (tu 1 den n)
    int a;  // thoi gian gia cong tren may A
    int b;  // thoi gian gia cong tren may B
} Job;

// Ham so sanh de sap xep nhom N1 tang theo thoi gian tren may A
int cmpN1(const void *p1, const void *p2)
{
    Job *j1 = (Job *)p1;
    Job *j2 = (Job *)p2;
    return j1->a - j2->a;
}

// Ham so sanh de sap xep nhom N2 giam theo thoi gian tren may B
int cmpN2(const void *p1, const void *p2)
{
    Job *j1 = (Job *)p1;
    Job *j2 = (Job *)p2;
    return j2->b - j1->b;
}

int main()
{
    FILE *f = fopen("johnsond.txt", "r");
    if (f == NULL)
    {
        printf("Khong the mo file johnsona.txt\n");
        return 1;
    }

    int n;
    fscanf(f, "%d", &n);
    Job *jobs = (Job *)malloc(n * sizeof(Job));

    // Doc thoi gian gia cong tren may A
    for (int i = 0; i < n; i++)
    {
        fscanf(f, "%d", &jobs[i].a);
    }
    // Doc thoi gian gia cong tren may B va gan id cho moi chi tiet
    for (int i = 0; i < n; i++)
    {
        fscanf(f, "%d", &jobs[i].b);
        jobs[i].id = i + 1;
    }
    fclose(f);

    // Buoc 1: Chia cac chi tiet thanh 2 nhom
    // Nhom N1 gom cac chi tiet thoa man: a <= b
    // Nhom N2 gom cac chi tiet thoa man: a > b
    Job *N1 = (Job *)malloc(n * sizeof(Job));
    Job *N2 = (Job *)malloc(n * sizeof(Job));
    int countN1 = 0, countN2 = 0;

    for (int i = 0; i < n; i++)
    {
        if (jobs[i].a <= jobs[i].b)
        {
            N1[countN1++] = jobs[i];
        }
        else
        {
            N2[countN2++] = jobs[i];
        }
    }

    // Buoc 2:
    // Sap xep cac chi tiet trong N1 tang theo thoi gian tren may A
    qsort(N1, countN1, sizeof(Job), cmpN1);
    // Sap xep cac chi tiet trong N2 giam theo thoi gian tren may B
    qsort(N2, countN2, sizeof(Job), cmpN2);

    // Buoc 3: Noi cac chi tiet cua N2 vao cuoi cac chi tiet cua N1
    Job *L = (Job *)malloc(n * sizeof(Job));
    int idx = 0;
    for (int i = 0; i < countN1; i++)
    {
        L[idx++] = N1[i];
    }
    for (int i = 0; i < countN2; i++)
    {
        L[idx++] = N2[i];
    }

    // In cac buoc thuc hien
    printf("Buoc 1:\n");
    printf("N1 = { ");
    for (int i = 0; i < countN1; i++)
    {
        printf("D%d ", N1[i].id);
    }
    printf("}\n");

    printf("N2 = { ");
    for (int i = 0; i < countN2; i++)
    {
        printf("D%d ", N2[i].id);
    }
    printf("}\n");

    printf("Buoc 2:\n");
    printf("N1 = { ");
    for (int i = 0; i < countN1; i++)
    {
        printf("D%d ", N1[i].id);
    }
    printf("}\n");

    printf("N2 = { ");
    for (int i = 0; i < countN2; i++)
    {
        printf("D%d ", N2[i].id);
    }
    printf("}\n");

    printf("Buoc 3:\n");
    printf("L* = { ");
    for (int i = 0; i < n; i++)
    {
        printf("D%d ", L[i].id);
    }
    printf("}\n");

    // Tinh thoi gian hoan thanh gia cong
    int timeA = 0, timeB = 0;
    for (int i = 0; i < n; i++)
    {
        timeA += L[i].a; // may A bat dau ngay va cong don thoi gian
        if (timeB < timeA)
            timeB = timeA; // may B bat dau khi may A hoan thanh cong viec hien tai
        timeB += L[i].b;
    }

    printf("Thoi gian may A hoan thanh: %d\n", timeA);
    printf("Thoi gian may B hoan thanh: %d\n", timeB);

    // Giai phong bo nho
    free(jobs);
    free(N1);
    free(N2);
    free(L);

    return 0;
}
