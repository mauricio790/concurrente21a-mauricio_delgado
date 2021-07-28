# Reporte de eficiencia
---

# Comparación 01: Concurrencia en OMP vs pthreads

Para realizar esta comparación de concurrencia se utilizó el código con las mismas optimizaciones en ambos partes con la única diferencia de su implementación de concurrencia para así asegurar una comparación más justa. Dado el enunciado, la única diferencia está en el mapeo. 

La versión con pthreads mapea dinámicamente los números introducidos por el usuario entre la cantidad total de hilos disponibles mientras que la versión con OMP mapea las sumas de cada número, o sea que todos los hilos trabajan en las sumas de un mismo número a la vez. El speedup se realizó respecto a la versión serial que tenía una duración de **378.25 seg**

Notamos que los tiempos de ejecución tienen una diferencia de 15 seg aproximadamente, por lo tanto deducimos que la concurrencia con OMP es la más óptima para este programa.

**Tiempos de ejecución**
| pThread | OpenMP |
|:-----:|:-------:|
| 50.73 seg  | 35.30 seg  |

![](https://i.imgur.com/ELz3ZHr.png)
