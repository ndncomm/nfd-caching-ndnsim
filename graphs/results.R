library(ggplot2)
library(reshape)
library(plyr)

setwd("~/ccn/ndnSIM2.1/ns-3/src/ndnSIM/graphs")

cs100 <-  read.table("../results/ar100cs-trace.txt", header=T)
cs80 <-  read.table("../results/ar80cs-trace.txt", header=T)
cs60 <-  read.table("../results/ar60cs-trace.txt", header=T)
cs40 <-  read.table("../results/ar40cs-trace.txt", header=T)
cs20 <-  read.table("../results/ar20cs-trace.txt", header=T)

# cs25 <-  read.table("../results/ar25cs-trace.txt", header=T)

app100 <-  read.table("../results/ar100app-trace.txt", header=T)
app80 <-  read.table("../results/ar80app-trace.txt", header=T)
app60 <-  read.table("../results/ar60app-trace.txt", header=T)
app40 <-  read.table("../results/ar40app-trace.txt", header=T)
app20 <-  read.table("../results/ar20app-trace.txt", header=T)

cs100$Node <- factor(cs100$Node)
cs80$Node <- factor(cs80$Node)
cs60$Node <- factor(cs60$Node)
cs40$Node <- factor(cs40$Node)
cs20$Node <- factor(cs20$Node)

app100$Node <- factor(app100$Node)
app80$Node <- factor(app80$Node)
app60$Node <- factor(app60$Node)
app40$Node <- factor(app40$Node)
app20$Node <- factor(app20$Node)

summary(app100)
summary(app25)

cs100.hits <- subset(cs100, Type=="CacheHits")
cs80.hits <- subset(cs80, Type=="CacheHits")
cs60.hits <- subset(cs60, Type=="CacheHits")
cs40.hits <- subset(cs40, Type=="CacheHits")
cs20.hits <- subset(cs20, Type=="CacheHits")

summary(app60)
summary(app20)
summary(app80)

app100$HopCount[1:1978]

hopcounts <- melt(data.frame(app100$HopCount,app80$HopCount,app60$HopCount,app40$HopCount))
colnames(hopcounts) <- c("Type", "Packets")

hits <- melt(data.frame(cs100.hits$Packets,cs80.hits$Packets,cs60.hits$Packets,cs40.hits$Packets,cs20.hits$Packets))
colnames(hits) <- c("Type", "Packets")
summary(hits)

ggplot(hits, aes(x=Type, y=Packets)) + 
  geom_boxplot() +
  ylab("Avg. Cache Hits [/s]")


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


