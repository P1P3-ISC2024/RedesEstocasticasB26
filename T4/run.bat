@echo off
@echo off
set "fuente=MAIN.cpp"
set "salida=main"
if "%~1" NEQ "" (
    set "fuente=%1"
)
if "%~2" NEQ "" (
    set "salida=%2"
)
echo Compilando %fuente%...
cl /W4 /EHsc codigo\Exponencial\%fuente% /link /out:resultados\%salida%.exe
