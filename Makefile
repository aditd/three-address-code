A3_11: A3_11.l A3_11.y
	bison -d -v A3_11.y
	flex A3_11.l
	cc -o A3_11 A3_11.c lex.yy.c -ll
	echo "Starting the program"
	./A3_11 < A3_11.nc