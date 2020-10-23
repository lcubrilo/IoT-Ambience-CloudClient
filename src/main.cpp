#include <iostream>
#include "Wolk.h"
using namespace std;

int main()
{
    wolkabout::Device device("pi", "raspberry");

    std::unique_ptr<wolkabout::Wolk> wolk = wolkabout::Wolk::newBuilder(device).build();

    wolk->connect();
    cout<<"Hello world";
}
