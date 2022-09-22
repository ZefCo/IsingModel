rm(list=ls())

# Try to use the FFT version of X(t) because this is taking WAY to long for 2_1, 2_2, 2_0

# Tau for 2_2 ~ 4000. So guess @ 2_3, 2_4 and go from there.

# library(tseries)
library(forecast)
library(ggplot2)
library(pracma)



summation <- function() {
  term1 <- 0
  term2 <- 0
  term3 <- 0
}



ac_fft <- function(inseries) {
  return_thing <- 0
  tmax <- length(inseries)
  
  print(class(inseries[[1]]))
  
  ac_tilde <- fft(unlist(inseries))
  
  term1 <- 0
  term2 <- 0
  term3 <- 0
  
  alpha <- 1 / 1

  for (tp in 1: (tmax)) {
    term1 <- term1 + (inseries[tp] * inseries[tp])
    term2 <- term2 + inseries[tp]
    term3 <- term3 + inseries[tp]
  }

  x0 <- (alpha * term1) - ((alpha * term2) * (alpha * term3))
  
  x0tilda <- as.numeric(ac_tilde[[1]])**2
  
  tau <- x0tilda / x0
  # tau <- ac_tilde
  
  return(tau)
  
  
}



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
    
    if (x[t] < 0) {break}
  }
  
  x0 <- x[[1]]

  for (i in 1:length(x)) {
    x[[i]] <- x[[i]] / x0
  }
  
  return(x)
  
}

tfilter <- 500000

# Folder to look in
# rootpath = "D:\\Coding\\Cpp\\IsingModel\\OtherFiles\\2_3_20000000"
rootpath = "D:\\Coding\\Cpp\\IsingModel\\DataFiles"
rootfolder <- list.files(path = rootpath)

xframe <- data.frame()
xnames <- c()
areas <- list()

# lag = 6000

# for (i in 1:length(rootfolder)) {
#   
#   subpath <- paste(rootpath, rootfolder[i], sep = "\\")
#   subfolder <- list.files(path = subpath)
#   
#   master_frame <- data.frame()
#   
#   # Iterate through the folder and put all the data into the master frame
#   for (j in 1:length(subfolder)) {
#     filepath <- paste(subpath, subfolder[j], sep = "\\")
#     
#     local_frame <- read.csv(filepath, header = TRUE)
#     master_frame <- rbind(master_frame, local_frame)
#   }
#   
#   master_frame <- master_frame[master_frame["quarter"] > tfilter, ]
#   
#   # xcf <- Acf(master_frame["m"], type = "correlation", lag.max = lag, plot = T)
#   # x <- unlist(xcf$acf)
#   # # # 
#   # xnames[i] <- rootfolder[[i]]
#   # # 
#   x <- auto_core(master_frame$m, tmax = length(master_frame$m))
#   t <- rep(1:length(x) - 1)
#   
#   xt <- data.frame(x = t, y = x)
#   
#   cor_plot <- ggplot(data = xt, aes(x = x, y = y, group = 1)) + geom_line() + labs(title = rootfolder[[i]])
#   print(cor_plot)
#   
#   areas[rootfolder[[i]]] <- trapz(t, x)
#   
# }
# 
# tmax = length(master_frame$m)
# 
# 
# n <- sapply(areas, FUN = function(x) {tmax / (2*x)})
# 
# output_data <- data.frame(n = n, tau = unlist(areas))
# 
# write.csv(output_data, "D:\\Coding\\Cpp\\IsingModel\\AreaN.csv")





# # 
# xframe <- rbind(xframe, x)
# # 
# areas[rootfolder[[i]]] <- trapz(t, x)
# # }
# 
# 
# 
# 
# xframe <- as.data.frame(t(xframe))
# 
# colnames(xframe) <- xnames
# 
# 
# # fuckframe <- xframe
# 
# # fuckframe <- t(fuckframe)
# 
# rownames(xframe) <- t
# xframe$temp <- rownames(xframe)
# 
# # colnames(xframe) <- c("a", "b", "c", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", 'r', "s", "t", "u", "v", "w")
# # othernames <- xnames
# # othernames[length(othernames) + 1] <- "temp"
# # colnames(xframe) <- othernames
# # 
# # hail_mary <- ggplot(data = xframe, aes(x = temp, y = T2_4K, group = 1)) + geom_line()
# # print(hail_mary)
# 
# 
# write.csv(xframe, "D:\\Coding\\Cpp\\IsingModel\\critTimeArea_2.csv")
# 
# area_frame <- data.frame(a = unlist(areas), t = xnames)
# 
# time_plot <- ggplot(data = area_frame, aes(x = t, y = a, group = 1)) + geom_point() + geom_line() + labs(title = "Correlation Time")
# print(time_plot)






lag <- 100000
for (i in 1:length(rootfolder)) {
  
  subpath <- paste(rootpath, rootfolder[i], sep = "\\")
  subfolder <- list.files(path = subpath)
  
  master_frame <- data.frame()
  
  # Iterate through the folder and put all the data into the master frame
  for (j in 1:length(subfolder)) {
    filepath <- paste(subpath, subfolder[j], sep = "\\")
    
    local_frame <- read.csv(filepath, header = TRUE)
    master_frame <- rbind(master_frame, local_frame)
  }
  
  master_frame <- master_frame[master_frame["quarter"] > tfilter, ]
  
  tau <- ac_fft(master_frame["m"])
  
  # xcf <- Acf(master_frame["m"], type = "correlation", lag.max = lag, plot = T)
  # x <- unlist(xcf$acf)
  # #
  # xnames[i] <- rootfolder[[i]]
  #
  # x <- auto_core(master_frame$m, tmax = length(master_frame$m))
  # t <- rep(1:length(x) - 1)
  
  # xt <- data.frame(x = t, y = x)
  
  # cor_plot <- ggplot(data = xt, aes(x = x, y = y, group = 1)) + geom_line() + labs(title = rootfolder[[i]])
  # print(cor_plot)
  # 
  # areas[rootfolder[[i]]] <- trapz(t, x)
  
}

# tmax = length(master_frame$m)
# n <- sapply(areas, FUN = function(x) {tmax / (2*x)})


# taus <- data.frame(n = n, tau = unlist(areas))
# write.csv(taus, "D:\\Coding\\Cpp\\IsingModel\\DifficultAreas.csv")
