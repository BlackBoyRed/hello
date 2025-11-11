# Create a Data Frame
df <- data.frame(
  ID = 1:4,
  Name = c("Alice", "Bob", "Charlie", "David"),
  Salary = c(50000, 60000, 55000, 65000)
)

# View structure and summary
str(df)
summary(df)

# Accessing specific columns
print(df$Name)

# Subsetting rows (Salary > 55000)
high_salary <- df[df$Salary > 55000, ]
print("Employees with Salary > 55000:")
print(high_salary)
