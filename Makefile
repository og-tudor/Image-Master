build:
	gcc -g -o image_editor image_editor.c header.h -lm
run:
	valgrind --leak-check=full ./image_editor
style:
	cs/cs.sh image_editor.c header.h
single_test:
	./image_editor < tasks/image_editor/tests/04-image_editor/04-image_editor.in > tasks/image_editor/tests/04-image_editor/04-image_editor.out
clean:
	rm image_editor