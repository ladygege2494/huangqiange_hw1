#include "algebra.h"
#include <stdio.h>
#include <math.h>

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    if (a.rows != b.rows || a.cols != b.cols)
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    return result;
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    if (a.rows != b.rows || a.cols != b.cols)
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    return result;
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    if (a.cols != b.rows || a.rows != b.cols)
    {
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, b.cols);
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < b.cols; j++)
        {
            result.data[i][j] = 0;
            for (int k = 0; k < a.cols; k++)
            {
                result.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    return result;
}

Matrix scale_matrix(Matrix a, double k)
{
    Matrix result = create_matrix(a.rows, a.cols);
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] * k;
        }
    }
    return result;
}

Matrix transpose_matrix(Matrix a)
{
    if (a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.cols, a.rows);
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            result.data[j][i] = a.data[i][j];
        }
    }
    return result;
}

double det_matrix(Matrix a)
{
    if (a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    if (a.rows == 1)
    {
        return a.data[0][0];
    }
    if (a.rows == 2)
    {
        return a.data[0][0] * a.data[1][1] - a.data[0][1] * a.data[1][0];
    }
    double det = 0;
    for (int p = 0; p < a.cols; p++)
    {
        Matrix sub = create_matrix(a.rows - 1, a.cols - 1);
        //求删去第一行第p列后的子矩阵
        for (int i = 1; i < a.rows; i++) //按第一行展开，i从第二行开始
        {
            int colIndex = 0;
            for (int j = 0; j < a.cols; j++)
            {
                if (j == p)
                {
                    continue;
                }
                sub.data[i - 1][colIndex++]=a.data[i][j];
            }
        }
        det += pow(-1, p+1) * a.data[0][p] * det_matrix(sub); // 每次算都按第一行展开
    }
    return det;
}

Matrix inv_matrix(Matrix a)
{
    if (a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    double det = det_matrix(a);
    if (det == 0)
    {
        printf("Error: The matrix is singular.\n");
        return create_matrix(0, 0);
    }
    Matrix result = create_matrix(a.rows, a.cols);

    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            Matrix sub = create_matrix(a.rows - 1, a.cols - 1);
            // 求删去第i行第j列后的子矩阵
            for (int k = 0; k < a.rows; k++)
            {
                if (k == i)
                {
                    continue;
                }
                for (int l = 0, colIndex = 0; l < a.cols; l++)
                {
                    if (l == j)
                    {
                        continue;
                    }
                    sub.data[k < i ? k : k - 1][colIndex++] = a.data[k][l]; // 特别注意若当前行大于所删行，索引值要-1
                }
            }
            result.data[j][i] = pow(-1, i + j) * det_matrix(sub) / det;
        }
    }
    return result;
}

int rank_matrix(Matrix a)
{
    int rank = a.rows < a.cols ? a.rows : a.cols; // 取行数和列数的最小值

    Matrix temp = create_matrix(a.rows, a.cols); // 创建一个临时矩阵用于操作，避免修改原矩阵
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            temp.data[i][j] = a.data[i][j];
        }
    }

    for (int j = 0; j < a.cols; j++) // 遍历每一列
    {
        int i;
        for (i = j; i < a.rows; i++) // 从第j行开始寻找对角线上非零元素
        {
            if (temp.data[i][j] != 0)
            {
                break;
            }
        }
        if (i == a.rows) // 在下方找不到非零元素，说明当前列已经全为零
        {
            rank--;
            continue;
        }
        if (i != j) // a_ii=0，需要在下方第j行寻找一个非零元素，并将其与当前行交换
        {
            for (int k = 0; k < a.cols; k++)
            {
                double t = temp.data[j][k];
                temp.data[j][k] = temp.data[i][k];
                temp.data[i][k] = t;
            }
        }
        for (int i = j + 1; i < a.rows; i++) // 行运算下方消成0
        {
            double t = temp.data[i][j] / temp.data[j][j];
            for (int k = 0; k < a.cols; k++)
            {
                temp.data[i][k] -= t * temp.data[j][k];
            }
        }
    }
    return rank;
}

double trace_matrix(Matrix a)
{
    if (a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    double trace = 0.0;
    for (int i = 0; i < a.rows; i++)
    {
        trace += a.data[i][i];
    }
    return trace;
}

void print_matrix(Matrix a)
{
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}