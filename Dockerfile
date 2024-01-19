FROM debian:bullseye-slim

# Aggiorna il sistema e installa le dipendenze necessarie
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    g++ \
    libssl-dev \
    libboost-all-dev \
    uuid-dev

# Crea la directory di lavoro
WORKDIR /app

# Copia i file sorgente nell'immagine
COPY . .

# Compila il programma C++
RUN g++ -std=c++2a -Wall -Wextra -o server/server server/main.cpp -luuid -lboost_system -lboost_thread -pthread -lssl -lcrypto
RUN g++ -std=c++2a -Wall -Wextra -o client/horcrux client/src/main.cpp -luuid -lboost_system -lboost_thread -pthread -lssl -lcrypto

# Comando predefinito per eseguire il programma quando il container viene avviato
CMD /bin/bash -c "./server/server"
