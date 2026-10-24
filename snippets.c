#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int relu(int a) { return (a > 0) ? a : 0; }
void nwln() { printf("\n"); }

void print_arr(const int *arr, size_t size) {
  printf("[");
  for (size_t i = 0; i < size; i++) {
    printf("%d%s", arr[i], (i == size - 1) ? "" : ", ");
  }
  printf("]\n");
}

void populate_arr(int *arr, size_t size, char *varname) {
  for (int i = 0; i < size; i++) {
    printf("input %s[%d] = ", varname, i);
    scanf("%d", &arr[i]);
  }
}

int **allocate_matrix(int rows, int cols) {
  int **mat = malloc(rows * sizeof(int *));
  if (mat == NULL) {
    return NULL; // Allocation failed
  }

  for (int i = 0; i < rows; i++) {
    mat[i] = calloc(cols, sizeof(int));
    if (mat[i] == NULL) {
      // Rollback: free already allocated rows to prevent memory leaks
      for (int j = 0; j < i; j++) {
        free(mat[j]);
      }
      free(mat);
      return NULL;
    }
  }
  return mat;
}

void populate_matrix(int **A, int m, int n, char *varname) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("input %s[%d][%d] = ", varname, i, j);
      scanf("%d", &A[i][j]);
    }
  }
}

void print_matrix(int **mat, int m, int n) {
  printf("[ ");

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", mat[i][j]);
    }
    if (i != m - 1) {
      printf("\n  ");
    }
  }

  printf(" ]\n");
}

void slice_matrix(int **slice, int **A, int xs, int xe, int ys, int ye) {
  for (int i = xs; i < xe; i++) {
    for (int j = ys; j < ye; j++) {
      slice[i - xs][j - ys] = A[i][j];
    }
  }
}

void transpose_matrix(int **destmat, int **srcmat, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      destmat[j][i] = srcmat[i][j];
    }
  }
}

void mul_matrix(int **destmat, int **A, int **B, int m, int n, int w) {
  int tmp = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < w; j++) {
      for (int k = 0; k < n; k++) {
        tmp += A[i][k] * B[k][j];
      }
      destmat[i][j] = tmp;
      tmp = 0;
    }
  }
}

int cmp_matrix(int **A, int **B, int m, int n) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (A[i][j] != B[i][j]) {
        return 0;
      }
    }
  }
  return 1;
}

void free_matrix(int **A, int m) {
  for (int i = 0; i < m; i++) {
    free(A[i]);
  }
  free(A);
}

int **get_minor(int **minor, int **src, int n, int excludei, int excludej) {
  // int **minor = allocate_matrix(n - 1, n - 1);
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1; j++) {
      minor[i][j] =
          src[(i >= excludei) ? i + 1 : i][(j >= excludej) ? j + 1 : j];
    }
  }
  return minor;
}

int determinant(int **A, int n) {

  if (n == 1) {
    return A[0][0];
  }

  int det = 0;
  int cof = 1;
  int **minor = allocate_matrix(n - 1, n - 1);

  for (int i = 0; i < n; i++) {
    get_minor(minor, A, n, i, 0);
    det += cof * A[i][0] * determinant(minor, n - 1);
    cof *= -1;
  }

  free_matrix(minor, n - 1);
  return det;
}
