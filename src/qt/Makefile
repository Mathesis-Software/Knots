include ../config.mk

all:
	@make -C setValue all
	@make -C window all
	@make -C widget all
	@make -C manager all

clean:
	@make -C setValue .clean
	@make -C window .clean
	@make -C widget .clean
	@make -C manager .clean
	@$(RM) manager/KnotEditor
