rm(list=ls())

# library(tseries)
# library(forecast)
library(ggplot2)
library(pracma)

# For Linux (and Mac?) use //
# For windows use \\

lattice_size = "80x80"
output_file = "AllAreaN_80x80_v3.csv"
tfilter <- 2000000
time_names = "quarter"  #legacy problem: sometimes it's quarter, other times it sweep

seperator = "\\" # No need to adjust this, will be adjusted based on OS in the next line

if (print(Sys.info()['sysname']) == "Linux") {seperator = "//"} 

rootpath = paste("D:", "Coding", "Cpp", "IsingModel", "DataFiles", lattice_size, sep = seperator)


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
    if (t > 50000) {break}
    
  }
  
  x0 <- x[[1]]

  for (i in 1:length(x)) {
    x[[i]] <- x[[i]] / x0
  }
  
  return(x)
  
}


measure_count <- function(tau_series, tmax_series) {
  nseries <- list()
  
  for (i in 1:length(tau_series)) {
    nseries[[i]] <- tmax_series[[i]] / (2*tau_series[[i]])
  }
  
  return(nseries)

}




# Folder to look in
# rootpath = "D:\\Coding\\Cpp\\IsingModel\\OtherFiles\\2_3_20000000"

rootfolder <- list.files(path = rootpath)

xframe <- data.frame()
xnames <- c()
mareas <- list()
aareas <- list()
sareas <- list()
mtmaxs <- list()
atmaxs <- list()
stmaxs <- list()
# lag = 6000

for (i in 1:length(rootfolder)) {

  subpath <- paste(rootpath, rootfolder[i], sep = seperator)
  subfolder <- list.files(path = subpath)

  master_frame <- data.frame()

  # Iterate through the folder and put all the data into the master frame
  for (j in 1:length(subfolder)) {
    filepath <- paste(subpath, subfolder[j], sep = seperator)

    local_frame <- read.csv(filepath, header = TRUE)
    master_frame <- rbind(master_frame, local_frame)
  }
  
  master_frame <- master_frame[order(master_frame[time_names]), ]
  master_frame <- master_frame[master_frame[time_names] > tfilter, ]
  rownames(master_frame) <- 1:nrow(master_frame)
  
  # xcf <- Acf(master_frame["m"], type = "correlation", lag.max = lag, plot = T)
  # x <- unlist(xcf$acf)
  # # #
  # xnames[i] <- rootfolder[[i]]
  # #
  absm <- abs(master_frame$m)
  squarem <- master_frame[, "m" ]^2
  
  mx <- auto_core(master_frame$m)
  print("Finished X(t) for m")
  ax <- auto_core(absm)
  print("Finished X(t) for |m|")
  sx <- auto_core(squarem)
  print("Finished X(t) for m**2")
  mt <- rep(1:length(mx) - 1)
  # print(unlist(mt))
  at <- rep(1:length(ax) - 1)
  st <- rep(1:length(sx) - 1)

  write.csv(data.frame(x = mt, y = mx), file = paste(rootpath, rootfolder[[i]], "XofT_m.csv", sep = seperator))
  write.csv(data.frame(x = at, y = ax), file = paste(rootpath, rootfolder[[i]], "XofT_absm.csv", sep = seperator))
  write.csv(data.frame(x = st, y = sx), file = paste(rootpath, rootfolder[[i]], "XofT_sqrm.csv", sep = seperator))
    
      # 
  # cor_plot <- ggplot(data = xt, aes(x = x, y = y, group = 1)) + geom_line() + labs(title = rootfolder[[i]])
  # print(cor_plot)
  
  mareas[rootfolder[[i]]] <- trapz(unlist(mt), unlist(mx))
  mtmaxs[rootfolder[[i]]] <- length(unlist(master_frame$m))
  aareas[rootfolder[[i]]] <- trapz(unlist(at), unlist(ax))
  atmaxs[rootfolder[[i]]] <- length(unlist(absm))
  sareas[rootfolder[[i]]] <- trapz(unlist(st), unlist(sx))
  stmaxs[rootfolder[[i]]] <- length(unlist(squarem))
  
  print(paste("Finished", rootfolder[[i]]))
    
}

# tmax = length(master_frame$m)
# 
# 
mn <- measure_count(mareas, mtmaxs)
an <- measure_count(aareas, atmaxs)
sn <- measure_count(sareas, stmaxs)
# 

output_data <- data.frame(Mtau = unlist(mareas), Mtmax = unlist(mtmaxs), Mn = unlist(mn), 
                          Atau = unlist(aareas), Atmax = unlist(atmaxs), An = unlist(an),
                          Stau = unlist(sareas), Stmax = unlist(stmaxs), Sn = unlist(sn))
# 
# write.csv(output_data, "D:\\Coding\\Cpp\\IsingModel\\AreaAllN.csv")
write.csv(output_data, paste(rootpath, output_file, sep = seperator))





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

# sanity <- master_frame$m
# sanity <- auto_core(sanity)
# sanity <- unlist(mt)
# sanity <- unlist(mx)
# sanity <- trapz(unlist(mt), unlist(mx))
# 
# print(length(unlist(st)))
# print(length(unlist(sx)))
# 
# ax <- auto_core(absm)
# 
# print(class(mx))
# print(class(ax))
# 
# print(at <- rep(1:length(ax) - 1))
# print(length(at))
