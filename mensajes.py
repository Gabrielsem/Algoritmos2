# Comandos a ejecutar para las distintas funciones
CMD_CAMINO = "camino"
CMD_CANCIONES_IMPORTANTES = "mas_importantes"
CMD_RECOMENDACION = "recomendacion"
CMD_CICLO = "ciclo"
CMD_RANGO = "rango"
CMD_CLUSTERING = "clustering"
CMD_INVALIDO = "ERROR: comando inválido ({})"

# Mensajes de salida y de error para las funciones en funciones_tp3
FLECHA = " --> "
SIN_RECORRIDO = "No se encontro recorrido"
NO_CANCION = "{} no es una canción válida."
NO_NUMERO = "{} no es una cantidad válida."

# Separador de canciones para los parámetros de entrada
SEPARADOR_CANCIONES = " >>>> "

# Mensajes de salida y error para la función camino de funciones_tp3 
CAM_ERROR = "Tanto el origen como el destino deben ser canciones."
CAM_MENSAJES = ["aparece en la playlist", "de", "tiene una playlist", "donde aparece"]\

# Separador para la salida de listas de usuarios o canciones
SEPARADOR = "; "

# Formato de salida para el resultado de la función clustering de funciones_tp3
CLUST_FORMATO = "{:.3f}"

# Formato de salida para las canciones (primero nombre de la canción, luego del arista)
FORMATO_CANCION = "{} - {}"

# Argumento para elegir que recomendar y mensajes de error en la función recomendacion de funciones_tp3
RECOM_CANCIONES = "canciones"
RECOM_USUARIOS = "usuarios"
RECOM_ERROR = f"Se debe especificar si recomendar \"{RECOM_CANCIONES}\" o \"{RECOM_USUARIOS}\""