include ../config.mk

all:
	@make -C setupColor all
	@make -C setValue all
	@make -C abstractWindow all
	@make -C diagram all
	@make -C gl all
	@make -C knot all
	@make -C manager all

clean:
	@make -C setupColor .clean
	@make -C setValue .clean
	@make -C abstractWindow .clean
	@make -C diagram .clean
	@make -C gl .clean
	@make -C knot .clean
	@make -C manager .clean
	@$(RM) manager/KnotEditor
