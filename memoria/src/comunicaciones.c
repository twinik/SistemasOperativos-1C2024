#include "../include/comunicaciones.h"

typedef struct
{
	t_log *log;
	int fd;
	char *server_name;
} t_procesar_conexion_args;

static void procesar_conexion(void *void_args)
{
	t_procesar_conexion_args *args = (t_procesar_conexion_args *)void_args;
	t_log *logger = args->log;
	int cliente_socket = args->fd;
	char *server_name = args->server_name;
	free(args);

	op_code cop;
	t_list *lista;
	while (cliente_socket != -1)
	{

		if (recv(cliente_socket, &cop, sizeof(op_code), 0) != sizeof(op_code))
		{
			log_info(logger, "DISCONNECT!");
			return;
		}

		switch (cop)
		{
		case MENSAJE:
			recibir_mensaje(cliente_socket, logger);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_socket);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void *)iterator);
			break;

		// Errores
		case -1:
			log_error(logger, "Cliente desconectado de %s...", server_name);
			return;
		default:
			log_error(logger, "Algo anduvo mal en el server de %s", server_name);
			log_info(logger, "Cop: %d", cop);
			return;
		}
	}

	log_warning(logger, "El cliente se desconecto de %s server", server_name);
	return;
}

int server_escuchar(t_log *logger, char *server_name, int server_socket)
{
	int cliente_socket = esperar_cliente(server_socket, logger);

	if (cliente_socket != -1)
	{
		pthread_t hilo;
		t_procesar_conexion_args *args = malloc(sizeof(t_procesar_conexion_args));
		args->log = logger;
		args->fd = cliente_socket;
		args->server_name = server_name;
		pthread_create(&hilo, NULL, (void *)procesar_conexion, (void *)args);
		pthread_detach(hilo);
		return 1;
	}
	return 0;
}