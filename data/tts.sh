#!/usr/bin/env bash

execLine() {
    echo "Ejecutar como: ./tts.sh [-h] <lang> <text> <filename>"
}

if [ $# -eq 0 ]; then
    execLine
    exit 1
fi

if [ $# -lt 3 ]; then
    if [ $1 = "-h" ]; then
        execLine
        echo "<lang>"
        echo "  Idioma de la voz soportada por el motor de Google."
        echo "  Algunas son: es (Español), de (Alemán), en (Inglés)."
        echo ""
        echo "<text>"
        echo "  El texto que queremos convertir a voz."
        echo ""
        echo "<filename>"
        echo "  El nombre del archivo resultante, con la extensión."
        exit 1
    fi
    execLine
    exit 1
fi

lang=$1
text="$2"
filename=$3
request="http://translate.google.com/translate_tts?ie=UTF-8&tl=$lang&q=${text// /%20}"

# Download text to speeched file
echo "[1/2] Downloading requested audio..."
wget -q -U Mozilla -O $filename $request

# Convert it to wav
echo "[2/2] Converting it to wav format..."
mplayer \
    -quiet \
    -vo null \
    -vc null \
    -af volume=0,resample=16000:0:1 \
    -format s16le \
    -ao pcm:waveheader:file=sounds/$filename.wav $filename

# Delete temp
rm $filename
