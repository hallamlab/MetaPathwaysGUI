#include "globaldatatransit.h"

GlobalDataTransit * GlobalDataTransit::globalDataTransit = 0;


GlobalDataTransit *GlobalDataTransit::getGlobalDataTransit() {
    if( GlobalDataTransit::globalDataTransit == 0 )
        GlobalDataTransit::globalDataTransit = new GlobalDataTransit();
    return GlobalDataTransit::globalDataTransit;
}

GlobalDataTransit::GlobalDataTransit()
{


}
