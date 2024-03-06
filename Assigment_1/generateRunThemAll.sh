#!/bin/bash

# Buscar archivos que coincidan con el patrón 'generateData*.sh' en el directorio actual
for script in generateData*.sh; do
    # Verificar si el archivo existe para evitar intentar leer algo que no es un archivo
    if [[ -f "$script" ]]; then
        # Buscar la palabra 'Change' dentro del archivo
        if ! grep -q 'Change' "$script"; then
            echo "*********************** Executing $script ***********************"
            # Ejecutar el script
            # "$script"
        fi
    else
        echo "No se encontraron archivos que coincidan con el patrón."
    fi
done