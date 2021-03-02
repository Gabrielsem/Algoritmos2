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

# Separador de entrada y mensajes de salida y error para la función camino de funciones_tp3 
CAM_SEPARADOR = " >>>> "
CAM_ERROR = "Tanto el origen como el destino deben ser canciones."
CAM_MENSAJES = ["aparece en la playlist", "de", "tiene una playlist", "donde aparece"]\

# Separador para la salida de canciones de la función canciones_importantes de funciones_tp3
TOPC_SEPARADOR = "; "

# Formato de salida para el resultado de la función clustering de funciones_tp3
CLUST_FORMATO = "{:.3f}"

# Formato de salida para las canciones (primero nombre de la canción, luego del arista)
FORMATO_CANCION = "{} - {}"