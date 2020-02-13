#include "MYClient.h"

int main(int argc, char **argv)
{
    MyClient client("192.168.0.13");

    client.start();

    return EXIT_SUCCESS;
}