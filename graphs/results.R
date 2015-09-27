library(ggplot2)
library(reshape)
library(plyr)
library(doBy)

setwd("~/ccn/ndnSIM2.1/ns-3/src/ndnSIM/graphs")

cs100 <-  read.table("../results/ar100cs-trace.txt", header=T)
cs80 <-  read.table("../results/ar80cs-trace.txt", header=T)
cs60 <-  read.table("../results/ar60cs-trace.txt", header=T)
cs40 <-  read.table("../results/ar40cs-trace.txt", header=T)
cs20 <-  read.table("../results/ar20cs-trace.txt", header=T)
cs10 <-  read.table("../results/ar10cs-trace.txt", header=T)

app100 <-  read.table("../results/ar100app-trace.txt", header=T)
app80 <-  read.table("../results/ar80app-trace.txt", header=T)
app60 <-  read.table("../results/ar60app-trace.txt", header=T)
app40 <-  read.table("../results/ar40app-trace.txt", header=T)
app20 <-  read.table("../results/ar20app-trace.txt", header=T)
app10 <-  read.table("../results/ar10app-trace.txt", header=T)

cs100$Node <- factor(cs100$Node)
cs80$Node <- factor(cs80$Node)
cs60$Node <- factor(cs60$Node)
cs40$Node <- factor(cs40$Node)
cs20$Node <- factor(cs20$Node)
cs10$Node <- factor(cs10$Node)

app100$Node <- factor(app100$Node)
app80$Node <- factor(app80$Node)
app60$Node <- factor(app60$Node)
app40$Node <- factor(app40$Node)
app20$Node <- factor(app20$Node)
app10$Node <- factor(app10$Node)

summary(app100)

cs100.hits <- subset(cs100, Type=="CacheHits")
cs80.hits <- subset(cs80, Type=="CacheHits")
cs60.hits <- subset(cs60, Type=="CacheHits")
cs40.hits <- subset(cs40, Type=="CacheHits")
cs20.hits <- subset(cs20, Type=="CacheHits")
cs10.hits <- subset(cs10, Type=="CacheHits")

summary(cs20.hits)

cs10.hits.sum <- ddply(cs10.hits, .(Node), summarize, freq=length(Node), tot=sum(Packets))
cs20.hits.sum <- ddply(cs20.hits, .(Node), summarize, freq=length(Node), tot=sum(Packets))
cs40.hits.sum <- ddply(cs40.hits, .(Node), summarize, freq=length(Node), tot=sum(Packets))
cs60.hits.sum <- ddply(cs60.hits, .(Node), summarize, freq=length(Node), tot=sum(Packets))
cs80.hits.sum <- ddply(cs80.hits, .(Node), summarize, freq=length(Node), tot=sum(Packets))
cs100.hits.sum <- ddply(cs100.hits, .(Node), summarize, freq=length(Node), tot=sum(Packets))

cs100.hits.sum$scen=100
cs80.hits.sum$scen=80
cs60.hits.sum$scen=60
cs40.hits.sum$scen=40
cs20.hits.sum$scen=20
cs10.hits.sum$scen=10


min.length <- min(length(app100$HopCount),length(app80$HopCount),length(app60$HopCount),length(app40$HopCount),length(app20$HopCount),length(app10$HopCount))
min.length

hopcounts <- melt(data.frame(app100$HopCount[1:min.length],app80$HopCount[1:min.length],app60$HopCount[1:min.length],
                             app40$HopCount[1:min.length],app20$HopCount[1:min.length],app10$HopCount[1:min.length]))
colnames(hopcounts) <- c("Type", "Packets")



# hits <- melt(data.frame(cs100.hits$Packets,cs80.hits$Packets,cs60.hits$Packets,cs40.hits$Packets,cs20.hits$Packets,cs10.hits$Packets))
# hits <- melt(data.frame(cs10.hits.sum$tot,cs20.hits.sum$tot,cs40.hits.sum$tot,cs60.hits.sum$tot,cs80.hits.sum$tot,cs100.hits.sum$tot))
hits <- rbind(cs100.hits.sum,cs80.hits.sum,cs60.hits.sum,cs40.hits.sum,cs20.hits.sum,cs10.hits.sum)

hits

hits$sum <- by(hits$tot, hits$Node, sum)
summary(hits)


g.hits <- ggplot(hits, aes(x=ordered(scen), y=tot/10, fill=Node)) + geom_bar(stat="identity") + 
ylab("Cache Hit Ratio [%]") + xlab("Cache Admission Rate [%]")


# function for computing mean, DS, max and min values
min.mean.sd.max <- function(x) {
  r <- c(min(x), mean(x) - sd(x), mean(x), mean(x) + sd(x), max(x))
  names(r) <- c("ymin", "lower", "middle", "upper", "ymax")
  r
}


ggplot(hopcounts, aes(x = Type, y=Packets)) + 
  stat_summary(fun.data = min.mean.sd.max, geom="boxplot") + ylab("Hopcounts") + ylim(0,5)

ggplot(hopcounts, aes(x = Type, y=Packets)) +
#   stat_summary(fun.data = min.mean.sd.max, geom = "boxplot") + 
  geom_jitter(position=position_jitter(width=.2), size=3) 

summary(app100$HopCount)
summary(app80$HopCount)
summary(app60$HopCount)
summary(app40$HopCount)
summary(app20$HopCount)
summary(app10$HopCount)
 
# ggplot(delays, aes(x=Type, y=Packets)) + 
#   geom_boxplot() +
#   ylab("Avg. Delay [s]")


pdf("./hits.pdf", height=3, width=4.5)
g.selpar.req + scale_color_brewer(type="qual", palette="Set1")
dev.off()