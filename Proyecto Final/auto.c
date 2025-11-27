#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "auto.h"

#define ARCHIVO_AUTOS "autos.bin"

int ingresar_entero(char mensaje[])
{
    char buffer[50];
    int valido;
    int i;
    int numeroFinal = 0;

    do
    {
        valido = 1;
        printf("%s", mensaje);
        fflush(stdin);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            valido = 0;
        }
        else
        {
            // 1. Detectar si el usuario escribio mas de lo que entra en el buffer
            int len = strlen(buffer);

            // Si el buffer esta lleno y NO termina en enter, sobraron caracteres en el teclado
            if (len == sizeof(buffer) - 1 && buffer[len - 1] != '\n')
            {
                // Limpiamos el sobrante del teclado para que no rompa el siguiente ciclo
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }

            // 2. Limpiar el \n si existe
            if (len > 0 && buffer[len - 1] == '\n')
            {
                buffer[len - 1] = '\0';
                len--;
            }

            if(len == 0) // Si apreto Enter sin nada
            {
                valido = 0;
            }
            else
            {
                // Verificamos que sean solo numeros
                for(i = 0; i < len; i++)
                {
                    if(buffer[i] < '0' || buffer[i] > '9')
                    {
                        valido = 0;
                        break;
                    }
                }
            }
        }

        if(valido == 0)
        {
            printf("ERROR: Debe ingresar solo numeros enteros validos.\n");
        }

    }
    while(valido == 0);

// Recorremos la cadena validada y hacemos la conversión manual
    for (i = 0; buffer[i] != '\0'; i++)
    {
        // 1. Multiplicamos el número actual por 10 para hacer espacio para el nuevo dígito.
        numeroFinal *= 10;

        // 2. Convertimos el carácter a su valor numérico restándole '0' (ASCII).
        // Por ejemplo: '5' - '0' = 5
        numeroFinal += (buffer[i] - '0');
    }

    return numeroFinal;
}

float ingresar_float(char mensaje[])
{
    char buffer[50];
    int valido;
    int i;
    int puntos;
    float numeroFinal = 0;

    do
    {
        valido = 1;
        puntos = 0;
        printf("%s", mensaje);
        fflush(stdin);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            valido = 0;
        }
        else
        {
            int len = strlen(buffer);

            if (len > 0 && buffer[len - 1] == '\n')
            {
                buffer[len - 1] = '\0';
            }

            if(strlen(buffer) == 0)
            {
                valido = 0;
            }

            for(i = 0; i < strlen(buffer); i++)
            {
                if(buffer[i] == '.')
                {
                    puntos++;
                    if(puntos > 1)
                    {
                        valido = 0;
                        break;
                    }
                }
                else if(buffer[i] < '0' || buffer[i] > '9')
                {
                    valido = 0;
                    break;
                }
            }
        }

        if(valido == 0)
        {
            printf("ERROR: Debe ingresar un precio valido.\n");
        }

    }
    while(valido == 0);

    sscanf(buffer, "%f", &numeroFinal);

    return numeroFinal;
}

int existe_patente_en_archivo(char patenteBuscada[])
{
    FILE *file = fopen(ARCHIVO_AUTOS, "rb");
    if(file == NULL)
    {
        return 0;
    }

    Auto a;
    while(fread(&a, sizeof(Auto), 1, file) == 1)
    {
        if(strcmp(a.patente, patenteBuscada) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}


int es_marca_valida(char m[])
{
    /// Declara una variable auxiliar para no tocar la original 'm'
    char temp[50];
    /// Copia el contenido de 'm' dentro de 'temp'
    strcpy(temp, m);

    /// Recorre letra por letra la palabra copiada
    for(int i=0; i<strlen(temp); i++)
    {
        /// Convierte cada letra minúscula a MAYÚSCULA
        temp[i] = toupper(temp[i]);
    }

    // --- MARCAS GENERALES ---
    /// Compara si la palabra en 'temp' es idéntica a "FORD"
    if(strcmp(temp, "FORD") == 0)
    {
        return 1; /// Si es igual, devuelve 1 (Verdadero) y termina la función acá
    }
    if(strcmp(temp, "CHEVROLET") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "FIAT") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "VOLKSWAGEN") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "VW") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "TOYOTA") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "RENAULT") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "PEUGEOT") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "HONDA") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "NISSAN") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "CITROEN") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "JEEP") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "DODGE") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "RAM") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "HYUNDAI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "KIA") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "MAZDA") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "SUBARU") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "MITSUBISHI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "SUZUKI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "CHERY") == 0)
    {
        return 1;
    }

    // --- MARCAS PREMIUM Y LUJO ---
    if(strcmp(temp, "BMW") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "MERCEDES BENZ") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "MERCEDES") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "AUDI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "LEXUS") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "VOLVO") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "ALFA ROMEO") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "JAGUAR") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "LAND ROVER") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "MINI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "DS") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "PORSCHE") == 0)
    {
        return 1;
    }

    // --- SUPERDEPORTIVOS Y EXOTICOS ---
    if(strcmp(temp, "FERRARI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "LAMBORGHINI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "MCLAREN") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "MASERATI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "ASTON MARTIN") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "ROLLS ROYCE") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "BENTLEY") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "BUGATTI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "PAGANI") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "KOENIGSEGG") == 0)
    {
        return 1;
    }
    if(strcmp(temp, "TESLA") == 0)
    {
        return 1;
    }

    return 0;
}


// --- LOGICA COMPLETA DE MODELOS ---
int es_modelo_valido(char marcaElegida[], char modeloIngresado[])
{
    char m[50], mod[50];
    strcpy(m, marcaElegida);
    strcpy(mod, modeloIngresado);

    // Convertir a mayusculas
    for(int i=0; i<strlen(m); i++)
    {
        m[i] = toupper(m[i]);
    }
    for(int i=0; i<strlen(mod); i++)
    {
        mod[i] = toupper(mod[i]);
    }
    // 1. FORD
    if(strcmp(m, "FORD") == 0)
    {
        if(strcmp(mod, "RANGER")==0 || strcmp(mod, "FIESTA")==0 || strcmp(mod, "FOCUS")==0 || strcmp(mod, "KA")==0 ||
                strcmp(mod, "MUSTANG")==0 || strcmp(mod, "ECOSPORT")==0 || strcmp(mod, "TERRITORY")==0 || strcmp(mod, "BRONCO")==0 ||
                strcmp(mod, "MAVERICK")==0 || strcmp(mod, "F-150")==0 || strcmp(mod, "KUGA")==0 || strcmp(mod, "MONDEO")==0)
        {
            return 1;
        }
    }
    // 2. CHEVROLET
    else if(strcmp(m, "CHEVROLET") == 0)
    {
        if(strcmp(mod, "CRUZE")==0 || strcmp(mod, "ONIX")==0 || strcmp(mod, "S10")==0 || strcmp(mod, "CAMARO")==0 ||
                strcmp(mod, "TRACKER")==0 || strcmp(mod, "SPARK")==0 || strcmp(mod, "PRISMA")==0 || strcmp(mod, "EQUINOX")==0 ||
                strcmp(mod, "TRAILBLAZER")==0 || strcmp(mod, "CORVETTE")==0 || strcmp(mod, "AVEO")==0 || strcmp(mod, "SPIN")==0)
        {
            return 1;
        }
    }
    // 3. FIAT
    else if(strcmp(m, "FIAT") == 0)
    {
        if(strcmp(mod, "CRONOS")==0 || strcmp(mod, "ARGO")==0 || strcmp(mod, "TORO")==0 || strcmp(mod, "STRADA")==0 ||
                strcmp(mod, "PULSE")==0 || strcmp(mod, "MOBI")==0 || strcmp(mod, "500")==0 || strcmp(mod, "PUNTO")==0 ||
                strcmp(mod, "PALIO")==0 || strcmp(mod, "SIENA")==0 || strcmp(mod, "FASTBACK")==0 || strcmp(mod, "TITANO")==0)
        {
            return 1;
        }
    }
    // 4. PEUGEOT
    else if(strcmp(m, "PEUGEOT") == 0)
    {
        if(strcmp(mod, "208")==0 || strcmp(mod, "2008")==0 || strcmp(mod, "308")==0 || strcmp(mod, "408")==0 ||
                strcmp(mod, "3008")==0 || strcmp(mod, "5008")==0 || strcmp(mod, "PARTNER")==0 || strcmp(mod, "206")==0 ||
                strcmp(mod, "207")==0)
        {
            return 1;
        }
    }
    // 5. VOLKSWAGEN
    else if(strcmp(m, "VOLKSWAGEN") == 0 || strcmp(m, "VW") == 0)
    {
        if(strcmp(mod, "GOL")==0 || strcmp(mod, "GOL TREND")==0 || strcmp(mod, "AMAROK")==0 || strcmp(mod, "POLO")==0 ||
                strcmp(mod, "VENTO")==0 || strcmp(mod, "TAOS")==0 || strcmp(mod, "TIGUAN")==0 || strcmp(mod, "T-CROSS")==0 ||
                strcmp(mod, "NIVUS")==0 || strcmp(mod, "VIRTUS")==0 || strcmp(mod, "UP")==0 || strcmp(mod, "FOX")==0 ||
                strcmp(mod, "SURAN")==0 || strcmp(mod, "SCIROCCO")==0 || strcmp(mod, "BEETLE")==0)
        {
            return 1;
        }
    }
    // 6. TOYOTA
    else if(strcmp(m, "TOYOTA") == 0)
    {
        if(strcmp(mod, "HILUX")==0 || strcmp(mod, "COROLLA")==0 || strcmp(mod, "ETIOS")==0 || strcmp(mod, "YARIS")==0 ||
                strcmp(mod, "SW4")==0 || strcmp(mod, "COROLLA CROSS")==0 || strcmp(mod, "RAV4")==0 || strcmp(mod, "PRIUS")==0 ||
                strcmp(mod, "CAMRY")==0 || strcmp(mod, "86")==0 || strcmp(mod, "LAND CRUISER")==0 || strcmp(mod, "HIACE")==0)
        {
            return 1;
        }
    }
    // 7. RENAULT
    else if(strcmp(m, "RENAULT") == 0)
    {
        if(strcmp(mod, "SANDERO")==0 || strcmp(mod, "LOGAN")==0 || strcmp(mod, "KANGOO")==0 || strcmp(mod, "DUSTER")==0 ||
                strcmp(mod, "KWID")==0 || strcmp(mod, "ALASKAN")==0 || strcmp(mod, "OROCH")==0 || strcmp(mod, "STEPWAY")==0 ||
                strcmp(mod, "KOLEOS")==0 || strcmp(mod, "CAPTUR")==0 || strcmp(mod, "CLIO")==0 || strcmp(mod, "MEGANE")==0)
        {
            return 1;
        }
    }
    // 8. NISSAN
    else if(strcmp(m, "NISSAN") == 0)
    {
        if(strcmp(mod, "VERSA")==0 || strcmp(mod, "SENTRA")==0 || strcmp(mod, "KICKS")==0 || strcmp(mod, "FRONTIER")==0 ||
                strcmp(mod, "X-TRAIL")==0 || strcmp(mod, "NOTE")==0 || strcmp(mod, "MARCH")==0 || strcmp(mod, "LEAF")==0)
        {
            return 1;
        }
    }
    // 9. HONDA
    else if(strcmp(m, "HONDA") == 0)
    {
        if(strcmp(mod, "CIVIC")==0 || strcmp(mod, "FIT")==0 || strcmp(mod, "HR-V")==0 || strcmp(mod, "CR-V")==0 ||
                strcmp(mod, "WR-V")==0 || strcmp(mod, "ZR-V")==0 || strcmp(mod, "CITY")==0 || strcmp(mod, "PILOT")==0)
        {
            return 1;
        }
    }
    // 10. CITROEN
    else if(strcmp(m, "CITROEN") == 0)
    {
        if(strcmp(mod, "C3")==0 || strcmp(mod, "C4")==0 || strcmp(mod, "BERLINGO")==0 || strcmp(mod, "C4 CACTUS")==0 ||
                strcmp(mod, "C5 AIRCROSS")==0 || strcmp(mod, "CELYSEE")==0)
        {
            return 1;
        }
    }
    // 11. JEEP / RAM / DODGE
    else if(strcmp(m, "JEEP") == 0)
    {
        if(strcmp(mod, "RENEGADE")==0 || strcmp(mod, "COMPASS")==0 || strcmp(mod, "WRANGLER")==0 ||
                strcmp(mod, "COMMANDER")==0 || strcmp(mod, "GRAND CHEROKEE")==0 || strcmp(mod, "GLADIATOR")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "RAM") == 0 || strcmp(m, "DODGE") == 0)
    {
        if(strcmp(mod, "1500")==0 || strcmp(mod, "2500")==0 || strcmp(mod, "RAMPAGE")==0 || strcmp(mod, "JOURNEY")==0 ||
                strcmp(mod, "CHALLENGER")==0 || strcmp(mod, "CHARGER")==0)
        {
            return 1;
        }
    }
    // 12. ASIATICAS (HYUNDAI / KIA)
    else if(strcmp(m, "HYUNDAI") == 0)
    {
        if(strcmp(mod, "TUCSON")==0 || strcmp(mod, "CRETA")==0 || strcmp(mod, "SANTA FE")==0 || strcmp(mod, "I10")==0 ||
                strcmp(mod, "I30")==0 || strcmp(mod, "VELOSTER")==0 || strcmp(mod, "KONA")==0 || strcmp(mod, "H1")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "KIA") == 0)
    {
        if(strcmp(mod, "SPORTAGE")==0 || strcmp(mod, "SELTOS")==0 || strcmp(mod, "RIO")==0 || strcmp(mod, "PICANTO")==0 ||
                strcmp(mod, "CERATO")==0 || strcmp(mod, "SORENTO")==0 || strcmp(mod, "CARNIVAL")==0 || strcmp(mod, "SOUL")==0)
        {
            return 1;
        }
    }
    // 13. ALTA GAMA (BMW / MERCEDES / AUDI)
    else if(strcmp(m, "BMW") == 0)
    {
        if(strcmp(mod, "X1")==0 || strcmp(mod, "X3")==0 || strcmp(mod, "X4")==0 || strcmp(mod, "X5")==0 || strcmp(mod, "X6")==0 ||
                strcmp(mod, "SERIE 1")==0 || strcmp(mod, "SERIE 2")==0 || strcmp(mod, "SERIE 3")==0 || strcmp(mod, "SERIE 4")==0 ||
                strcmp(mod, "SERIE 5")==0 || strcmp(mod, "M3")==0 || strcmp(mod, "M4")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "MERCEDES BENZ") == 0 || strcmp(m, "MERCEDES") == 0)
    {
        if(strcmp(mod, "CLASE A")==0 || strcmp(mod, "CLASE C")==0 || strcmp(mod, "CLASE E")==0 || strcmp(mod, "GLA")==0 ||
                strcmp(mod, "GLB")==0 || strcmp(mod, "GLC")==0 || strcmp(mod, "GLE")==0 || strcmp(mod, "SPRINTER")==0 || strcmp(mod, "VITO")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "AUDI") == 0)
    {
        if(strcmp(mod, "A1")==0 || strcmp(mod, "A3")==0 || strcmp(mod, "A4")==0 || strcmp(mod, "A5")==0 || strcmp(mod, "Q2")==0 ||
                strcmp(mod, "Q3")==0 || strcmp(mod, "Q5")==0 || strcmp(mod, "Q7")==0 || strcmp(mod, "Q8")==0 || strcmp(mod, "TT")==0)
        {
            return 1;
        }
    }
    // 14. TESLA
    else if(strcmp(m, "TESLA") == 0)
    {
        if(strcmp(mod, "MODEL 3")==0 || strcmp(mod, "MODEL Y")==0 || strcmp(mod, "MODEL S")==0 ||
                strcmp(mod, "MODEL X")==0 || strcmp(mod, "CYBERTRUCK")==0)
        {
            return 1;
        }
    }
    // 15. VOLVO
    else if(strcmp(m, "VOLVO") == 0)
    {
        if(strcmp(mod, "XC40")==0 || strcmp(mod, "XC60")==0 || strcmp(mod, "XC90")==0 ||
                strcmp(mod, "S60")==0 || strcmp(mod, "V40")==0 || strcmp(mod, "C40")==0)
        {
            return 1;
        }
    }
    // 16. LEXUS
    else if(strcmp(m, "LEXUS") == 0)
    {
        if(strcmp(mod, "NX")==0 || strcmp(mod, "RX")==0 || strcmp(mod, "UX")==0 ||
                strcmp(mod, "IS")==0 || strcmp(mod, "ES")==0 || strcmp(mod, "LX")==0)
        {
            return 1;
        }
    }
    // 17. ALFA ROMEO
    else if(strcmp(m, "ALFA ROMEO") == 0)
    {
        if(strcmp(mod, "GIULIA")==0 || strcmp(mod, "STELVIO")==0 || strcmp(mod, "TONALE")==0 ||
                strcmp(mod, "GIULIETTA")==0 || strcmp(mod, "MITO")==0 || strcmp(mod, "4C")==0)
        {
            return 1;
        }
    }
    // 18. MINI
    else if(strcmp(m, "MINI") == 0)
    {
        if(strcmp(mod, "COOPER")==0 || strcmp(mod, "COUNTRYMAN")==0 || strcmp(mod, "CLUBMAN")==0)
        {
            return 1;
        }
    }
    // 19. LAND ROVER & JAGUAR
    else if(strcmp(m, "LAND ROVER") == 0)
    {
        if(strcmp(mod, "RANGE ROVER")==0 || strcmp(mod, "EVOQUE")==0 || strcmp(mod, "DEFENDER")==0 ||
                strcmp(mod, "DISCOVERY")==0 || strcmp(mod, "VELAR")==0 || strcmp(mod, "SPORT")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "JAGUAR") == 0)
    {
        if(strcmp(mod, "F-PACE")==0 || strcmp(mod, "E-PACE")==0 || strcmp(mod, "XE")==0 ||
                strcmp(mod, "XF")==0 || strcmp(mod, "F-TYPE")==0)
        {
            return 1;
        }
    }
    // 20. MASERATI
    else if(strcmp(m, "MASERATI") == 0)
    {
        if(strcmp(mod, "GHIBLI")==0 || strcmp(mod, "LEVANTE")==0 || strcmp(mod, "GRECALE")==0 ||
                strcmp(mod, "GRANTURISMO")==0 || strcmp(mod, "MC20")==0 || strcmp(mod, "QUATTROPORTE")==0)
        {
            return 1;
        }
    }
    // 21. ASTON MARTIN
    else if(strcmp(m, "ASTON MARTIN") == 0)
    {
        if(strcmp(mod, "DB11")==0 || strcmp(mod, "DB12")==0 || strcmp(mod, "VANTAGE")==0 ||
                strcmp(mod, "DBX")==0 || strcmp(mod, "VALHALLA")==0 || strcmp(mod, "DBS")==0)
        {
            return 1;
        }
    }
    // 22. BENTLEY & ROLLS ROYCE
    else if(strcmp(m, "BENTLEY") == 0)
    {
        if(strcmp(mod, "CONTINENTAL")==0 || strcmp(mod, "BENTAYGA")==0 || strcmp(mod, "FLYING SPUR")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "ROLLS ROYCE") == 0)
    {
        if(strcmp(mod, "PHANTOM")==0 || strcmp(mod, "GHOST")==0 || strcmp(mod, "CULLINAN")==0 ||
                strcmp(mod, "WRAITH")==0 || strcmp(mod, "DAWN")==0 || strcmp(mod, "SPECTRE")==0)
        {
            return 1;
        }
    }
    // 23. PORSCHE
    else if(strcmp(m, "PORSCHE") == 0)
    {
        if(strcmp(mod, "911")==0 || strcmp(mod, "CAYENNE")==0 || strcmp(mod, "MACAN")==0 ||
                strcmp(mod, "PANAMERA")==0 || strcmp(mod, "718")==0 || strcmp(mod, "BOXSTER")==0 || strcmp(mod, "TAYCAN")==0)
        {
            return 1;
        }
    }
    // 24. FERRARI
    else if(strcmp(m, "FERRARI") == 0)
    {
        if(strcmp(mod, "488")==0 || strcmp(mod, "F8")==0 || strcmp(mod, "ROMA")==0 ||
                strcmp(mod, "PORTOFINO")==0 || strcmp(mod, "296")==0 || strcmp(mod, "SF90")==0 ||
                strcmp(mod, "PUROSANGUE")==0 || strcmp(mod, "LAFERRARI")==0 || strcmp(mod, "ENZO")==0 ||
                strcmp(mod, "TESTAROSSA")==0 || strcmp(mod, "F40")==0 || strcmp(mod, "812")==0 ||
                strcmp(mod, "12CILINDRI")==0)
        {
            return 1;
        }
    }
    // 25. LAMBORGHINI
    else if(strcmp(m, "LAMBORGHINI") == 0)
    {
        if(strcmp(mod, "AVENTADOR")==0 || strcmp(mod, "HURACAN")==0 || strcmp(mod, "URUS")==0 ||
                strcmp(mod, "REVUELTO")==0 || strcmp(mod, "GALLARDO")==0 || strcmp(mod, "MURCIELAGO")==0 ||
                strcmp(mod, "DIABLO")==0 || strcmp(mod, "COUNTACH")==0)
        {
            return 1;
        }
    }
    // 26. HIPERAUTOS
    else if(strcmp(m, "MCLAREN") == 0)
    {
        if(strcmp(mod, "720S")==0 || strcmp(mod, "765LT")==0 || strcmp(mod, "GT")==0 ||
                strcmp(mod, "ARTURA")==0 || strcmp(mod, "P1")==0 || strcmp(mod, "SENNA")==0 ||
                strcmp(mod, "570S")==0 || strcmp(mod, "W1")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "BUGATTI") == 0)
    {
        if(strcmp(mod, "CHIRON")==0 || strcmp(mod, "VEYRON")==0 || strcmp(mod, "DIVO")==0 ||
                strcmp(mod, "BOLIDE")==0 || strcmp(mod, "TOURBILLON")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "PAGANI") == 0)
    {
        if(strcmp(mod, "HUAYRA")==0 || strcmp(mod, "ZONDA")==0 || strcmp(mod, "UTOPIA")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "KOENIGSEGG") == 0)
    {
        if(strcmp(mod, "JESKO")==0 || strcmp(mod, "AGERA")==0 || strcmp(mod, "GEMERA")==0 || strcmp(mod, "REGERA")==0)
        {
            return 1;
        }
    }
    else if(strcmp(m, "CHERY") == 0)
    {
        if(strcmp(mod, "TIGGO 2")==0 || strcmp(mod, "TIGGO 4")==0 || strcmp(mod, "TIGGO 8")==0 ||
                strcmp(mod, "QQ")==0 || strcmp(mod, "ARRIZO")==0)
        {
            return 1;
        }
    }

    return 0;
}
Auto cargar_auto()
{
    Auto a;
    int valido = 0;
    char aux[50];

    printf("\n========================================\n");
    printf("           CARGA DE NUEVO AUTO          \n");
    printf("========================================\n");

    // 1. PATENTE
    do
    {
        printf("Patente (AA 123 CD): ");
        fflush(stdin); // LIMPIEZA IMPORTANTE
        gets(aux);
        for(int i=0; i<strlen(aux); i++)
        {
            if(aux[i]>='a' && aux[i]<='z')
            {
                aux[i]-=32;
            }
        }

        if(strlen(aux)==9 && aux[0]>='A' && aux[0]<='Z' && aux[1]>='A' && aux[1]<='Z' && aux[2]==' ' &&
                aux[3]>='0' && aux[3]<='9' && aux[4]>='0' && aux[4]<='9' && aux[5]>='0' && aux[5]<='9' &&
                aux[6]==' ' && aux[7]>='A' && aux[7]<='Z' && aux[8]>='A' && aux[8]<='Z')
        {
            if(existe_patente_en_archivo(aux)==1)
            {
                printf("ERROR: Patente duplicada.\n");
                valido = 0;
            }
            else
            {
                strcpy(a.patente, aux);
                valido = 1;
            }
        }
        else
        {
            printf("ERROR: Formato invalido. Use: AA 123 CD\n");
            valido = 0;
        }
    }
    while(valido == 0);

    // 2. MARCA
    valido = 0;
    do
    {
        printf("Marca: ");
        fflush(stdin);
        gets(aux);
        if(es_marca_valida(aux)==1)
        {
            strcpy(a.marca, aux);
            valido = 1;
        }
        else
        {
            printf("ERROR: Marca no reconocida (Ej: Ford, Fiat, BMW...)\n");
            valido = 0;
        }
    }
    while(valido == 0);

    // 3. MODELO
    valido = 0;
    do
    {
        printf("Modelo: ");
        fflush(stdin);
        gets(aux);
        if(es_modelo_valido(a.marca, aux)==1)
        {
            strcpy(a.modelo, aux);
            valido = 1;
        }
        else
        {
            printf("ERROR: El modelo '%s' no corresponde a la marca '%s' o no existe.\n", aux, a.marca);
            valido = 0;
        }
    }
    while(valido == 0);

    // 4. ANIO
    do
    {
        a.anio = ingresar_entero("Anio (1885 - 2025): ");
        if(a.anio >= 1885 && a.anio <= 2025)
        {
            valido = 1;
        }
        else
        {
            printf("ERROR: Anio fuera de rango.\n");
            valido = 0;
        }
    }
    while(valido == 0);

    a.kms = ingresar_entero("Kilometraje: ");
    a.precioDeAdquisicion = ingresar_float("Precio de Adquisicion: ");

    strcpy(a.titular.nombre, "Concesionaria");
    strcpy(a.titular.dni, "00000000");
    strcpy(a.titular.telefono, "000000000");
    strcpy(a.titular.direccion, "S/N");
    strcpy(a.titular.rol, "concesionaria");

    a.precioFinal = a.precioDeAdquisicion * 1.20;
    return a;
}


void agregar_auto_stock()
{
    Auto a = cargar_auto();
    FILE *f = fopen(ARCHIVO_AUTOS, "ab");
    if(f != NULL)
    {
        fwrite(&a, sizeof(Auto), 1, f);
        fclose(f);
        printf("\nAuto agregado al stock exitosamente.\n");
    }
    else
    {
        printf("Error al abrir el archivo.\n");
    }
}

void mostrar_auto(Auto a)
{
    printf("\n------------------------------\n");
    printf(" Patente: %s\n", a.patente);
    printf(" Marca:   %s\n", a.marca);
    printf(" Modelo:  %s\n", a.modelo);
    printf(" Anio:    %d\n", a.anio);
    printf(" Kms:     %d\n", a.kms);
    printf(" Costo:   $%.2f\n", a.precioDeAdquisicion);
    printf(" Venta:   $%.2f\n", a.precioFinal);
    printf(" Titular: %s\n", a.titular.nombre);
    printf("------------------------------\n");
}

void mostrar_todos_autos(char archivo[])
{
    FILE *f = fopen(archivo, "rb");
    if(f == NULL)
    {
        printf("No hay autos registrados.\n");
        return;
    }
    Auto a;
    printf("\n--- LISTADO COMPLETO (Stock) ---\n");
    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        mostrar_auto(a);
    }
    fclose(f);
}

// --- LOGICA COMPLETA DE MODIFICAR ---
void modificar_auto_stock()
{
    char patenteBuscada[20];
    char aux[50];
    int valido;

    mostrar_todos_autos(ARCHIVO_AUTOS);

    do
    {
        printf("\nIngrese la patente del auto a modificar: ");
        fflush(stdin);
        gets(patenteBuscada);
    }
    while(strlen(patenteBuscada) == 0);

    for(int i=0; i<strlen(patenteBuscada); i++)
    {
        if(patenteBuscada[i] >= 'a' && patenteBuscada[i] <= 'z')
        {
            patenteBuscada[i] -= 32;
        }
    }

    FILE *f = fopen(ARCHIVO_AUTOS, "r+b");
    if(f == NULL)
    {
        printf("Error.\n");
        return;
    }

    Auto a;
    int encontrado = 0;

    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        if(strcmp(a.patente, patenteBuscada) == 0)
        {
            encontrado = 1;
            int subOp = 0;

            do
            {
                system("cls");
                printf("\n--- EDITANDO AUTO: %s ---\n", a.patente);
                mostrar_auto(a);

                printf("\nSELECCIONE CAMPO A MODIFICAR:\n");
                printf("1. Modificar Patente\n");
                printf("2. Modificar Marca\n");
                printf("3. Modificar Modelo\n");
                printf("4. Modificar Anio\n");
                printf("5. Modificar Kilometraje\n");
                printf("6. Modificar Precio de Adquisicion\n");
                printf("0. GUARDAR Y SALIR\n");
                printf("-----------------------------\n");

                subOp = ingresar_entero("Su eleccion: ");

                switch(subOp)
                {
                case 1:
                    valido = 0;
                    do
                    {
                        printf("\nNueva Patente (AA 123 CD): ");
                        fflush(stdin);
                        gets(aux);
                        for(int i=0; i<strlen(aux); i++)
                        {
                            if(aux[i]>='a' && aux[i]<='z')
                            {
                                aux[i]-=32;
                            }
                        }

                        if(strlen(aux)==9 && aux[0]>='A' && aux[0]<='Z' && aux[1]>='A' && aux[1]<='Z' &&
                                aux[2]==' ' && aux[3]>='0' && aux[3]<='9' && aux[4]>='0' && aux[4]<='9' &&
                                aux[5]>='0' && aux[5]<='9' && aux[6]==' ' && aux[7]>='A' && aux[7]<='Z' &&
                                aux[8]>='A' && aux[8]<='Z')
                        {
                            strcpy(a.patente, aux);
                            valido = 1;
                            printf("Patente cambiada.\n");
                        }
                        else
                        {
                            printf("Formato invalido.\n");
                        }
                    }
                    while(valido == 0);

                    system("pause");

                    break;

                case 2:
                    valido = 0;
                    do
                    {
                        printf("\nNueva Marca: ");
                        fflush(stdin);
                        gets(aux);
                        if(es_marca_valida(aux)==1)
                        {
                            strcpy(a.marca, aux);
                            valido = 1;
                            printf("Marca actualizada.\n");
                        }
                        else
                        {
                            printf("Marca no valida.\n");
                        }
                    }
                    while(valido == 0);

                    system("pause");
                    break;

                case 3:
                    valido = 0;
                    do
                    {
                        printf("\nNuevo Modelo (para %s): ", a.marca);
                        fflush(stdin);
                        gets(aux);
                        if(es_modelo_valido(a.marca, aux)==1)
                        {
                            strcpy(a.modelo, aux);
                            valido = 1;
                            printf("Modelo modificado correctamente.\n");
                        }
                        else
                        {
                            printf("ERROR: Modelo invalido para la marca %s.\n", a.marca);
                        }
                    }
                    while(valido == 0);

                    system("pause");

                    break;

                case 4:
                    valido = 0;
                    do
                    {
                        a.anio = ingresar_entero("Nuevo Anio: ");
                        if(a.anio >= 1885 && a.anio <= 2025)
                        {
                            valido = 1;
                        }
                        else
                        {
                            printf("Anio fuera de rango.\n");
                        }
                    }
                    while(valido == 0);

                    break;

                case 5:
                    a.kms = ingresar_entero("Nuevo Kilometraje: ");
                    break;

                case 6:
                    a.precioDeAdquisicion = ingresar_float("Nuevo Precio: ");
                    a.precioFinal = a.precioDeAdquisicion * 1.20;
                    printf("Precio Final actualizado a: $%.2f\n", a.precioFinal);
                    system("pause");
                    break;

                case 0:
                    printf("Guardando cambios...\n");
                    break;

                default:
                    printf("Opcion invalida.\n");
                    break;
                }
            }
            while(subOp != 0);

            fseek(f, -sizeof(Auto), SEEK_CUR);
            fwrite(&a, sizeof(Auto), 1, f);

            printf("Auto modificado correctamente.\n");

            break;
        }
    }

    if(encontrado == 0)
    {
        printf("No se encontro un auto con la patente %s en el stock.\n", patenteBuscada);
    }
    fclose(f);
}
