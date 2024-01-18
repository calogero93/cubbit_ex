# cubbit_ex

Cubbit Interview Exercise

## Build Project

`docker build -t <name image> .`

## Run Container

`docker run --name=<name container> -d <name image>`

## Enter on container shell

`docker exec -it <name container> /bin/bash`

## Start Server

`./server/server &`

## Start Client

# Create test file

` cd client && touch file.txt && echo "abcde" > file.txt`

# Start client

`./horcrux save -n numero_horcrux file.txt`
`./horcrux load id_generato percorso_scelto`
