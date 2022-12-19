build:
	gcc tema1.c  LinkedList.c LinkedList.h utils.h -g -o tema

clean:
	rm tema

checker:
	python3 checker.py
