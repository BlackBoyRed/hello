# --- Part 1: CSV File Operations ---

# 1. Create a sample data frame
student_data <- data.frame(
  id = c("S01", "S02", "S03", "S04"),
  name = c("Anna", "Ben", "Clara", "David"),
  score = c(85, 92, 78, 88)
)

# 2. Write the data frame to a CSV file in your working directory
write.csv(student_data, "students.csv", row.names = FALSE)
print("File 'students.csv' has been created.")

# 3. Read the data back from the CSV file
my_data_from_csv <- read.csv("students.csv")

# 4. Analyze the data
print("--- Data read from students.csv ---")
print(my_data_from_csv)

print("--- Summary of the data ---")
print(summary(my_data_from_csv))


# --- Part 2: Excel File Operations ---
# This requires the 'readxl' package. Run install.packages("readxl") once.
# library(readxl)

# This part requires you to have an Excel file (e.g., "students.xlsx")
# in your working directory.

# Example code (will error if the file doesn't exist):
#
# my_data_from_excel <- read_excel("students.xlsx")
# print("--- Data read from Excel file ---")
# print(my_data_from_excel)
# print(summary(my_data_from_excel))

print("CSV operations are complete. Excel code is for demonstration.")
