# Load the library (you must run install.packages("dplyr") once)
library(dplyr)

# We will use the built-in 'iris' dataset
data("iris")

print("--- Original Data (first 6 rows) ---")
print(head(iris))

# Chain operations together using the pipe operator %>%

# Task: Find the average petal length and width for the 'setosa' species,
# sorted by petal length.

analysis_result <- iris %>%
  
  # 1. Filter to keep only the "setosa" species
  filter(Species == "setosa") %>%
  
  # 2. Select only the columns we care about
  select(Petal.Length, Petal.Width) %>%
  
  # 3. Mutate to create a new column
  mutate(Petal.Area = Petal.Length * Petal.Width) %>%
  
  # 4. Arrange in descending order of the new column
  arrange(desc(Petal.Area))

print("--- Filtered, Selected, Mutated, and Arranged Data ---")
print(head(analysis_result))


# Task 2: Get a summary (average) for ALL species
species_summary <- iris %>%
  group_by(Species) %>%
  summarise(
    Avg.Sepal.Length = mean(Sepal.Length),
    Avg.Sepal.Width = mean(Sepal.Width),
    Count = n()  # 'n()' is a helper that counts rows
  )

print("--- Summarised Data by Species ---")
print(species_summary)
  
