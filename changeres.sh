#/bin/bash

#| Listar modos CEA
#|  $ tvservice -m CEA
#| Listar modos DMT
#|  $ tvservice -m DMT
#| Estado actual
#|  $ tvservice -s
#| Cambiar a la resolución #1 en modo CEA
#|  $ tvservice -e "CEA 1"
#| Actualizar framebuffer
#|  $ fbset -depth 8 && fbset -depth 16

if [ $1 == "list" ]; then
		tvservice -m CEA
else
		tvservice -e "CEA $1"
		fbset -depth 8 && fbset -depth 16
fi
