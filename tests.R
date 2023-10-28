require("stats")
require("dplyr")
library("tidyr")
library("ez")
library ("ggpubr")

shared <- c(4.25316,
  4.08298,
  4.33247,
  4.43508,
  4.31961,
  4.36475,
  4.41134,
  4.40525,
  4.24102,
  4.38621,
  4.28619,
  4.41644,
  4.26755,
  4.41560,
  4.34266,
  4.32459,
  4.29833,
  4.30637,
  4.37074,
  4.29446)
private <- c(4.01180,
             3.98429,
             4.19808,
             3.93601,
             3.91274,
             4.17609,
             3.99839,
             4.19969,
             4.19143,
             3.98246,
             4.17385,
             4.22297,
             4.23089,
             4.18258,
             4.17078,
             4.20009,
             4.20117,
             3.96725,
             3.96108,
             4.07614)
diff <- shared - private
df <- data.frame(shared, private, diff)
df
normalidad <- shapiro.test(diff)
normalidad

alfa <- 0.05
valor_nulo <- 0
prueba_1 <- t.test (diff,alternative = "two.sided",mu = valor_nulo ,conf.level = 1 - alfa )

prueba_1

#H0: Son iguales
#Ha: Son distintos

c10 <- c(4.32000,
         4.22000,
         4.27117,
         4.34203,
         4.33079,
         4.26585,
         4.28742,
         4.32123,
         4.29019,
         4.30624,
         4.38115,
         4.29657,
         4.24633,
         4.30114,
         4.54894,
         4.3482,
         4.43427,
         4.3048,
         4.33578,
         4.18644
)
c1000 <- c(4.19717,
           4.22174,
           4.28654,
           4.24367,
           4.19843,
           4.19495,
           4.17968,
           4.12144,
           4.0708,
           4.23781,
           4.22217,
           4.12284,
           4.08128,
           4.2243,
           4.27612,
           4.19013,
           4.23085,
           4.09935,
           4.22439,
           4.16547)
c100000 <- c(4.21791,
            4.31700,
            4.21325,
            4.24704,
            4.38931,
            4.22924,
            4.2418,
            4.2342,
            4.26846,
            4.38267,
            4.26174,
            4.26799,
            4.29617,
            4.20238,
            4.22473,
            4.38614,
            4.24121,
            4.26369,
            4.28081,
            4.27665
)
instancia <- factor (1:20)
chunk <- data.frame(c10, c1000, c100000,instancia)
datos <- chunk %>% pivot_longer(c("c10", "c1000", "c100000") ,names_to = "chunks", values_to = "tiempo")
datos [["chunks"]] <- factor ( datos [["chunks" ]])
prueba <- aov ( tiempo ~ chunks + Error ( instancia /( chunks ) ) ,data = datos )
print ( summary ( prueba ) )

prueba2 <- ezANOVA ( data = datos , dv = tiempo , within = chunks ,wid = instancia , return_aov = TRUE )
prueba2

g2 <- ezPlot ( data = datos , dv = tiempo , wid = instancia , within = chunks ,y_lab = "Tiempo promedio de ejecución con 5 tareas", x = chunks )
print ( g2 )

bonferroni <- pairwise.t.test ( datos [["tiempo"]] , datos [["chunks"]] ,p.adj = "bonferroni", paired = TRUE )
print ( bonferroni )



g <- ggqqplot ( datos , "tiempo", facet.by = "chunks",color = "chunks")
print(g)

