buddy_gawk:
	@rm -f tests/gawk-3.1.8/custom_alloc.c
	@ln ./buddy_alloc.c tests/gawk-3.1.8/custom_alloc.c
	@cd tests/gawk-3.1.8/ && $(MAKE) check
