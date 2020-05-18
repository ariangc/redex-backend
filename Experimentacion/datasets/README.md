# Datasets del proyecto

Los datasets presentan la siguiente estructura:

n m  
ICAO1,CONTINENT1,COUNTRY1,CITY1,CITY-CODE1  
ICAO2,CONTINENT2,COUNTRY2,CITY2,CITY-CODE2  
...  
ICAOn,CONTINENTn,COUNTRYn,CITYn,CITY-CODEn  
ICAO-O1,ICAO-D1,DEPT1,ARRV1  
ICAO-O2,ICAO-D2,DEPT2,ARRV2  
...  
ICAO-Om,ICAO-Dm,DEPTm,ARRVm  
  
Donde:  
  
n = Número de aeropuertos  
m = Número de vuelos  
  
ICAOi = Código ICAO del aeropuerto i  
CONTINENTi = Continente del aeropuerto i  
COUNTRYi = País del aeropuerto i  
CITYi = Ciudad del aeropuerto i  
CITY-CODE = Código de ciudad del aeropuerto i  
  
ICAO-Oj = Código ICAO del origen del vuelo j  
ICAO-Dj = Código ICAO del destino del vuelo j  
DEPTj = Hora (hh:mm) de partida del vuelo j  
ARRVj = Hora (hh:mm) de llegada del vuelo j  

