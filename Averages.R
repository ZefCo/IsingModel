rm(list=ls())

library(tseries)
library(ggplot2)
library(dplyr)

# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles"
rootfolder <- list.files(path = rootpath)

cortime_frame <- read.csv("D:\\Coding\\Cpp\\IsingModel\\AreaAllN.csv", header = T)
rownames(cortime_frame) <- cortime_frame$X
cortime_frame <- cortime_frame %>% select(-X)

# Hold data for Temp Stuff, going to make plots from these later
ave_frame <- data.frame()
ave_names <- c()
tfilter <- 500000



nsamples <- function(inseries, max_n, tau, start_index) {
  nsamples <- c()
  max_index <- length(inseries)
  
  for (k in 1:max_n) {
    sample_index <- start_index + (k*tau)
    if (sample_index > max_index) {break}
    
    nsamples[[k]] <- sample_index
  }
  
  return(nsamples)
  
}



resampleError <- function(samples){
  samplemean <- mean(samples)
  
  n <- length(samples)
  if (n > 2000) {n <- 1000}
  else if ((n < 2000) && (n > 100)) {n <- 100}
  else {n <- as.integer(n/2)}
  
  subSamples <- sample(x = samples, size = n)
  
  all_rows <- seq(1, n)
  nMeans <- c()
  
  for (nn in 1:n) {
    
    sub_n <- setdiff(all_rows, nn)
    
    nSample <- subSamples[unlist(sub_n)]
    nMeans[[nn]] <- mean(nSample)
  }
  
  sigma <- sqrt(Reduce("+", lapply(nMeans, function(x) {(x - samplemean)^2})))
  returnData <- c(samplemean - sigma, samplemean, samplemean + sigma)

  return(returnData)
  
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
  
  # sample_m <- master_frame[master_frame["quarter"] > tfilter, "m"]
  sample_frame <- master_frame[master_frame["quarter"] > tfilter, ]
  sample_frame$abs <- abs(master_frame[master_frame["quarter"] > tfilter, "m"])
  sample_frame$square <- master_frame[master_frame["quarter"] > tfilter, "m"]^2
  
  mn <- as.integer(cortime_frame[rootfolder[[i]], "Mn"])
  mtau <- (as.integer(cortime_frame[rootfolder[[i]], "Mtau"]) * 2) + 1 # add 1 to it in case it's zero
  an <- as.integer(cortime_frame[rootfolder[[i]], "An"])
  atau <- (as.integer(cortime_frame[rootfolder[[i]], "Atau"]) * 2) + 1 # add 1 to it in case it's zero
  sn <- as.integer(cortime_frame[rootfolder[[i]], "Sn"])
  stau <- (as.integer(cortime_frame[rootfolder[[i]], "Stau"]) * 2) + 1 # add 1 to it in case it's zero
  
  msamples <- nsamples(sample_frame$m, mn, mtau, tfilter)
  asamples <- nsamples(sample_frame$abs, an, atau, tfilter)
  ssamples <- nsamples(sample_frame$square, sn, stau, tfilter)
  
  
  sample_m <- sample_frame[unlist(msamples), "m"]
  sample_abs <- sample_frame[unlist(asamples), "abs"]
  sample_square <- sample_frame[unlist(ssamples), "square"]

  sem <- resampleError(sample_m)
  seabs <- resampleError(sample_abs)
  sesquare <- resampleError(sample_square)

  new_row <- c(sem, seabs, sesquare)

  ave_frame <- rbind(ave_frame, new_row)
  ave_names[i] <- rootfolder[i]
  
  print(paste("Finished", rootfolder[[i]]))

}

colnames(ave_frame) <- c("mmin", "mean", "mmax", "amin", "abs", "amax", "smin", "square", "smax")
rownames(ave_frame) <- ave_names

mplot <- ggplot(data = ave_frame, aes(x = rownames(ave_frame), y = mean, group = 1)) + geom_point() + geom_errorbar(aes(ymin = mmin, ymax = mmax)) + labs(title = "<m>") + xlab("Temp") + ylab("")
print(mplot)

aplot <- ggplot(data = ave_frame, aes(x = rownames(ave_frame), y = abs, group = 1)) + geom_point() + geom_errorbar(aes(ymin = amin, ymax = amax)) + labs(title = "<|m|>") + xlab("Temp") + ylab("")
print(aplot)

splot <- ggplot(data = ave_frame, aes(x = rownames(ave_frame), y = square, group = 1)) + geom_point() + geom_errorbar(aes(ymin = smin, ymax = smax)) + labs(title = "<m**2>") + xlab("Temp") + ylab("")
print(splot)

tplot <- ggplot(data = cortime_frame, aes(x = rownames(cortime_frame), y = Mtau, group = 1)) + geom_point() + geom_line() + labs(title = "Tau: X(t) calculated to a maximum of 10,000 points") + xlab("Temp") + ylab("")
print(tplot)

write.csv(ave_frame, "D:\\Coding\\Cpp\\IsingModel\\AveValues.csv")
