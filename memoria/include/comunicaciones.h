#ifndef COMUNICACIONES_H_
#define COMUNICACIONES_H_

#include <stdlib.h>
#include <stdio.h>

#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <inttypes.h>
#include <readline/readline.h>

#include "../../utils/include/sockets_client.h"
#include "../../utils/include/sockets_server.h"
#include "../../utils/include/contexto.h"
#include "../include/gestor.h"

t_proceso_memoria *proceso_memoria;

int server_escuchar(t_log *logger, char *server_name, int server_socket);
t_proceso_memoria *recibir_proceso_memoria(int socket);
void extraer_de_paquete(t_paquete *paquete, void *destino, int size);
void recibir_pedido_instruccion(uint32_t *pid, uint32_t *pc, int socket);
t_proceso_memoria obtener_proceso_pid(uint32_t pid_pedido);
t_instruccion obtener_instruccion_del_proces_pc(t_proceso_memoria proceso, uint32_t pc);
char *obtener_nombre_instruccion(nombre_instruccion instruccion);
void enviar_instruccion(int socket, t_instruccion *instruccion);
void serializar_instruccion(t_paquete *paquete, t_instruccion *instruccion);
t_proceso_memoria *iniciar_proceso_path(t_proceso_memoria *proceso_nuevo);
t_list *parsear_instrucciones(char *path);
t_instruccion *armar_estructura_instruccion(nombre_instruccion instruccion, char *parametro1, char *parametro2);
char *leer_archivo(char *path);

#endif