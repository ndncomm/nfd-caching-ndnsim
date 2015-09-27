library(ggplot2)
library(reshape)
library(plyr)
library(doBy)

setwd("~/ccn/ndnSIM2.1/ns-3/src/ndnSIM/graphs")

cs <- data.frame()
app <- data.frame()
dflist <- c(100,80,60,40,20,10)
for (i in dflist) {
  # Do some complex things
  cs.temp <- read.table(paste("../results/ar",i,"cs-trace.txt",sep=""), header=T)
  app.temp <- read.table(paste("../results/ar",i,"app-trace.txt",sep=""), header=T)
  cs.temp$scen=i
  app.temp$scen=i
  cs <- rbind(cs,cs.temp)
  app <- rbind(app,app.temp)
}

summary(app)

summary(cs)
cs.sum <- ddply(cs, .(Node,Type,scen), summarize, freq=length(Node), tot=sum(Packets))
cs.hits.sum <- subset(cs.sum, Type=="CacheHits")

min.length <- min(length(app100$HopCount),length(app80$HopCount),length(app60$HopCount),length(app40$HopCount),length(app20$HopCount),length(app10$HopCount))
min.length

hopcounts <- melt(data.frame(app100$HopCount[1:min.length],app80$HopCount[1:min.length],app60$HopCount[1:min.length],
                             app40$HopCount[1:min.length],app20$HopCount[1:min.length],app10$HopCount[1:min.length]))
colnames(hopcounts) <- c("Type", "Packets")
hopcounts
summary(app)


g.hits <- ggplot(cs.hits.sum, aes(x=ordered(scen), y=tot/10, fill=factor(Node))) + geom_bar(stat="identity") + 
  ylab("Cache Hit Ratio [%]") + xlab("Cache Admission Rate [%]")
g.hits

# function for computing mean, DS, max and min values
min.mean.sd.max <- function(x) {
  r <- c(min(x), mean(x) - sd(x), mean(x), mean(x) + sd(x), max(x))
  names(r) <- c("ymin", "lower", "middle", "upper", "ymax")
  r
}

summary(app)

ggplot(app, aes(x = factor(scen), y=HopCount)) + 
  stat_summary(fun.data = min.mean.sd.max, geom="boxplot") + ylab("Hopcounts") + ylim(0,5)

ggplot(app, aes(x = factor(scen), y=HopCount)) +
  geom_jitter(position=position_jitter(width=.2), size=3) 


pdf("./hits.pdf", height=3, width=4.5)
g.hits
dev.off()