#include "headers/parcel.hpp"

using namespace std;

XPI_Addr& parcel_struct::addr(){
    return records.top().addr;
}

string& parcel_struct::target_action(){
    return records.top().target_action;
}

vector<unsigned char>& parcel_struct::environment_data(){
    return records.top().environment_data;
}
