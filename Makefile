default:
	@echo " [ CD ] src/"
	+@make -C src/
	@echo "Done."
	@echo  

clean:
	+@echo " [ CD ] src/"
	@make -C src/ clean
	@echo "Done."
	@echo 

