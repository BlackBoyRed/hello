install.packages("shiny")
install.packages("shinydashboard")

# Load the required libraries
library(shiny)
library(shinydashboard)

# ------------------------------------------------------------------
# Define the User Interface (UI) - What the user sees
# ------------------------------------------------------------------
ui <- dashboardPage(
  
  # --- 1. Header ---
  # This is the top blue bar
  dashboardHeader(title = "My First Dashboard"),
  
  # --- 2. Sidebar ---
  # This is the collapsible menu on the left
  dashboardSidebar(
    sidebarMenu(
      # We add our inputs here
      sliderInput(
        inputId = "bins",                 # The ID for this input (used by server)
        label = "Number of histogram bins:", # The text label
        min = 5,                          # Minimum value of the slider
        max = 50,                         # Maximum value of the slider
        value = 30                        # Default value
      )
    )
  ),
  
  # --- 3. Body ---
  # This is the main white area where outputs are shown
  dashboardBody(
    # We use fluidRow to create a row-based layout
    fluidRow(
      
      # A "box" is a container for content
      box(
        title = "Histogram of Geyser Eruptions",
        status = "primary",  # This gives it a blue header
        solidHeader = TRUE,
        collapsible = TRUE,  # Adds a minimize button
        
        # plotOutput tells Shiny where to put our plot
        # "distPlot" is the ID we will use in the server
        plotOutput(outputId = "distPlot")
      )
    )
  )
)

# ------------------------------------------------------------------
# Define the Server Logic - What the computer does
# ------------------------------------------------------------------
server <- function(input, output) {
  
  # This block of code creates the histogram
  # It is a "reactive" function, so it re-runs
  # every time an input it uses (like input$bins) changes.
  
  output$distPlot <- renderPlot({
    
    # 1. Get data
    x    <- faithful$waiting  # 'faithful' is a built-in dataset
    
    # 2. Get the number of bins from the UI's slider input
    # We access it using input$<inputId>
    bins <- seq(min(x), max(x), length.out = input$bins + 1)
    
    # 3. Draw the histogram
    hist(x, breaks = bins, col = "#0073b7", border = 'white',
         xlab = 'Waiting time to next eruption (in mins)',
         main = 'Histogram of waiting times')
  })
  
}

# ------------------------------------------------------------------
# Run the Application
# ------------------------------------------------------------------
shinyApp(ui = ui, server = server)
