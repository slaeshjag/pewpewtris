default:
	@mkdir -p res
	@echo " [LDMZ] playfield_background.tmx"
	@darnit-tmxconv playfield_background.tmx res/playfield_background.ldmz 2>/dev/null
	@echo " [LDMZ] mainmenu_background.tmx"
	@darnit-tmxconv mainmenu_background.tmx res/mainmenu_background.ldmz 2>/dev/null
	@echo " [LDMZ] highscore_background.tmx"
	@darnit-tmxconv highscore_background.tmx res/highscore_background.ldmz 2>/dev/null
	@cp -f block.png res/
	@cp -f bullet.png res/
	@cp -f playfield_background.png res/
	@cp -f font.ttf res/
	@echo " [ LDI] res/"
	@darnit-fsimage res/ res.ldi
	@cat res.ldi >> ../bin/pewpewtris.elf
	@rm res.ldi

clean:
