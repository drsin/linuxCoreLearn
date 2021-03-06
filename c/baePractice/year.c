#include <stdio.h>

typedef struct date
{
	int year;
	int month;
	int day;
}date;

//d=20140724 
date int2date(int d)
{
	date dt;

	dt.day = d%100;
	dt.year = d/10000;
	dt.month = d/100%100;
	
	return dt;
}

int date2int(date d)
{
	int id = d.year*10000 + d.month*100 + d.day;
	
	return id;
}

date input( void )
{
	date d;

	printf("year/month/day: ");

	scanf("%d/%d/%d", &d.year, &d.month, &d.day);

	return d;
}

void print( date d )
{
	printf("%d-%d-%d\n", d.year, d.month, d.day);
}

int main( void )
{
	date dt;

	dt = input();
	print(dt);
	
	dt = int2date(20140724);
	print(dt);
	printf("%d\n", date2int(dt));

	return 0;
}

