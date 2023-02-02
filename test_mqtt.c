#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>

#define BUFFER_SIZE 50000
// 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
int main(void)
{
    char buffer[BUFFER_SIZE];
    FILE *fp;

    fp = fopen("text.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    size_t bytes_read = fread(buffer, 1, BUFFER_SIZE, fp);
    if (bytes_read == 0)
    {
        perror("Error reading file");
        fclose(fp);
        return 1;
    }

    int rc;
    struct mosquitto *mosq;

    mosquitto_lib_init();

    mosq = mosquitto_new("publisher-test", true, NULL);

    rc = mosquitto_connect(mosq, "122.170.105.253", 1883, 60);
    if (rc != 0)
    {
        printf("Client could not connect to broker! Error Code: %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    }
    printf("We are now connected to the broker!\n");

    mosquitto_publish(mosq, NULL, "airpro/dev/to/cloud", BUFFER_SIZE, buffer, 0, false);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();

    printf("Read %ld bytes from text.txt:\n%.*s\n", bytes_read, (int)bytes_read, buffer);

    fclose(fp);
    return 0;
}
