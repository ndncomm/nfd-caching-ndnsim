library(ggplot2)
library(reshape)
library(plyr)

setwd("~/ccn/ndnSIM2.1/ns-3/src/ndnSIM/graphs")

cs100 <-  read.table("../results/ar100cs-trace.txt", header=T)
cs25 <-  read.table("../results/ar25cs-trace.txt", header=T)

app100 <-  read.table("../results/ar100app-trace.txt", header=T)
app25 <-  read.table("../results/ar25cs-trace.txt", header=T)



summary(cs25)
summary(cs100)

cs25.misses <- subset(cs25, Type=="CacheMisses")
cs100.misses <- subset(cs100, Type=="CacheMisses")


summary(cs25.misses)
summary(cs100.misses)

