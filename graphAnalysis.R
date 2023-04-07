graphData = read.csv("C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\Analysis.csv")

fileA19 = "C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\analysis\\A19.csv"
fileA20 = "C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\analysis\\A20.csv"
fileA21 = "C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\analysis\\A21.csv"
fileA22 = "C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\analysis\\A22.csv"
a19 = read.csv(fileA19, skip = 2, header =  T)
a20 = read.csv(fileA20, skip = 2, header = T)
a21 = read.csv(fileA21, skip = 2, header = T)
a22 = read.csv(fileA22, skip = 2, header = T)

a19Plot <- head()

x <- a19$Messages
y1 <- a19$Hops
y2 <- a20$Hops
y3 <- a21$Hops
y4 <- a22$Hops

plot(x, y2, type = "l" ,col = 'red', xlab = 'Number of Messages', ylab = 'Hop Counts', main = "Hopcounts vs Number of Messages \nGraphs: A19, A20, A21, A22", lwd = 3)
lines(x, y1, col = 'blue', lwd = 3, labels = c("A19"))
lines(x, y3, col = 'royalblue', lwd = 3)
lines(x, y4, col = 'maroon', lwd = 3)



fileA36 = "C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\analysis\\A36.csv"
fileA37 = "C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\analysis\\A37.csv"
fileA38 = "C:\\Users\\mwbur\\OneDrive\\Documents\\IUPUI\\OperatingSystems\\Projects\\Project 3\\Group\\CSCI403_spring23_P3\\analysis\\A38.csv"
a19 = read.csv(fileA19, skip = 2, header =  T)
a20 = read.csv(fileA20, skip = 2, header = T)
a21 = read.csv(fileA21, skip = 2, header = T)
a22 = read.csv(fileA22, skip = 2, header = T)

a19Plot <- head()

x <- a19$Messages
y1 <- a19$Hops
y2 <- a20$Hops
y3 <- a21$Hops
y4 <- a22$Hops

plot(x, y2, type = "l" ,col = 'red', xlab = 'Number of Messages', ylab = 'Hop Counts', main = "Hopcounts vs Number of Messages \nGraphs: A19, A20, A21, A22", lwd = 3)
lines(x, y1, col = 'blue', lwd = 3, labels = c("A19"))
lines(x, y3, col = 'royalblue', lwd = 3)
lines(x, y4, col = 'maroon', lwd = 3)

