#include "headers/thread.hpp"

using namespace std;

thread_struct::thread_struct(parcel_struct creator):
    addr(creator.addr()),
    target_action(creator.target_action()),
    environment_data(creator.environment_data())
{
}
