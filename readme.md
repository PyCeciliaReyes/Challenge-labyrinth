Al arrancar el proyecto labyrinth, empece por separar en dos grandes bloques el proceso de ejecucion del juego. Por lo que quedo de la siguiente manera: 
1. **Generar un laberinto aleatorio.**
2. **Un algoritmo que pueda resolver el laberinto.**
Acontinuacion estare describiendo el paso a paso en cada bloque de solucion.
**Obs:** Cada función tiene un objetivo especifico dentro del proceso general de crear y resolver un laberinto.

# Parte 1: Generación del laberinto: Crear un laberinto aleatorio que sea lógico y tenga al menos una solución:
**Descripcion general:**
- Con las funciones generarCamino() y generarlaberinto(), abordo la primera parte del desafío: crear un programa en C++ que genere un laberinto aleatorio y lógico. Con estas funciones aseguro que el laberinto generado no tenga soluciones imposibles, ya que implemento el algoritmo de backtracking en la funcion generarCamino(), esta funcion explorará todas las posibles rutas para construir el laberinto.
- Estas funciones trabajan juntas para generar un laberinto aleatorio. generarlaberinto() establece el escenario y comienza el proceso, mientras que generarCamino() realiza el trabajo detallado de crear un camino a través del laberinto utilizando backtracking y recursividad.



# Parte 2: Solución del Laberinto. Desarrollar un algoritmo para resolver el laberinto generado:

**Desarrollar un algoritmo de solución:** Use el algoritmo búsqueda en anchura (BFS)
- **el algoritmo BFS que aplico para resolver eln laberinto,**  manteniendo un registro de las celdas visitadas y sus relaciones padre-hijo para reconstruir el camino al final.

- **Estructura de Datos de Cola**: La cola es vida o muerte para mantener el orden de exploración en BFS.

- **Reconstrucción del camino**: El camino se reconstruye retrocediendo desde la celda de destino hasta el punto de inicio, utilizando la matriz `padre`.

- **`vector<vector<bool>> visitado`**: esta matriz mantiene un registro de las celdas que ya han sido visitadas para evitar procesar una celda más de una vez.

- **`vector<vector<pair<int, int>>> padre`**: esta matriz almacena el nodo previo (o padre) de cada nodo en el camino, lo que es fundamental para poder reconstruir el camino al final del algoritmo.

