#line 1 "c:\\Users\\54911\\Documents\\GitHub\\C\\Proyecto 9 - IoT\\Esp\\BrokerAPI\\BrokerAPI.h"
#ifndef _BROKER_API_H
#define _BROKER_API_H

    #include <list>
    #include <algorithm>
    
    class IVar {
        private:
            String name;
    }

    class DVar<T> : IVar {
        private:
            T value;
        public:
            T val();
            String valStr();
    }

    class Device {
        private:
            BrokerAPI broker;
            String topic;
            list<IVar> vars; 

        public:
            void publish();
    }

#endif