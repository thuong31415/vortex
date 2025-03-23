#include "server/Server.h"

int main() {
    Server server{};
    server.Run(8080);
}
