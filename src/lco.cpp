#include "headers/lco.hpp"

using namespace std;
using namespace hpx;

//Should this return a copy? How do we know how big?
void* xpi_future::value(){
    gotten=true;
    return hpx_future.get();
}

void xpi_future::trigger(void* data){
    //should we set the bufer? How big?
    hpx_promise.set_value(data);
}

size_t xpi_future::size(){
    return buffer.size();
}

bool had_get(){
    return gotten;
}
