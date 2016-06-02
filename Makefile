buddy_gawk:
	@rm -f tests/gawk-3.1.8/custom_alloc.c
	@ln ./buddy_alloc.c tests/gawk-3.1.8/custom_alloc.c
	@cd tests/gawk-3.1.8/ && $(MAKE) clean check

linked_gawk:
	@rm -f tests/gawk-3.1.8/custom_alloc.c
	@ln ./linked_alloc.c tests/gawk-3.1.8/custom_alloc.c
	@cd tests/gawk-3.1.8/ && $(MAKE) clean check

mini_test:
	rm -f ./test
	gcc -g -o ./test.out tests/mini_test.c buddy_alloc.c
	./test.out
