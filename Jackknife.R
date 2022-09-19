
(list=ls())

library("dplyr")
library('ggplot2')

# ave_frame <- read.csv("D:\\Coding\\Cpp\\IsingModel\\AveM.csv")
ave_frame <- read.csv("D:\\Coding\\Cpp\\IsingModel\\AveMnofilter.csv")
rownames(ave_frame) <- ave_frame$X
ave_frame <- ave_frame[c("mean", "abs", "square")]

samples_used <- 1000
se_frame <- data.frame()
# colnames(se_frame) <- rownames(temp_frame)
# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles"
rootfolder <- list.files(path = rootpath)


resampleMean <- function(samples){
  returnMeans <- c()

  n <- length(samples)
  all_rows <- seq(1, n)

  for (nn in 1:n) {

    sub_n <- setdiff(all_rows, nn)

    subSample <- samples[unlist(sub_n)]
    returnMeans[[nn]] <- mean(subSample)
  }

  return(returnMeans)

}



se_names <- c()
mean_max <- c()
mean_min <- c()
abs_min <- c()
abs_max <- c()
square_min <- c()
square_max <- c()



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
  
  # make sure the master frame is ordered because 10 comes before 2, 3, 4, etc.
  master_frame <- master_frame[order(master_frame$quarter), ]
  master_frame <- master_frame[master_frame["quarter"] > 0, ]
  
  resample_frame <- sample_n(master_frame, samples_used)
  resample_frame$abs <- abs(resample_frame$m)
  resample_frame$square <- resample_frame[, "m"]^2
  
  resample_ave <- mean(resample_frame$m)
  resample_absave <- mean(resample_frame$abs)
  resample_squareave <- mean(resample_frame$square)
  
  mi <- resampleMean(resample_frame$m)
  ma <- resampleMean(resample_frame$abs)
  ms <- resampleMean(resample_frame$square)
  sei <- sqrt(Reduce("+", lapply(mi, function(x) {(x - resample_ave)^2})))
  sea <- sqrt(Reduce("+", lapply(ma, function(x) {(x - resample_absave)^2})))
  ses <- sqrt(Reduce("+", lapply(ms, function(x) {(x - resample_squareave)^2})))

  mean_max[i] <- ave_frame[i, "mean"] + sei
  mean_min[i] <- ave_frame[i, "mean"] - sei
  abs_max[i] <- ave_frame[i, "abs"] + sea
  abs_min[i] <- ave_frame[i, "abs"] - sea
  square_max[i] <- ave_frame[i, "square"] + ses
  square_min[i] <- ave_frame[i, "square"] - ses

}

ave_frame$mmin <- mean_min
ave_frame$amin <- abs_min
ave_frame$smin <- square_min
ave_frame$mmax <- mean_max
ave_frame$amax <- abs_max
ave_frame$smax <- square_max
ave_frame$temp <- rownames(ave_frame)


mplot <- ggplot(data = ave_frame, aes(x = temp, y = mean, group = 1)) + geom_point() + geom_errorbar(aes(ymin = mmin, ymax = mmax))
print(mplot)

aplot <- ggplot(data = ave_frame, aes(x = temp, y = abs, group = 1)) + geom_point() + geom_errorbar(aes(ymin = amin, ymax = amax))
print(aplot)

splot <- ggplot(data = ave_frame, aes(x = temp, y = square, group = 1)) + geom_point() + geom_errorbar(aes(ymin = smin, ymax = smax))
print(splot)


# write.csv(ave_frame, "D:\\Coding\\Cpp\\IsingModel\\AveMwError.csv")


