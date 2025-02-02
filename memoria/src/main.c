#include "../include/main.h"

char *PUERTO_ESCUCHA_MEMORIA;
char *IP_MEMORIA;
int TAM_MEMORIA;
int TAM_PAGINA;
char *PATH_INSTRUCCIONES;
int RETARDO_RESPUESTA;
int CLIENTE_CPU, CLIENTE_KERNEL, CLIENTE_IN_OU;

t_log *LOGGER_MEMORIA;
t_config *CONFIG_MEMORIA;

int fd_memoria;

pthread_mutex_t mutex_comunicacion_procesos;
pthread_mutex_t mutex_procesos;
pthread_mutex_t mutex_memoria_usuario;

void *memoriaUsuario;
uint32_t tamanioMemoria;
uint32_t valorGlobalDescritura;
uint32_t valorTotalaDeLeer;

t_list *marcosPaginas;
t_list *procesos_totales;

int main(int argc, char **argv)
{
	inicializar_config(argv[1]);
	iniciar_semaforos();
	iniciar_memoria_usuario();

	fd_memoria = iniciar_servidor(LOGGER_MEMORIA, "MEMORIA", IP_MEMORIA, PUERTO_ESCUCHA_MEMORIA);

	while (server_escuchar(LOGGER_MEMORIA, "MEMORIA", fd_memoria));

	terminar_programa(fd_memoria, LOGGER_MEMORIA, CONFIG_MEMORIA);

	return 0;
}

void inicializar_config(char *arg)
{
	char config_path[256];
	strcpy(config_path, "./config/");
	strcat(config_path, arg);
	strcat(config_path, ".config");

	LOGGER_MEMORIA = iniciar_logger("memoria.log", "Servidor Memoria");
	CONFIG_MEMORIA = iniciar_config(config_path, "MEMORIA");
	PUERTO_ESCUCHA_MEMORIA = config_get_string_value(CONFIG_MEMORIA, "PUERTO_ESCUCHA");
	TAM_MEMORIA = config_get_int_value(CONFIG_MEMORIA, "TAM_MEMORIA");
	TAM_PAGINA = config_get_int_value(CONFIG_MEMORIA, "TAM_PAGINA");
	PATH_INSTRUCCIONES = config_get_string_value(CONFIG_MEMORIA, "PATH_INSTRUCCIONES");
	RETARDO_RESPUESTA = config_get_int_value(CONFIG_MEMORIA, "RETARDO_RESPUESTA");
	IP_MEMORIA = config_get_string_value(CONFIG_MEMORIA, "IP_MEMORIA");
}

void iniciar_memoria_usuario()
{
	memoriaUsuario = malloc(TAM_MEMORIA);
	memset(memoriaUsuario, 0, TAM_MEMORIA);
	tamanioMemoria = TAM_MEMORIA;
	if (memoriaUsuario == NULL)
	{
		log_error(LOGGER_MEMORIA, "Error al reservar memoria para el usuario");
		exit(EXIT_FAILURE);
	}

	marcosPaginas = list_create();
	procesos_totales = list_create();

	iniciar_marcos();
}

void iniciar_marcos()
{
	int cantidadMarcos = TAM_MEMORIA / TAM_PAGINA;
	for (int i = 0; i < cantidadMarcos; i++)
	{
		t_marco *marco = malloc(sizeof(t_marco));
		marco->numeroMarco = i;
		marco->pid = -1;
		list_add(marcosPaginas, marco);
	}
}