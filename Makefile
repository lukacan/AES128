all:	prog1 prog2 prog3 prog4 prog5 prog6
clean:
	rm *.o
prog1:  aes_NI.cpp
	g++ -Wall -pedantic -Ofast -maes aes_NI.cpp -o aes_NI.o -g
prog2:  aes_simple.cpp
	g++ -Wall -pedantic -Ofast -maes aes_simple.cpp -o aes_simple.o -g
prog3:  aes_NI.cpp
	g++ -Wall -pedantic -Ofast -maes aes_TBoxes.cpp -o aes_TBoxes.o -g
prog4:  aes_NI_Encrypt.cpp
	g++ -Wall -pedantic -Ofast -maes aes_NI_Encrypt.cpp -o aes_NI_Encrypt.o -g
prog5:  aes_simple_Encrypt.cpp
	g++ -Wall -pedantic -Ofast -maes aes_simple_Encrypt.cpp -o aes_simple_Encrypt.o -g
prog6:  aes_NI_Encrypt.cpp
	g++ -Wall -pedantic -Ofast -maes aes_TBoxes_Encrypt.cpp -o aes_TBoxes_Encrypt.o -g

test1: 	
	@./measure_time.sh 1
test2: 	
	@./measure_time.sh 1000
test3: 	
	@./measure_time.sh 100000
test4: 	
	@./measure_time.sh 1000000
AES_NI_E:
	@./aes_NI_Encrypt.o
AES_TBox_E:
	@./aes_TBoxes_Encrypt.o
AES_Simple_E:
	@./aes_simple_Encrypt.o
AES_ALL_E: AES_NI_E AES_TBox_E AES_Simple_E
