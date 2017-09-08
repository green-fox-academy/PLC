/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOCKET_SERVER_H
#define __SOCKET_SERVER_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define SERVER_PORT			5656
#define SERVER_QUEUE_SIZE	2
#define SERVER_BUFF_LEN		100

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void socket_server_thread(void const *argument);

#endif /* __SOCKET_SERVER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
