#!/bin/bash

echo "Building test app.."

# Container name
container="deploy-freertos"

# Tag Docker Image
tag="freertos"
# Project name
project="esp32-freeRTOS2"
# branch
branch="main"
# dir name
app="test"
# App to be compiled
macro="APP_TEST"
# App version
app_version="1.0.0"

if [ -d "images/${app}" ]; then
    rm -r "images/${app}"
fi

wget -q https://github.com/zimbora/${project}/archive/refs/heads/${branch}.zip
unzip ${branch}.zip
rm ${branch}.zip
mv ${project}-${branch} ${project}

mkdir -p ${project}/src/app/${app}
cp *.cpp *.c *.h *.hpp deploy.sh ${project}/src/app/${app}

docker build \
--build-arg PROJECT=$project \
--build-arg APP=$app \
--build-arg MACRO=$macro \
--build-arg APP_VERSION=$app_version \
-t ${tag} .

#docker run -it --name $container ${tag}:latest
#docker stop $container

docker create --name ${container} ${tag}
docker cp ${container}:${project}/images/${app} ./images/

docker rm $container
#docker rmi $tag

rm -r $project
