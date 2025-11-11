set.seed(123) # Ensure reproducibility

# Create dummy data
student_data <- data.frame(
  RollNo = 1:10,
  Maths = sample(50:100, 10, replace = TRUE),
  Science = sample(50:100, 10, replace = TRUE)
)

# Manipulation: Add a new column 'Total'
student_data$Total <- student_data$Maths + student_data$Science

# Manipulation: Add a 'Result' column based on condition
student_data$Result <- ifelse(student_data$Total > 150, "Pass", "Fail")

print(student_data)
