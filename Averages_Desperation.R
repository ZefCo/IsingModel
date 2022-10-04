rm(list=ls())

library(tseries)
library(ggplot2)
library(dplyr)

output_file = "AveValues_10x10.csv"
lattice_size <- 10*10

# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles\\10x10"
rootfolder <- list.files(path = rootpath)

cortime_frame <- read.csv("D:\\Coding\\Cpp\\IsingModel\\DataFiles\\AreaAllN_10x10.csv", header = T)
rownames(cortime_frame) <- cortime_frame$X
cortime_frame <- cortime_frame %>% select(-X)

# Hold data for Temp Stuff, going to make plots from these later
ave_frame <- data.frame()
ave_names <- c()
tfilter <- 2000000


for (i in 1:length(rootfolder)) {
  # frame for holding all the data
  master_frame <- data.frame()
  
  # changes the temp into a number
  temp = rootfolder[[i]]
  temp = gsub("_", ".", temp)
  temp = gsub("K", "", temp)
  temp = as.numeric(temp)
  
  subpath <- paste(rootpath, rootfolder[i], sep = "\\")
  subfolder <- list.files(path = subpath)
  
  # Iterate through the folder and put all the data into the master frame
  for (j in 1:length(subfolder)) {
    filepath <- paste(subpath, subfolder[j], sep = "\\")
    
    local_frame <- read.csv(filepath, header = TRUE)
    master_frame <- rbind(master_frame, local_frame)
  }
  
  master_frame <- master_frame[order(master_frame$sweep), ]
  
  # sample_m <- master_frame[master_frame["sweep"] > tfilter, "m"]
  sample_frame <- master_frame[master_frame["sweep"] > tfilter, ]
  sample_frame$abs <- abs(master_frame[master_frame["sweep"] > tfilter, "m"])
  sample_frame$square <- master_frame[master_frame["sweep"] > tfilter, "m"]^2
  
  rownames(sample_frame) <- 1:nrow(sample_frame)
  
  mn <- as.integer(cortime_frame[rootfolder[[i]], "Mn"])
  an <- as.integer(cortime_frame[rootfolder[[i]], "An"])
  sn <- as.integer(cortime_frame[rootfolder[[i]], "Sn"])
  #   
  #   msamples <- nsamples(sample_frame$m, mn, mtau, tfilter)
  #   asamples <- nsamples(sample_frame$abs, an, atau, tfilter)
  #   ssamples <- nsamples(sample_frame$square, sn, stau, tfilter)
  #   
  #   
  #   sample_m <- sample_frame[unlist(msamples), "m"]
  #   sample_abs <- sample_frame[unlist(asamples), "abs"]
  #   sample_square <- sample_frame[unlist(ssamples), "square"]
  # 
  #   sem <- resampleError(sample_m)  # returns mean - error, mean, mean + error
  #   seabs <- resampleError(sample_abs)
  #   sesquare <- resampleError(sample_square)
  #   
  #   sus <- (lattice_size / temp) * (sesquare[[2]] - (sem[[2]])^2)
  # 
  #   new_row <- c(sem, seabs, sesquare, sus)
  # 
  #   ave_frame <- rbind(ave_frame, new_row)
  #   ave_names[i] <- rootfolder[i]
  #   
  print(paste("Finished", rootfolder[[i]]))
  break
  # 
}