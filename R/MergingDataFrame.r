# --- 1. Combining (Stacking) with rbind ---
# Use this when the data frames have the same columns.

# Data for Quarter 1
q1_sales <- data.frame(
  Employee = c("Alice", "Bob"),
  Sales = c(150, 200)
)
# Data for Quarter 2
q2_sales <- data.frame(
  Employee = c("Alice", "Bob"),
  Sales = c(180, 220)
)

# Stack them
all_sales <- rbind(q1_sales, q2_sales)
print("--- Combined (rbind) Data ---")
print(all_sales)


# --- 2. Merging (Joining) with merge ---
# Use this to join data frames by a common ID column.

# Table 1: Employee Names
employees <- data.frame(
  EmpID = c(101, 102, 103),
  Name = c("Alice", "Bob", "Charlie")
)
# Table 2: Employee Departments
departments <- data.frame(
  EmpID = c(101, 102, 104),
  Dept = c("Sales", "Marketing", "Finance")
)

# Inner Join (default): Keeps only rows that match in BOTH tables
inner_join <- merge(employees, departments, by = "EmpID")
print("--- Merged (Inner Join) Data ---")
print(inner_join)

# Outer Join: Keeps ALL rows from BOTH tables, fills missing with NA
outer_join <- merge(employees, departments, by = "EmpID", all = TRUE)
print("--- Merged (Outer Join) Data ---")
print(outer_join)
