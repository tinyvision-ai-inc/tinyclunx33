all:
	rm -f pinout.svg pinout.png
	python3 -m pinout.manager --export pinout_diagram.py pinout.svg
	chromium --headless --screenshot --window-size=1512,1210 pinout.svg
	mv screenshot.png tinyCLUNX33_devkit_Rev2_pinout.png
