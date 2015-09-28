
# function for computing mean, DS, max and min values
min.mean.sd.max <- function(x) {
  r <- c(min(x), mean(x) - sd(x), mean(x), mean(x) + sd(x), max(x))
  names(r) <- c("ymin", "lower", "middle", "upper", "ymax")
  r
}

# ggplot(app, aes(x = factor(scen), y=HopCount)) + 
#   stat_summary(fun.data = min.mean.sd.max, geom="boxplot") + ylab("Hopcounts") + ylim(0,5)
