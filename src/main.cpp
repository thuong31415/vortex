#include "server/Server.h"

int main() {
    Server server{8080, 5};
    server.Start();
}
