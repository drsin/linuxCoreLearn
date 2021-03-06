#include <stdio.h>
#include <stdarg.h>

void miniprintf(char fmt[], ...)\
	__attribute__((format(printf, 1, 2)));
void miniprintf(char fmt[], ...)
{
	va_list v;
	int i;
	
	va_start(v, fmt);
	for (i=0; fmt[i]!='\0'; i++) {
		switch ( fmt[i] ) {
			case '%':
			switch( fmt[++i] ) {
				case 'd': {
					int d = va_arg(v, int);
					printf("%d", d);
				}
				break;
				case 'f': {
					double d = va_arg(v, double);
					printf("%f", d);
				}
				break;
				case 'c': {
					int c = va_arg(v, int);
					printf("%c", c);
				}
				break;
				case 's': {
					char* p = va_arg(v, char*);
					printf("%s", p);
				}
			}
			break;
			default:
				printf("%c", fmt[i]);
			break;
		}
	}

	va_end(v);
}

int main( void )
{
	miniprintf("fuck %d\n", "canglaoshi");
		
	return 0;
}

