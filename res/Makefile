default:
	@mkdir -p res
	@echo " [LDMZ] playfield_background.tmx"
	@darnit-tmxconv playfield_background.tmx res/playfield_background.ldmz 2>/dev/null
	@cp -f block.png res/
	@cp -f playfield_background.png res/
	@echo " [ LDI] res/"
	@darnit-fsimage res/ res.ldi
	@cat res.ldi >> ../bin/pewpewtris.elf
	@rm res.ldi

clean:
