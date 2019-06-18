#include "sapi_i18n_es.h"
#include "c_i18n_es.h"

#include "sapi.h"

entero funcionPrincipal( vacio )
{
   // Config
   configurarPlataforma();
   
   mientras( VERDADERO ){
      // Repetir
      intercambiarPin( LEDB );
      retardo( 500 );
   }
   
   retornar 0;
}