library(ggplot2)
library(reshape)
library(plyr)
library(doBy)

setwd("~/ccn/ndnSIM2.1/ns-3/src/ndnSIM/graphs")

# dflist <- c(100)
dflist <- c(100,80,60,40,20,10,5)
tree.cs <- data.frame()
tree.app <- data.frame()
lin.cs <- data.frame()
lin.app <- data.frame()

for (i in dflist) {
  # Do some complex things
  tree.cs.temp <- read.table(paste("../results/tree_unif",i,"cs-trace.txt",sep=""), header=T)
  tree.app.temp <- read.table(paste("../results/tree_unif",i,"app-trace.txt",sep=""), header=T)
  tree.cs.temp$scen=i
  tree.app.temp$scen=i
  tree.cs <- rbind(tree.cs,tree.cs.temp)
  tree.app <- rbind(tree.app,tree.app.temp)
  
  lin.cs.temp <- read.table(paste("../results/lin_unif",i,"cs-trace.txt",sep=""), header=T)
  lin.app.temp <- read.table(paste("../results/lin_unif",i,"app-trace.txt",sep=""), header=T)
  lin.cs.temp$scen=i
  lin.app.temp$scen=i
  lin.cs <- rbind(lin.cs,lin.cs.temp)
  lin.app <- rbind(lin.app,lin.app.temp)
}

tree.cs.sum <- ddply(tree.cs, .(Node,Type,scen), summarize, freq=length(Node), tot=sum(Packets))
tree.cs.hits.sum <- subset(tree.cs.sum, Type=="CacheHits")
lin.cs.sum <- ddply(lin.cs, .(Node,Type,scen), summarize, freq=length(Node), tot=sum(Packets))
lin.cs.hits.sum <- subset(lin.cs.sum, Type=="CacheHits")


g.hits.tree <- ggplot(tree.cs.hits.sum, aes(x=ordered(scen), y=tot/10, fill=factor(Node))) + geom_bar(stat="identity") + 
  ylab("Cache Hit Ratio [%]") + xlab("Cache Admission Rate [%]")
g.hits.lin <- ggplot(lin.cs.hits.sum, aes(x=ordered(scen), y=tot/10, fill=factor(Node))) + geom_bar(stat="identity") + 
  ylab("Cache Hit Ratio [%]") + xlab("Cache Admission Rate [%]")

g.hits.tree
g.hits.lin

ggplot(tree.app, aes(x = factor(scen), y=HopCount)) +
  geom_jitter(position=position_jitter(width=.2), size=3) 

g.hopcount.lin <- ggplot(lin.app, aes(x = factor(scen), y=HopCount)) +
  geom_jitter(position=position_jitter(width=.2), size=3) 


tree.app.mean <- ddply(tree.app, .(Node,scen), summarize, freq=length(Node), HopCount=mean(HopCount))
lin.app.mean <- ddply(lin.app, .(Node,scen), summarize, freq=length(Node), HopCount=mean(HopCount))

g.hopcount.tree <- ggplot(tree.app.mean, aes(x = factor(scen), y=HopCount)) + 
  geom_boxplot()
#   geom_bar(stat="identity", position="dodge")


g.hopcount.lin <- ggplot(lin.app.mean, aes(x = factor(scen), y=HopCount)) + 
   geom_boxplot()
#    geom_bar(stat="identity", position="dodge")


pdf("./tree_hits.pdf",width=10)
g.hits.tree
dev.off()

pdf("./lin_hits.pdf",width=10)
g.hits.lin
dev.off()

pdf("./lin_hopcount_jitter.pdf",width=10)
g.hopcount.lin
dev.off()

pdf("./tree_hopcount.pdf", height=3, width=4.5)
g.hopcount.tree
dev.off()

pdf("./lin_hopcount.pdf", height=3, width=4.5)
g.hopcount.lin
dev.off()
