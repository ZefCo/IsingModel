rm(list=ls())

library(tseries)
library(ggplot2)
library(pracma)

auto_core <- function(inseries, tmax = 2000) {
  x <- c()

  # tmax <- length(inseries)
  
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
    
    # if (x[t] < 0) {break}
  }
  
  x0 <- x[[1]]

  for (i in 1:length(x)) {
    x[[i]] <- x[[i]] / x0
  }

  return(x)
  
}

# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles"
rootfolder <- list.files(path = rootpath)

xframe <- data.frame()
xnames <- c()
areas <- list()

for (i in 1:length(rootfolder)) {
  # frame for holding all the data
  master_frame <- data.frame()
  
  subpath <- paste(rootpath, rootfolder[i], sep = "\\")
  subfolder <- list.files(path = subpath)
  
  # Iterate through the folder and put all the data into the master frame
  for (j in 1:length(subfolder)) {
    filepath <- paste(subpath, subfolder[j], sep = "\\")
    
    local_frame <- read.csv(filepath, header = TRUE)
    master_frame <- rbind(master_frame, local_frame)
  }
  
  # x <- acf(master_frame["m"], type = "correlation", lag.max = lag, plot = T)
  # xpoints <- unlist(x$acf)
  # 
  xnames[i] <- paste("T", rootfolder[i], sep = "")
  
  x <- auto_core(master_frame$m)
  t <- rep(1:length(x) - 1)
  
  xframe <- rbind(xframe, x)
  
  areas[rootfolder[[i]]] <- trapz(t, x)
  
}




xframe <- as.data.frame(t(xframe))

colnames(xframe) <- xnames


# fuckframe <- xframe

# fuckframe <- t(fuckframe)

rownames(xframe) <- t
xframe$temp <- rownames(xframe)

hail_mary <- ggplot(data = xframe, aes(x = temp, y = xframe$T2_4k, group = 1)) + geom_line()
print(hail_mary)
