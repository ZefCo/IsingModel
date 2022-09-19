rm(list=ls())

library(tseries)
library(ggplot2)

auto_core <- function(inseries) {
  x <- c()
  
  tmax <- length(inseries)
  
  for (t in 1:(tmax - 1)) {
    alpha <- 1 / (tmax - t)
    
    term1 <- 0
    term2 <- 0
    term3 <- 0
    
    for (tp in 1: (tmax - t)) {
      term1 <- term1 + (inseries[tp] * inseries[tp + t])
      term2 <- term2 + (inseries[tp])
      term3 <- term3 + (inseries[tp + t])
    }
    
    x[t] <- (alpha*term1) - ((alpha*term2)*(alpha*term3))
    
    if (x[t] < 0) {break}
  }
  
  return(x)
  
}

# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles\\2_4K"
rootfolder <- list.files(path = rootpath)

# Hold data for Temp Stuff, going to make plots from these later
temp_frame <- data.frame()
temp_names <- c()

xframe = data.frame()

lag <- 100000
master_frame <- data.frame()
for (i in 1:length(rootfolder)) {
  # Iterate through the folder and put all the data into the master frame
  filepath <- paste(rootpath, rootfolder[i], sep = "\\")
    
  local_frame <- read.csv(filepath, header = TRUE)
  master_frame <- rbind(master_frame, local_frame)

  # # x <- acf(master_frame["m"], type = "correlation", lag.max = lag, plot = T)
  # # xpoints <- unlist(x$acf)
  # # 
  # xnames[i] <- rootfolder[i]
  # 
  # x <- auto_core(master_frame$m)
  # xframe <- rbind(xframe, x)
  
}


x <- auto_core(master_frame$m)
t <- rep(1:length(x) - 1)
xdata <- data.frame(x = t, y = x)


# xframe <- t(xframe)
# 
# colnames(xframe) <- xnames
# rownames(xframe) <- t
# 
# 
hail_mary <- ggplot(data = xdata, aes(x = t, y = y)) + geom_line()
print(hail_mary)
