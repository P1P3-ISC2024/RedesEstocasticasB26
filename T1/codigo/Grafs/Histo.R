# Programador: Felipe de Jesús Martínez Alfaro, con ayuda de Chat-GPT xD
# Nota: en la consola R primero insralar la libreria.
# install.packages("readxl")

# imports...
library(readxl)

# Seleccinar la ruta de trabajo...
setwd("D:/USER/Desktop/carpetas/CIC/SEM3/RedesEstoc/Tareas/T1/resultados")

# Leer el csv de la carpeta...
datos <- read.csv("histo-exp.csv", header = TRUE, stringsAsFactors = FALSE)

print(datos)

# Gráfico de barras con datos ordenados
barplot(datos$conteos,
        names.arg = datos$x,
        main = "Conteo por rango de 0.1",
        xlab = "x",
        ylab = "Frecuencia",
        col = "skyblue",
        #horiz = TRUE,       # Barras horizontales.
        las = 2,            # 1 Letras horiz, con 2 es verticales.
        width = 2,          # Ancho de las barras
        border = "white")


head(datos)      	# Muestra las primeras filas.
names(datos)     	# Muestra los nombres de las columnas.
str(datos[[1]])	# Verificamos que detecta bien la info de la columna.

