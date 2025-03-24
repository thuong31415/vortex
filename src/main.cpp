#include "server/Server.h"

int main() {
    Server server{8080};
    server.Start();
}
