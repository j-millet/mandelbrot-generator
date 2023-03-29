all: mandelbrot

mandelbrot:
	gcc -o mandelbrot main.c -lm

clean:
	rm mandelbrot