//incluyendo bibliotecas para funcionalidades:
#include <iostream> //para operaciones de entrada y salida. como cout, cin, endl.
#include <ctime> //para funciones relacionadas con el tiempo y la generacion de nros aleatorios
#include <vector> //para usar contenedores que facilitan el manejo de colecciones de datos. Los vectores son contenedores de datos dinamicos, similares a las listas en Python, pero con mas funcionalidades
#include <algorithm> //para operaciones como random_shuffle
#include <queue> //para usar la estructura de datos cola, util en el algoritmos de busqueda

using namespace std; //para evitar tener que escribir std:: antes de cada funcion estandar

//Declaracion de 3 funciones:
//esta funcion generará un camino dentro del laberinto
void generarCamino(int **matriz, int filas, int columnas, int x, int y);
//esta funcion generara un laberinto y retorna un puntero a un puntero de enteros, quiero usar para representar una matriz bidimensional
int** generarlaberinto(int filas, int columnas);
//función que resuelve el laberinto utilizando el algoritmo BFS. Retorna un booleano (true o false) que indica si se encontró un camino
bool resolverLaberintoDFS(int **matriz, int filas, int columnas, int x, int y, vector<pair<int, int>>& camino);

//PARTE 1: GENERACION DEL LABERINTO

//funcion para generar caminos en el laberinto y utilizo en la funcion generarlaberinto()
void generarCamino(int **matriz, int filas, int columnas, int x, int y) {
    //direcciones posibles para expandir el camino: arriba, abajo, izquierda y derecha. Las direcciones se representan como desplazamientos en x e y
    int dx[4] = {0, 0, -2, 2}; //desplazar en x para cada direccion
    int dy[4] = {-2, 2, 0, 0}; //desplazar en y para cada direccion
    vector<int> direcciones = {0, 1, 2, 3}; //lista de indices de direcciones para explorar
    random_shuffle(direcciones.begin(), direcciones.end()); // mezclar al vector direcciones con la funcion random_shuflle,asegurando asi que el camino generado sea aleatorio.

    //el bucle for explora cada direccion. Si la nueva posicion (nx, ny) es valida (dentro de los limites del laberinto y actualmente es una pared), la funcion se llama a si misma recursivamente desde esa nueva posicion, creando un camino.

    for (int i : direcciones) {
        //calcula las nuevas coordenadas despues de moverse en la direccion i.
        int nx = x + dx[i], ny = y + dy[i];

        //verifica si las nuevas coordenadas estan dentro de los limites del laberinto y si la celda esta sin visitar
        if (ny > 0 && ny < filas-1 && nx > 0 && nx < columnas-1 && matriz[ny][nx] == 1) {
            //marca la celda actual y la celda intermedia como parte del camino, cambiando su valor de 1 (pared) a 0 (camino)    
            matriz[ny][nx] = 0;
            matriz[y + dy[i]/2][x + dx[i]/2] = 0;
            generarCamino(matriz, filas, columnas, nx, ny); //continua generando el camino desde las nuevas coordenadas, utilizando la recursion

        }
    }
}

int** generarlaberinto(int filas, int columnas) {
    //matriz inicial
    int **matriz = new int *[filas];
    //bucle para inicializar cada fila de la matriz
    for (int i = 0; i < filas; i++) {
        //asignar a cada fila un array de enteros
        matriz[i] = new int[columnas];
        //bucle para inicializar cada columna en la fila actual
        for (int j = 0; j < columnas; j++) {
            // asignar el valor 1 a cada celda, representando una pared
            matriz[i][j] = 1;
        }
    }
    //inicializar el generador de números aleatorios
    srand(time(NULL));
    //seleccionar una posición inicial aleatoria para el camino en el laberinto. inicioX - inicioY, segura que la posición inicial sea impar, para evitar que los caminos se corten entre si
    int inicioX = 2 * (rand() % ((columnas - 1) / 2)) + 1;
    int inicioY = 2 * (rand() % ((filas - 1) / 2)) + 1;
    //marco la posicion inicial como espacio libre (0 representa espacio libre)
    matriz[inicioY][inicioX] = 0;
    //invoco a la funcion 'generarCamino' para generar caminos desde la posicion inicial
    generarCamino(matriz, filas, columnas, inicioX, inicioY); //modifica la matriz para crear un laberinto

    //crear un entrada y salida en el laberinto
    matriz[0][inicioX] = 0; //entrada en la fila superior
    matriz[filas - 1][inicioX] = 0; //salida en la fila inferior

    return matriz; //retornar la matriz generada
}


//PARTE 2: SOLUCION DEL LABERINTO

//funcion para verificar si una posicion (x, y) esta dentro del laberinto y no es una pared. Utilizo dentro de la funcion 'resolverLaberintoBFS'
bool esValido(int x, int y, int filas, int columnas) {
    //retorna verdadero si la posicion esta dentro de los limites del laberinto
    return x >= 0 && x < columnas && y >= 0 && y < filas;
}

//funcion para resolver el laberinto usando BFS
vector<pair<int, int>> resolverLaberintoBFS(int **laberinto, int filas, int columnas) {
    //direccion: Arriba, Abajo, Izquierda, Derecha
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    //matriz para llevar registro de las celdas ya visitadas
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));

    //cola para manejar el orden en el que se visitan las celdas
    queue<pair<int, int>> cola;

    //inicializar la cola con la posicion de inicio y marcarla como visitada
    cola.push({0, 1}); //entrada del laberinto esta en (0, 1)
    visitado[0][1] = true;

    //matriz para mantener registro del "padre" de cada celda, es decir, la celda desde la que se llego
    vector<vector<pair<int, int>>> padre(filas, vector<pair<int, int>>(columnas, {-1, -1}));

    //bucle while (mientras) haya elementos en la cola
    while (!cola.empty()) {
        //tomar el elemento frontal de la cola y eliminarlo de la cola
        pair<int, int> actual = cola.front();
        cola.pop();

        //x e y son las coordenadas actuales
        int x = actual.first, y = actual.second;

        //verificar si llegamos al final del laberinto
        if (x == filas - 1) {
            //si se ha llegado al final, construir y retornar el camino
            vector<pair<int, int>> camino;
            while (!(x == -1 && y == -1)) {
                camino.push_back({x, y});
                pair<int, int> p = padre[x][y];
                x = p.first;
                y = p.second;
            }
            reverse(camino.begin(), camino.end());
            return camino;
        }

        //explorar celdas vecinas
        for (int i = 0; i < 4; ++i) {
            //nx y ny representan las nuevas coordenadas despues de moverse
            int nx = x + dx[i], ny = y + dy[i];
            //comprobar si la nueva posicion es valida, esta libre (laberinto[nx][ny] == 0) y no ha sido visitada
            if (esValido(nx, ny, filas, columnas) && laberinto[nx][ny] == 0 && !visitado[nx][ny]) {
                //marcar la nueva posicion como visitada
                visitado[nx][ny] = true;
                //anhadir la nueva posicion a la cola para su posterior exploracion
                cola.push({nx, ny});
                //registrar la celda actual como el "padre" de la nueva posicion
                padre[nx][ny] = {x, y};
            }
        }
    }
    //si se sale del bucle sin encontrar un camino, retornar un vector vacio. esto indica que no hay un camino valido desde la entrada hasta el final del laberinto
    return {};
}

//funcion para imprimir el laberinto con el rastro del caminito de Entrada/Salida en la consola. Utilizo en main()
void imprimirLaberinto(int **laberinto, int filas, int columnas) {
    //bucle for para recorrer cada fila del laberinto
    for (int i = 0; i < filas; ++i) {
        //bucle for para recorrer cada columna de la fila actual
        for (int j = 0; j < columnas; ++j) {
            //si el valor en la celda actual es 1, representa una pared (imprime '■■')
            //si el valor en la celda es 2, representa el camino de salida (imprime '++')
            //de lo contrario (0), representa un camino libre (imprime '  ')
            cout << (laberinto[i][j] == 1 ? "■■" : (laberinto[i][j] == 2 ? "++" : "  "));
        }
        //al final de cada fila, imprimir un salto de linea para iniciar la siguiente fila
        cout << "\n";
    }
}


//Ejecutando el programa
int main() {
    //declarar variables para filas y columnas del laberinto
    int filas, columnas;

    //pedir al usuario que ingrese el numero de filas y columnas del laberinto
    cout << "Ingrese el número de filas del laberinto: ";
    cin >> filas;

    cout << "Ingrese el número de columnas del laberinto: ";
    cin >> columnas;
    
    //invoco a la funcion generarlaberinto para crear el laberinto
    int **laberinto = generarlaberinto(filas, columnas);
    
    //invoco a resolverLaberintoBFS para encontrar un camino en el laberinto
    //retorna un vector de pares que representan las coordenadas del camino
    vector<pair<int, int>> camino = resolverLaberintoBFS(laberinto, filas, columnas);
    //verificar si se encontro un camino
    if (!camino.empty()) {
        //si se encontro un camino, marcar en el laberinto
        //el 2 representa el camino en la funcion imprimirLaberinto()
        for (auto p : camino) {
            laberinto[p.first][p.second] = 2; 
        }
    } else {
        //si no se encontro un camino, imprimir un mensaje
        cout << "No se encontro camino" << endl;
    }

    //invocar a imprimirLaberinto() para mostrar el laberinto y el camino de solucion si lo hubiere
    imprimirLaberinto(laberinto, filas, columnas);

    //liberar la memoria asignada para el laberinto, ya que uso punteros y memorias dinamicas para evitar fugas
    for (int i = 0; i < filas; ++i) {
        delete[] laberinto[i];
    }
    delete[] laberinto;
    
    //finalizar la funcion main() con un retorno de 0, indicando que todo salio exito.
    return 0;
}