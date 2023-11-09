# Use a base image that has bash and apt (e.g., ubuntu)
FROM zimbora/arduino-deploy:latest

# Set a default value (optional)
ENV BASE_DIR=/root
ARG PROJECT="esp32-freeRTOS2"
ARG APP="test"
ARG MACRO="APP_TEST"
ARG APP_VERSION=""


# Copy the script into the container
COPY $PROJECT $PROJECT

# Make the script executable
RUN chmod +x ${PROJECT}/src/app/${APP}/deploy.sh

RUN ${PROJECT}/src/app/${APP}/deploy.sh -d $BASE_DIR -p $PROJECT -a $APP -m $MACRO -v $APP_VERSION --docker > build_output.txt 2>&1

# Set the script as the default command to run when starting the container
#CMD ["${PROJECT}/src/app/deploy.sh", "-d", "$BASE_DIR", "-p", "$PROJECT", "-a", "$APP", -v, "$APP_VERSION"]

