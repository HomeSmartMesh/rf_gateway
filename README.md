# Raspberry pi c++ RF Gateway
Serial interface between the STM32 dongle and a the MQTT Broquer
* [STM32 RF Dongle firmware](https://github.com/HomeSmartMesh/IoT_Frameworks/tree/master/stm32_rf_dongle/rf_bridge)

# Usage
build
`scons` 

run
`./gateway` 

Config file used is `"./rf_gateway_config.json"`

#C, Cpp clients
##Install
```bash
sudo apt-get install libmosquitto-dev
sudo apt-get install libmosquittopp-dev
```

### Environment and Dependencies
- Scons : simple maintenance for a samll project
- C++11 : Modern c++
- Boost 1.60 (filesystem) : see install instructions
- json for modern c++ : json as easy to use as in javascript https://github.com/nlohmann/json (to be replaced by Poco::JSON)

### Boost installation on the Raspberry pi
Minimal install for filesystem support:
```
mkdir boost
cd boost
wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.bz2
tar xvfo boost_1_60_0.tar.bz2
cd boost_1_60_0
./bootstrap.sh --with-libraries=filesystem,system
sudo ./b2 install
```
