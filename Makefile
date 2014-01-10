include config.mk
MAKEFLAGS	+=	--no-print-directory

TOPDIR		=	$(shell pwd)
export TOPDIR

default:
	@mkdir -p bin/
	@echo " [ CD ] src/"
	+@make -C src/
	@echo " [ CD ] res/"
	+@make -C res/
	@echo "Done."
	@echo  

pandora: default
	@echo " [ PND] pewpewtris.pnd"
	@cp -f release_data/pewpewtris_pandora.sh bin/
	@cp -f release_data/PXML.xml bin/
	@rm -f pewpewtris.pnd
	@mksquashfs bin/* pewpewtris.pnd >/dev/null
	@cat bin/PXML.xml >> pewpewtris.pnd
	@#@cat bin/icon.png >> pewpewtris.pnd
	@echo 


clean:
	@rm -Rf bin/
	@echo " [ CD ] src/"
	+@make -C src/ clean
	@echo " [ CD ] res/"
	+@make -C res clean
	@echo "Done."
	@echo 

