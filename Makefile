A4_11: A4_11.l A4_11.y A4_11_test1.nc A4_11_translator.c
	@bison -d -v -Wcounterexamples A4_11.y
	@flex A4_11.l
	@cc -o A4_11 lex.yy.c A4_11.tab.c A4_11_translator.c -ll
	@echo "Starting program"
	@./A4_11 < A4_11_test1.nc > A4_11_quads1.output