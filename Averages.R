rm(list=ls())

library(tseries)

# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles"
rootfolder <- list.files(path = rootpath)

# Hold data for Temp Stuff, going to make plots from these later
ave_frame <- data.frame()
ave_names <- c()
tfilter <- 0


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
  new_row <- c(mean(cor_frame$m), mean(cor_frame$abs), mean(cor_frame$square))
  ave_frame <- rbind(ave_frame, new_row)
  ave_names[i] <- rootfolder[i]
  
}

colnames(ave_frame) <- c("mean", "abs", "square")
rownames(ave_frame) <- ave_names

write.csv(ave_frame, "D:\\Coding\\Cpp\\IsingModel\\AveMnofilter.csv")
