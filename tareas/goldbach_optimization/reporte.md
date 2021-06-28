# Reporte de eficiencia
---
# Optimización 1

**Tiempos de ejecución**
| Serial    | Tarea02 | Optimización 1 |
| --- |:--------:|:--------:|
|378.248 seg     |    161.857 seg   |     145.206 seg   |
 ---
# Opimización 2



En kCachegrind podemos ver que el método es_primo:

```
int64_t es_primo(int64_t num){
  if(num == 2){
    return 1;
  }//caso trivial
  if(num <= 1 || !(num & 1)){
    return 0;
  } //caso trivial
  for(int64_t impares = 3;impares*impares<=num;impares+=2){
    if(num%impares == 0){
      return 0;
    }
  }
  return 1;
}
```
se usa un total de **7967608** veces. En su mayoría de veces verificando los números primos que tienen en común. Por ejemplo, los números **20** y **11** tienen en común los primos **1,2,3,5 y 7**, el programa no tiene registro de cuáles números primos cononce.  

![](https://i.imgur.com/nQgyyRM.png)

Esta optimización se encargará de registrar estos primos y ahorrarnos el ciclo que los verifica.
```
for(int64_t impares = 3;impares*impares<=num;impares+=2){
    if(num%impares == 0){
      return 0;
    }
  }
```
**Tiempos de ejecución con Optimización 2**



| Antes | Después |
|:-----:|:-------:|
| 145.206 seg  | 50.726 seg  |
---
# Comparación 01: Optimizaciones


|   Tarea01   |   Tarea02   | Optimización 1 | Optimización 2 |
|:-----------:|:-----------:|:--------------:|:--------------:|
| 378.248 seg | 161.857 seg |  145.206 seg   |   50.726 seg   |

![](https://i.imgur.com/YKTP9h8.png)



---
# Comparación 02: grado de concurrencia

![](https://i.imgur.com/r2MhkXI.png)
