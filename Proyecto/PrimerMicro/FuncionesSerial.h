#include <mega328p.h>
#include <io.h>

// -- Funciones lectura escritura UART -- //

// Leer
unsigned char UART_read(void)
{
	if(UCSR0A&(1<<7))						//si el bit7 del registro UCSR0A se ha puesto a 1
		return UDR0;						//devuelve el dato almacenado en el registro UDR0
	else
		return 0;
}

// Escribir data (Solo un char)
void UART_write(unsigned char caracter)
{
	while(!(UCSR0A&(1<<5)));				// mientras el registro UDR0 est? lleno espera
	UDR0 = caracter;						//cuando el el registro UDR0 est? vacio se envia el caracter
}

// Escribir data (Cadena de testo)
void UART_write_txt(char* cadena)			//cadena de caracteres de tipo char
{
	while(*cadena !=0x00)
	{
		UART_write(*cadena);				//transmite los caracteres de cadena
		cadena++;							//incrementa la ubicaci?n de los caracteres en cadena
											//para enviar el siguiente caracter de cadena
	}
}