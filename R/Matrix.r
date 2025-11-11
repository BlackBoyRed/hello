# Create a 3x3 Matrix
mat_A <- matrix(1:9, nrow = 3, ncol = 3, byrow = TRUE)
mat_B <- matrix(rep(2, 9), nrow = 3, ncol = 3)

print("Matrix A:")
print(mat_A)

# Matrix Addition
print("Matrix Addition (A + B):")
print(mat_A + mat_B)

# Element-wise Multiplication
print("Element-wise Multiplication:")
print(mat_A * mat_B)

# Matrix Multiplication (Dot Product)
print("Matrix Multiplication (A %*% B):")
print(mat_A %*% mat_B)

# Transpose
print("Transpose of A:")
print(t(mat_A))
