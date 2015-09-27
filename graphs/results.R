library(ggplot2)
library(reshape)
library(plyr)

setwd("~/ccn/ndnSIM2.1/ns-3/src/ndnSIM/graphs")

cs100 <-  read.table("../results/ar100cs-trace.txt", header=T)
cs25 <-  read.table("../results/ar25cs-trace.txt", header=T)

app100 <-  read.table("../results/ar100app-trace.txt", header=T)
app25 <-  read.table("../results/ar25app-trace.txt", header=T)

cs100$Node <- factor(cs100$Node)
cs25$Node <- factor(cs25$Node)
app100$Node <- factor(app100$Node)
app25$Node <- factor(app25$Node)

summary(cs100)
summary(cs25)

cs100$Perc=100
cs25$Perc=25


summary(app100)
summary(app25)




cs100.hits <- subset(cs100, Type=="CacheHits")
cs25.hits <- subset(cs25, Type=="CacheHits")

delays <- melt(data.frame(app100$HopCount,app25$DelayS))
colnames(delays) <- c("Type", "Packets")

hopcounts <- melt(data.frame(app100$HopCount,app25$HopCount))
colnames(hopcounts) <- c("Type", "Packets")

hits <- melt(data.frame(cs100.hits$Packets,cs25.hits$Packets))
colnames(hits) <- c("Type", "Packets")
summary(hits)

ggplot(hits, aes(x=Type, y=Packets)) + 
  geom_boxplot() +
  ylab("Avg. Cache Hits [/s]")

hopcounts

ggplot(hopcounts, aes(x=Type, y=Packets)) + 
  geom_boxplot() +
  ylab("Avg. Cache Hits [/s]")

# function for computing mean, DS, max and min values
min.mean.sd.max <- function(x) {
  r <- c(min(x), mean(x) - sd(x), mean(x), mean(x) + sd(x), max(x))
  names(r) <- c("ymin", "lower", "middle", "upper", "ymax")
  r
}

ggplot(hopcounts, aes(x = Type, y=Packets)) + 
  #stat_summary(fun.data = min.mean.sd.max, geom = "boxplot") + 
  geom_jitter(position=position_jitter(width=.2), size=3) 

# ggplot(delays, aes(x=Type, y=Packets)) + 
#   geom_boxplot() +
#   ylab("Avg. Delay [s]")

ggplot(cs100.hits, aes(x=Node, y=Packets)) + 
  geom_bar(stat="identity",position="dodge") +
  ylab("Cache Hits")

ggplot(cs25.hits, aes(x=Node, y=Packets)) + 
  geom_bar(stat="identity",position="dodge") +
  ylab("Cache Hits")

cs100.hits


summary(cs100.hits$Packets)
summary(cs25.hits$Packets)


