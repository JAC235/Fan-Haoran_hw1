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
    if(a.cols == b.cols && a.rows == b.rows)
    {
        int row,col;
        int i,j;
        row = a.rows;
        col = a.cols;
        Matrix c = create_matrix(row,col);
        for(i=0; i<row; i++)
        {
            for (j=0; j<col; j++)
            {
                c.data[i][j] = a.data[i][j] + b.data[i][j];
            }
            
        }
        return c;
    }
    else
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
    
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    if(a.cols == b.cols && a.rows == b.rows)
    {
        int row,col;
        int i,j;
        row = a.rows;
        col = a.cols;
        Matrix c = create_matrix(row,col);
        for(i=0; i<row; i++)
        {
            for (j=0; j<col; j++)
            {
                c.data[i][j] = a.data[i][j] - b.data[i][j];
            }
            
        }
        return c;
    }
    else
    {
        printf("Error: Matrix a and b must have the same rows and cols.\n");
        return create_matrix(0, 0);
    }
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    if(a.cols == b.rows)
    {
        int row,col;
        int i,j,n;
        double sum;
        row = a.rows;
        col = b.cols;
        Matrix c = create_matrix(row,col);
        for(i=0; i<row; i++)
        {
            for (j=0; j<col; j++)
            {
                sum=0;
                for(n=0; n<a.cols; n++)
                {
                    sum += a.data[i][n] * b.data[n][j];    
                }
                c.data[i][j] = sum;
 
            }
            
        }
        return c;
    }
    else
    {
        printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
        return create_matrix(0, 0);
    }
}

Matrix scale_matrix(Matrix a, double k)
{
    int row,col;
    int i,j;
    row = a.rows;
    col = a.cols;
    Matrix c = create_matrix(row,col);
    for(i=0; i<row; i++)
    {
        for (j=0; j<col; j++)
        {               
        c.data[i][j] = k * a.data[i][j];

       }
            
    }
    return c;
}

Matrix transpose_matrix(Matrix a)
{
    int row,col;
    int i,j;
    col = a.rows;
    row = a.cols;
    Matrix c = create_matrix(row,col);
    for(i=0; i<row; i++)
    {
        for (j=0; j<col; j++)
        {               
        c.data[i][j] = a.data[j][i];

       }
            
    }
    return c;
}

double det_matrix(Matrix a)
{
    if (a.rows == a.cols)
    {
        if (a.rows == 1)
        {
            return a.data[0][0];
        }
        else
        {
            if(a.rows == 2)
            {
                return a.data[0][0] * a.data[1][1] - a.data[0][1] * a.data[1][0];
            }
            else
            {
                int row,col;
                double sum = 0;
                int i,j,n,k;
                row = a.rows - 1;
                col = a.cols - 1;
                Matrix c = create_matrix(row,col);
                for (n = 0; n < row; n++)
                {
                    //求余子式
                    for(i=0; i<row; i++)
                    {
                        for (j=0,k=0; k<col; j++,k++)
                        {
                            if (j == n)
                            {
                                j++;
                            }
                                           
                            c.data[i][j] = a.data[i+1][j];

                        }
                    
                    }
                    //求代数余子式之和
                    sum += (n%2?-1:1) * a.data[0][n] * det_matrix(c);
                }
                
                return sum;
            }
                
        }
        
    }
    else 
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    
}

Matrix inv_matrix(Matrix a)
{
    if(a.rows != a.cols)
    {
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    else if (det_matrix(a) == 0)
    {
        printf("Error: The matrix is singular.\n");
        return create_matrix(0, 0);
    }
    else
    {
        //求余子式矩阵
        int row,col;
        row = a.rows;
        col = a.cols;
        Matrix c = create_matrix(row,col);
        int row1,col2;
        row1 = a.rows - 1;
        col2 = a.cols - 1;
        Matrix d = create_matrix(row1,col2);
        int i,j;
        double det;
        for ( i = 0; i < row; i++)
        {
            for(j = 0 ; j < col; j++)
            {
                d = cof(a,i,j);
                det = det_matrix(d);
                c.data[i][j] = pow(-1,i+j) * det;
            }
        }
        //转置
        c = transpose_matrix(c);
        //求逆
        Matrix e = create_matrix(row,col);
        double m;
        m = 1.00 / det_matrix(a);
        e = scale_matrix(c,m);   
        return e;
    }
    
    
}

int rank_matrix(Matrix a)
{
        int rank;
        int i,j,p;
        if(a.rows > a.cols)
        {
            a = transpose_matrix(a);
        }
        rank = a.rows;
        for( j = 0 ; j < a.rows; j++)
        {
            if(a.data[j][j] == 0)
            {
                p=0;
                for(i = 1; i < a.rows - j; i++)
                {
                    if (a.data[j+i][j] != 0)
                    {
                        a = transrow(a,j,j+i);
                        p = 1;
                        break;
                    }
                    
                }
                if(p == 0)
                {
                    rank--;
                }
            }
            a = gass(a,j);
        }
        return rank;
    
}

double trace_matrix(Matrix a)
{
    if(a.rows == a.cols)
    {
    int i, sum = 0;
        for(i = 0; i < a.rows; i++)
        {
            sum += a.data[i][i];
        }
        return sum;
    }
    else
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
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

Matrix cof(Matrix a, int row1, int col1)
{
    //建立余子式c
    int row,col;
    double sum = 0;
    int i,j,n,k;
    row = a.rows - 1;
    col = a.cols - 1;
    Matrix c = create_matrix(row,col);
    //求余子式
    for(i=0,n=0; i<row; i++,n++)
    {
        if(n == row1)
        {
            n++;
        }
        for (j=0,k=0; j<col; j++,k++)
        {
            if (k == col1)
            {
                k++;
               }
                      
            c.data[i][j] = a.data[n][k];
        }
                    
    }
    return c;
        
}

Matrix transrow(Matrix a,int row1,int row2)
{
    int row,col;
    row = a.rows;
    col = a.cols;
    Matrix c = create_matrix(row,col);
    int i,j;
    for ( i = 0; i < row; i++)
    {
        if ( i == row1 )
        {
            for ( j = 0; j < col; j++)
            {
                c.data[i][j] = a.data[row2][j];
            }
        }
        else if ( i == row2 )
        {
            for ( j = 0; j < col; j++)
            {
                c.data[i][j] = a.data[row1][j];
            }
        } 
        else
        {
            for ( j = 0; j < col; j++)
            {
                c.data[i][j] = a.data[i][j];
            }
        } 
        
    }
    return c;
}

Matrix gass(Matrix a, int m)
{
    int i,j;
    for ( i = m + 1; i < a.rows; i++)
    {
        for ( j = m + 1; j < a.cols ; j++)
        {
            a.data[i][j] = a.data[i][j] - a.data[m][j] * a.data[i][m] / a.data[m][m];
        }
    }
    return a;
}

Matrix del(Matrix a)
{
    int row,col;
    row = a.rows - 1;
    col = a.cols - 1;
    Matrix c = create_matrix(row,col);
    int i,j;
    for ( i = 0; i < row; i++)
    {
        for ( j = 0; j < col; j++)
        {
            c.data[i][j] = a.data[i+1][j+1];
        }
       
    }
    return c;
}