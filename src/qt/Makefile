include ../config.mk

all:
	@make -C widget all
	@make -C window all
	@make -C manager all

clean:
	@make -C widget .clean
	@make -C window .clean
	@make -C manager .clean
	@$(RM) manager/KnotEditor
