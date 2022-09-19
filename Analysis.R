rm(list=ls())

library(tseries)
library(ggplot2)
library(pracma)
library(bootstrap)
library(Monte.Carlo.se)


# How many points for equillibrium
equil_time <- 100

# Folder to look in
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles"
rootfolder <- list.files(path = rootpath)

# Hold data for Temp Stuff, going to make plots from these later
temp_frame <- data.frame()
temp_names <- c()

samples <- 1000

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
  # Only grab the things before the equillibrium time and then find <m>, <|m|>, <m**2>
  print("Finding Critical Values")
  cor_frame <- master_frame[master_frame["quarter"] > 100000, ]
  cor_frame$abs <- abs(cor_frame$m)
  cor_frame$square <- cor_frame[, "m"]^2
  
  # print("Finding Jackknife")
  # theta <- function(x){mean(x)}
  # mjack <- jackknife(1:sample, theta, cor_frame$m)

  # mMean <- mean(sub_frame$x)

  # Find the X(t)
  print("Finding Autocorrelation")
  x <- acf(master_frame["m"], type = "correlation", lag.max = equil_time, plot = T)
  xpoints <- unlist(x$acf)
  t <- rep(1:length(xpoints) - 1)
  # Put that into it's down dataframe for finding the area under the curve
  xdata <- data.frame(t = t, y = xpoints)
  xarea <- trapz(xdata$t, xdata$y)

  # Put those into the temp frame, Doing it here so I can find the area which requires finding
  # the autocorrelation first
  new_row <- c(mean(cor_frame$m), mean(cor_frame$abs), mean(cor_frame$square), xarea)
  temp_frame <- rbind(temp_frame, new_row)
  temp_names[i] <- rootfolder[i]
  
  print("Plotting")
  xplot <- ggplot(data = xdata, aes(x = t, y = y)) + geom_line() + labs(x = "t", y = "X(t)", title = rootfolder[i])
  print(xplot)

  # break
  
}

# area <- trapz(unlist(xdata$t), unlist(xdata$y))
# xplot <- ggplot(data = xdata, aes(x = t, y = y)) + geom_line() + labs(x = "t", y = "X(t)", title = rootfolder[i])
# print(xplot)


rownames(temp_frame) <- temp_names
colnames(temp_frame) <- c("mean", "abs", "square", "corrTime")
temp_frame$temp <- rownames(temp_frame)

mplot <- ggplot(data = temp_frame, aes(x = temp, y = mean, group = 1)) + geom_point()
print(mplot)

absplot <- ggplot(data = temp_frame, aes(x = temp, y = abs, group = 1)) + geom_point() + geom_line()
print(absplot)

squareplot <- ggplot(data = temp_frame, aes(x = temp, y = square, group = 1)) + geom_line() + geom_point() 
print(squareplot)

timeplot <- ggplot(data = temp_frame, aes(x = temp, y = corrTime, group = 1)) + geom_line() + geom_point()
print(timeplot)


# write.csv(temp_frame, "D:\\Coding\\Cpp\\IsingModel\\AveTempFrame.csv")
