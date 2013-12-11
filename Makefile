include config.mk
MAKEFLAGS	+=	--no-print-directory

TOPDIR		=	$(shell pwd)
export TOPDIR

default:
	@echo " [ CD ] src/"
	+@make -C src/
	@echo " [ CD ] res/"
	+@make -C res/
	@echo "Done."
	@echo  

clean:
	@echo " [ CD ] src/"
	+@make -C src/ clean
	@echo " [ CD ] res/"
	+@make -C res clean
	@echo "Done."
	@echo 

