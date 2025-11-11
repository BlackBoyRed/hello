# --- 1. If, Else If, and Else Statement ---
# Used for making decisions
x <- -5

print(paste("The value of x is:", x))

if (x > 0) {
  print("x is a positive number")
} else if (x < 0) {
  print("x is a negative number")
} else {
  print("x is zero")
}

# --- 2. For Loop ---
# Used for iterating over a sequence
print("Running a FOR loop to print squares from 1 to 5:")
for (i in 1:5) {
  # 'i' will take the value 1, then 2, then 3, etc.
  square <- i * i
  print(paste("The square of", i, "is", square))
}

# --- 3. While Loop ---
# Used when the number of iterations is unknown
print("Running a WHILE loop to count up to 3:")
count <- 1
while (count <= 3) {
  print(paste("Current count is:", count))
  
  # Important: Increment the counter, or it will be an infinite loop!
  count <- count + 1
}
