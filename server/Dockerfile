FROM gcc:latest

COPY server /app/server


WORKDIR /app/server

RUN g++ -std=c++20 -Wall -Wextra -o server server/main.cpp -luuid -lboost_system -lboost_thread -pthread -lssl -lcrypto

CMD ["/app/server/server"]