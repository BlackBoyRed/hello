R Programming: List of PracticalsThis document contains a series of practical exercises designed to introduce fundamental concepts and operations in the R programming language. Each practical includes an aim, a theoretical background, a complete R program, the expected output, and a summary.Practical 1: Study of basic Syntaxes in RAim: To understand and implement the basic syntax, including variable assignment, data types, and comments in R.Theory:R is a programming language widely used for statistical computing and graphics. The basic syntax involves assigning values to variables using the <- or = operator. Comments are written using the # symbol, which tells the R interpreter to ignore the rest of the line. R supports various data types, including numeric (e.g., 10.5), integer (e.g., 10L), character (e.g., "hello"), and logical (e.g., TRUE or FALSE).Program:# --- Basic Variable Assignment ---
# Numeric variable
x <- 10.5
print(x)

# Character variable (string)
my_string <- "Hello, R!"
print(my_string)

# Logical variable
is_true <- TRUE
print(is_true)

# --- Getting the Class (Data Type) of a Variable ---
cat("The data type of x is:", class(x), "\n")
cat("The data type of my_string is:", class(my_string), "\n")
cat("The data type of is_true is:", class(is_true), "\n")
Output:[1] 10.5
[1] "Hello, R!"
[1] TRUE
The data type of x is: numeric 
The data type of my_string is: character 
The data type of is_true is: logical 
Summary:This practical demonstrated the fundamental syntax of R. We learned how to assign values of different data types (numeric, character, logical) to variables using the <- operator and how to use comments for code documentation. We also used the print(), cat(), and class() functions to display variables and check their data types.Practical 2: To perform the basic mathematical operations in R programmingAim: To use R as a calculator to perform basic arithmetic operations.Theory:R can perform a wide range of mathematical operations. Standard arithmetic operators are available for addition (+), subtraction (-), multiplication (*), division (/), exponentiation (^ or **), and finding the modulus or remainder (%%).Program:# Assign two numbers to variables
a <- 20
b <- 5

# --- Performing Mathematical Operations ---

# Addition
addition_result <- a + b
cat("Addition (a + b):", addition_result, "\n")

# Subtraction
subtraction_result <- a - b
cat("Subtraction (a - b):", subtraction_result, "\n")

# Multiplication
multiplication_result <- a * b
cat("Multiplication (a * b):", multiplication_result, "\n")

# Division
division_result <- a / b
cat("Division (a / b):", division_result, "\n")

# Exponentiation
exponent_result <- a ^ 2
cat("Exponentiation (a^2):", exponent_result, "\n")

# Modulus (Remainder)
modulus_result <- a %% 3
cat("Modulus (a %% 3):", modulus_result, "\n")
Output:Addition (a + b): 25 
Subtraction (a - b): 15 
Multiplication (a * b): 100 
Division (a / b): 4 
Exponentiation (a^2): 400 
Modulus (a %% 3): 2 
Summary:In this practical, we explored R's capability to perform fundamental mathematical calculations. We used standard arithmetic operators to perform addition, subtraction, multiplication, division, exponentiation, and modulus operations on numeric variables, demonstrating that R can function effectively as a powerful calculator.Practical 3: Implementation of vector and List data objects operationsAim: To create, access, and manipulate vectors and lists in R.Theory:Vectors are one-dimensional arrays that can hold elements of the same data type. They are created using the c() function. Elements can be accessed using their index in square brackets [] (indexing starts from 1).Lists are generic objects that can contain elements of different data types (e.g., numbers, strings, vectors, or even another list). They are created with the list() function. Elements can be accessed by their index using [[]] or by their name using the $ operator.Program:# --- Vector Operations ---
cat("--- Vector Operations ---\n")
# Create a numeric vector
num_vector <- c(10, 20, 30, 40, 50)
print(num_vector)

# Access the third element
cat("Third element:", num_vector[3], "\n")

# Slice the vector to get elements from 2 to 4
cat("Sliced vector (2 to 4):", num_vector[2:4], "\n")

# --- List Operations ---
cat("\n--- List Operations ---\n")
# Create a list with different data types
my_list <- list(name = "John", age = 25, scores = c(88, 92, 95), is_student = TRUE)
print(my_list)

# Access the 'age' element by name
cat("\nAge:", my_list$age, "\n")

# Access the 'scores' vector using double brackets
cat("Scores:", my_list[[3]], "\n")
Output:--- Vector Operations ---
[1] 10 20 30 40 50
Third element: 30 
Sliced vector (2 to 4): 20 30 40 

--- List Operations ---
$name
[1] "John"

$age
[1] 25

$scores
[1] 88 92 95

$is_student
[1] TRUE

Age: 25 
Scores: 88 92 95 
Summary:This practical covered two fundamental R data structures: vectors and lists. We learned that vectors store elements of the same type, while lists can store a mix of different types. We practiced creating these objects, accessing specific elements and subsets, and understanding the key differences in their structure and application.Practical 4: Implementation of various operations on matrix in RAim: To create matrices and perform basic matrix operations.Theory:A matrix is a two-dimensional data structure where all elements must have the same data type. It is created with the matrix() function, specifying the data, number of rows (nrow), and number of columns (ncol). Basic arithmetic operations like addition and subtraction can be performed element-wise. Matrix multiplication requires the %*% operator.Program:# Create two matrices
A <- matrix(data = 1:6, nrow = 2, ncol = 3)
B <- matrix(data = 7:12, nrow = 2, ncol = 3)

cat("Matrix A:\n")
print(A)

cat("\nMatrix B:\n")
print(B)

# Matrix Addition
cat("\nMatrix Addition (A + B):\n")
print(A + B)

# Transpose of a Matrix
C <- matrix(data = 1:6, nrow = 3, ncol = 2)
cat("\nMatrix C (for multiplication):\n")
print(C)

# Matrix Multiplication (requires compatible dimensions: 2x3 %*% 3x2)
cat("\nMatrix Multiplication (A %*% C):\n")
print(A %*% C)
Output:Matrix A:
     [,1] [,2] [,3]
[1,]    1    3    5
[2,]    2    4    6

Matrix B:
     [,1] [,2] [,3]
[1,]    7    9   11
[2,]    8   10   12

Matrix Addition (A + B):
     [,1] [,2] [,3]
[1,]    8   12   16
[2,]   10   14   18

Matrix C (for multiplication):
     [,1] [,2]
[1,]    1    4
[2,]    2    5
[3,]    3    6

Matrix Multiplication (A %*% C):
     [,1] [,2]
[1,]   22   49
[2,]   28   64
Summary:This practical focused on the matrix data structure. We learned how to create a matrix using the matrix() function and perform fundamental operations such as element-wise addition and proper matrix multiplication using the %*% operator. We also saw how to create a transpose for ensuring compatible dimensions for multiplication.Practical 5: Create a 3-dimensional array and perform operations like element access and slicingAim: To create and manipulate a 3-dimensional array.Theory:An array in R can store data in more than two dimensions. They are created using the array() function, which takes a vector of data and a dim attribute specifying the dimensions. For a 3D array, dim would be a vector of three numbers (e.g., c(rows, columns, tables)). Elements are accessed by specifying their index in each dimension, separated by commas.Program:# Create a vector of data
data_vector <- 1:24

# Create a 3-dimensional array (3 rows, 4 columns, 2 tables/layers)
my_array <- array(data_vector, dim = c(3, 4, 2))

cat("The 3D Array:\n")
print(my_array)

# Access a single element (Row 1, Column 3, Table 2)
element <- my_array[1, 3, 2]
cat("\nElement at [1, 3, 2]:", element, "\n")

# Slice the array to get the entire second table/layer
slice <- my_array[, , 2]
cat("\nSecond table/layer of the array:\n")
print(slice)
Output:The 3D Array:
, , 1

     [,1] [,2] [,3] [,4]
[1,]    1    4    7   10
[2,]    2    5    8   11
[3,]    3    6    9   12

, , 2

     [,1] [,2] [,3] [,4]
[1,]   13   16   19   22
[2,]   14   17   20   23
[3,]   15   18   21   24


Element at [1, 3, 2]: 19 

Second table/layer of the array:
     [,1] [,2] [,3] [,4]
[1,]   13   16   19   22
[2,]   14   17   20   23
[3,]   15   18   21   24
Summary:In this practical, we extended our knowledge from 2D matrices to 3D arrays. We learned to create a multi-dimensional array using the array() function and its dim attribute. We practiced how to access a single specific element and how to slice the array to extract an entire 2D matrix from it.Practical 6: Create a factor variable from categorical data and perform level manipulation.Aim: To understand how to create and manage factors for categorical data.Theory:Factors are used in R to represent categorical data. They are stored as a vector of integer values with a corresponding set of character values used for labeling. The factor() function is used to create a factor. The unique values in a factor are called "levels." The levels() function can be used to view or change the order of these levels.Program:# Create a character vector of categorical data
directions <- c("East", "West", "North", "South", "East", "North")

# Create a factor from the vector
factor_directions <- factor(directions)
cat("The factor:\n")
print(factor_directions)
cat("\nLevels of the factor:\n")
print(levels(factor_directions))

# Manipulating levels by setting a specific order
ordered_levels <- c("North", "South", "East", "West")
factor_directions_ordered <- factor(factor_directions, levels = ordered_levels)
cat("\nFactor with reordered levels:\n")
print(factor_directions_ordered)
cat("\nNew levels:\n")
print(levels(factor_directions_ordered))
Output:The factor:
[1] East  West  North South East  North
Levels: East North South West

Levels of the factor:
[1] "East"  "North" "South" "West" 

Factor with reordered levels:
[1] East  West  North South East  North
Levels: North South East West

New levels:
[1] "North" "South" "East"  "West" 
Summary:This practical introduced factors, R's primary data structure for handling categorical data. We learned how to convert a character vector into a factor and how to view its levels. Most importantly, we demonstrated how to manipulate the order of the levels, which is crucial for statistical modeling and creating well-organized plots.Practical 7: Implementation and perform the various operations on data frames in RAim: To create a data frame and perform common operations like accessing columns, adding new columns, and summarizing the data.Theory:A data frame is the most common way of storing and working with tabular data in R. It is a list of vectors of equal length, making it a 2-dimensional, table-like structure. Columns can be of different data types. Data frames are created with data.frame(). Columns can be accessed with the $ operator or [] notation. Functions like str() and summary() are used to inspect the structure and get statistical summaries of the data.Program:# Create a data frame
employee_data <- data.frame(
  ID = 1:4,
  Name = c("John", "Anna", "Peter", "Linda"),
  Department = c("Sales", "HR", "Tech", "Sales"),
  Salary = c(50000, 60000, 75000, 52000)
)

cat("--- The Original Data Frame ---\n")
print(employee_data)

# Get a summary of the data frame
cat("\n--- Summary of the Data Frame ---\n")
summary(employee_data)

# Access a specific column (e.g., Name)
cat("\n--- Accessing the 'Name' Column ---\n")
print(employee_data$Name)

# Add a new column 'YearsOfService'
employee_data$YearsOfService <- c(3, 5, 8, 4)

cat("\n--- Data Frame with New Column ---\n")
print(employee_data)
Output:--- The Original Data Frame ---
  ID  Name Department Salary
1  1  John      Sales  50000
2  2  Anna         HR  60000
3  3 Peter       Tech  75000
4  4 Linda      Sales  52000

--- Summary of the Data Frame ---
     ID            Name            Department           Salary      
 Min.   :1.00   Length:4           Length:4           Min.   :50000  
 1st Qu.:1.75   Class :character   Class :character   1st Qu.:51500  
 Median :2.50   Mode  :character   Mode  :character   Median :56000  
 Mean   :2.50                                         Mean   :59250  
 3rd Qu.:3.25                                         3rd Qu.:63750  
 Max.   :4.00                                         Max.   :75000  

--- Accessing the 'Name' Column ---
[1] "John"  "Anna"  "Peter" "Linda"

--- Data Frame with New Column ---
  ID  Name Department Salary YearsOfService
1  1  John      Sales  50000              3
2  2  Anna         HR  60000              5
3  3 Peter       Tech  75000              8
4  4 Linda      Sales  52000              4
Summary:This practical focused on the data frame, the most important data structure for data analysis in R. We learned how to create a data frame, inspect its structure and contents using summary(), access individual columns, and add new columns. This forms the basis for most data manipulation tasks in R.Practical 8: Study and implementation of various control structures in RAim: To implement control flow structures, including if-else statements and for loops.Theory:Control structures allow you to control the flow of execution in a program.if-else Statement: Executes a block of code if a specified condition is true, and an optional else block if the condition is false.for Loop: Repeats a block of code for each item in a sequence (like a vector).while Loop: Repeats a block of code as long as a specified condition is true.Program:# --- if-else Statement ---
cat("--- if-else Example ---\n")
x <- 15
if (x > 10) {
  cat("x is greater than 10\n")
} else {
  cat("x is not greater than 10\n")
}

# --- for Loop ---
cat("\n--- for Loop Example ---\n")
# Loop through a vector of numbers
for (i in 1:5) {
  cat("Current number is:", i, "\n")
}

# --- while Loop ---
cat("\n--- while Loop Example ---\n")
count <- 1
while (count <= 5) {
  cat("Count is:", count, "\n")
  count <- count + 1 # Increment the counter
}
Output:--- if-else Example ---
x is greater than 10

--- for Loop Example ---
Current number is: 1 
Current number is: 2 
Current number is: 3 
Current number is: 4 
Current number is: 5 

--- while Loop Example ---
Count is: 1 
Count is: 2 
Count is: 3 
Count is: 4 
Count is: 5 
Summary:In this practical, we explored the essential control structures that direct the flow of an R script. We implemented an if-else statement for conditional logic, a for loop to iterate over a sequence, and a while loop to repeat actions based on a condition. Mastering these structures is key to writing dynamic and complex programs.
