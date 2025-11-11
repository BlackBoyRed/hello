# --- Vector Operations ---
num_vector <- c(10, 20, 30, 40, 50)
print(num_vector[3])        # Access 3rd element
print(num_vector[1:3])      # Slicing
print(length(num_vector))   # Length of vector

# --- List Operations ---
my_list <- list(
  name = "Student A",
  age = 21,
  marks = c(85, 90, 88)
)

print(my_list$name)   # Access by name
print(my_list[[3]])   # Access by index
