rm(list=ls())

library(tseries)
library(ggplot2)

# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles"
rootfolder <- list.files(path = rootpath)

# Hold data for Temp Stuff, going to make plots from these later
ave_frame <- data.frame()
ave_names <- c()
tfilter <- 5000


resampleError <- function(samples, samplemean){
  returnMeans <- c()
  
  n <- length(samples)
  all_rows <- seq(1, n)
  
  for (nn in 1:n) {
    
    sub_n <- setdiff(all_rows, nn)
    
    subSample <- samples[unlist(sub_n)]
    returnMeans[[nn]] <- mean(subSample)
  }
  
  sigma <- sqrt(Reduce("+", lapply(returnMeans, function(x) {(x - samplemean)^2})))
  
  return(sigma)
  
}

std_error <- function(data) {
  n <- length(data)
  deviation <- sd(data)
  
  # se <- deviation/sqrt(n)
  se <- deviation
  
  return(se)
}


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
  
  cor_frame <- master_frame[master_frame["quarter"] > tfilter, ]
  cor_frame$abs <- abs(cor_frame$m)
  cor_frame$square <- cor_frame[, "m"]^2
  
  # sem <- std_error(cor_frame$m)
  # seabs <- std_error(cor_frame$abs)
  # sesquare <- std_error(cor_frame$square)
  
  sem <- resampleError(cor_frame$m)
  seabs <- resampleError(cor_frame$abs)
  sesquare <- resampleError(cor_frame$square)
  
  new_row <- c(mean(cor_frame$m) - sem, mean(cor_frame$m), mean(cor_frame$m) + sem, 
               mean(cor_frame$abs) - seabs, mean(cor_frame$abs), mean(cor_frame$abs) + seabs, 
               mean(cor_frame$square) - sesquare, mean(cor_frame$square), mean(cor_frame$square) + sesquare)
  ave_frame <- rbind(ave_frame, new_row)
  ave_names[i] <- rootfolder[i]
  
}

colnames(ave_frame) <- c("mmin", "mean", "mmax", "amin", "abs", "amax", "smin", "square", "smax")
rownames(ave_frame) <- ave_names

mplot <- ggplot(data = ave_frame, aes(x = rownames(ave_frame), y = mean, group = 1)) + geom_point() + geom_errorbar(aes(ymin = mmin, ymax = mmax))
print(mplot)

aplot <- ggplot(data = ave_frame, aes(x = rownames(ave_frame), y = abs, group = 1)) + geom_point() + geom_errorbar(aes(ymin = amin, ymax = amax))
print(aplot)

splot <- ggplot(data = ave_frame, aes(x = rownames(ave_frame), y = square, group = 1)) + geom_point() + geom_errorbar(aes(ymin = smin, ymax = smax))
print(splot)


# 
# write.csv(ave_frame, "D:\\Coding\\Cpp\\IsingModel\\AveMTest_million.csv")
