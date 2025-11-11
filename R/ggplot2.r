# Load the library (you must run install.packages("ggplot2") once)
library(ggplot2)

# We will use the built-in 'mpg' dataset
data("mpg")

# --- 1. Scatter Plot ---
# Shows the relationship between two continuous variables
# (Engine Displacement vs. Highway MPG)

plot1 <- ggplot(mpg, aes(x = displ, y = hwy)) +
  geom_point(aes(color = class)) + # Add points, color them by car class
  geom_smooth(method = "lm") +    # Add a "line of best fit"
  labs(
    title = "Engine Size vs. Highway MPG",
    x = "Engine Displacement (Liters)",
    y = "Highway Miles Per Gallon",
    color = "Car Class"
  ) +
  theme_minimal()

print("Displaying Scatter Plot...")
print(plot1)


# --- 2. Bar Plot ---
# Shows the count of a categorical variable (Car Manufacturer)

plot2 <- ggplot(mpg, aes(x = manufacturer)) +
  geom_bar(fill = "steelblue") +
  labs(
    title = "Number of Cars by Manufacturer",
    x = "Manufacturer",
    y = "Count"
  ) +
  # Rotate x-axis labels to prevent overlap
  theme(axis.text.x = element_text(angle = 65, vjust = 0.6))

print("Displaying Bar Plot...")
print(plot2)


# --- 3. Box Plot ---
# Shows the distribution of a continuous variable (Highway MPG)
# grouped by a categorical variable (Car Class).

plot3 <- ggplot(mpg, aes(x = class, y = hwy)) +
  geom_boxplot(aes(fill = class)) +
  labs(
    title = "Distribution of Highway MPG by Car Class",
    x = "Car Class",
    y = "Highway Miles Per Gallon"
  ) +
  theme_minimal() +
  theme(legend.position = "none") # Hide legend (it's redundant)

print("Displaying Box Plot...")
print(plot3)
