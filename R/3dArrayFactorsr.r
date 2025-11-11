# --- 3D Array ---
# Create an array of dimension 2x3x2
my_array <- array(1:12, dim = c(2, 3, 2))
print("Element at (Row 1, Col 2, Layer 1):")
print(my_array[1, 2, 1])

# --- Factors ---
# Categorical data
gender_vector <- c("Male", "Female", "Female", "Male", "Male")
gender_factor <- factor(gender_vector)

print("Factor Levels:")
print(levels(gender_factor))

print("Table count:")
print(table(gender_factor))
